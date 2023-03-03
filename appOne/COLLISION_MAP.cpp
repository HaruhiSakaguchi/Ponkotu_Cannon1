#include "FILE_BUFFER.h"
#include "NAME.h"
#include "COLLISION_MAP.h"
#include "CELL.h"


COLLISION_MAP::COLLISION_MAP(float len, const char* filename)
	:Len(len)//CUBE��CELL�̈�ӂ̒���
	, mGame(nullptr)
{
	//Triangle data�����[�h
	loadData(filename);
	//CELL�̉��A�O��A�c�̐�
	NumX = int((MaxX - MinX) / Len) + 1;
	NumZ = int((MaxZ - MinZ) / Len) + 1;
	NumY = int((MaxY - MinY) / Len) + 1;
	//CELL�̂R�����z��𓮓I�m��
	Cells = new CELL * *[NumY];
	for (int y = 0; y < NumY; y++) {
		Cells[y] = new CELL * [NumZ];
		for (int z = 0; z < NumZ; z++) {
			Cells[y][z] = new CELL[NumX];
		}
	}
	//�u�eCELL�̍������v�̍��W��ݒ�
	// �ł��u�������v�̃Z���̍��W��MinX,MinZ,MinY�i��������j
	for (int y = 0; y < NumY; y++) {
		for (int z = 0; z < NumZ; z++) {
			for (int x = 0; x < NumX; x++) {
				Cells[y][z][x].setLen_Pos(Len, MinX + Len * x, MinY + Len * y, MinZ + Len * z);
				CellS.emplace_back(&Cells[y][z][x]);
			}
		}
	}
	//�u�eCELL�v�Ɋ܂܂��O�p�|���S����o�^
	for (int y = 0; y < NumY; y++) {
		for (int z = 0; z < NumZ; z++) {
			for (int x = 0; x < NumX; x++) {
				for (int i = 0; i < NumTriangles; i++) {
					Cells[y][z][x].regist(Triangles[i]);
				}
			}
		}
	}
}

COLLISION_MAP::~COLLISION_MAP()
{
	//CELL�̂R�����z����J��
	for (int y = 0; y < NumY; y++) {
		for (int z = 0; z < NumZ; z++) {
			delete[] Cells[y][z];
		}
		delete[] Cells[y];
	}
	delete[] Cells;

	while (!CellS.empty())
	{
		CellS.pop_back();
	}
	while (!TriangleS.empty())
	{
		TriangleS.pop_back();
	}
	//Triangles���J��
	SAFE_DELETE_ARRAY(Triangles);

}

void COLLISION_MAP::loadData(const char* fileName)
{
	FILE_BUFFER fb(fileName);

	//�P�T�ڂ�Triangle���W�v�ƃf�[�^�`�F�b�N
	NAME name;
	int cntIndices = 0;
	fb.readString();
	while (!fb.end()) {
		if (fb == 'i') {
			name = fb.readString();
			int numIndices = fb.readInt();
			NumTriangles += numIndices / 3;
			cntIndices++;
		}
		if (fb == 'v') {
			NAME vName = fb.readString();
			if (name != vName) {
				WARNING(1, "COLLISION_MAP::loadData()", "i�̎���v�����Ă��Ȃ�");
			}
		}
		fb.readString();
	}
	Triangles = new TRIANGLE[NumTriangles];
	for (int i = 0; i < NumTriangles; i++)
	{
		TriangleS.emplace_back(&Triangles[i]);
	}

	//�Q�T�ڂ�Triangle������
	fb.restart();
	int* indices;
	int numIndices;
	VECTOR* vertices;
	int numVertices;
	int idx = 0;
	while (cntIndices > 0) {
		//index
		fb.readString();
		while (fb != 'i')fb.readString();
		fb.readString();
		numIndices = fb.readUnsigned();
		indices = new int[numIndices];
		for (int i = 0; i < numIndices; i++)indices[i] = fb.readUnsigned();
		//vertex
		fb.readString();
		while (fb != 'v')fb.readString();
		fb.readString();
		fb.readString();
		numVertices = fb.readUnsigned();
		vertices = new VECTOR[numVertices];

		for (int i = 0; i < numVertices; i++) {
			vertices[i].x = fb.readFloat();
			vertices[i].y = fb.readFloat();
			vertices[i].z = fb.readFloat();
			if (vertices[i].x < MinX)MinX = vertices[i].x;
			if (vertices[i].x > MaxX)MaxX = vertices[i].x;
			if (vertices[i].y < MinY)MinY = vertices[i].y;
			if (vertices[i].y > MaxY)MaxY = vertices[i].y;
			if (vertices[i].z < MinZ)MinZ = vertices[i].z;
			if (vertices[i].z > MaxZ)MaxZ = vertices[i].z;
		}
		//triangle
		int n = 0;
		while (n < numIndices) {
			int j = indices[n++];
			int k = indices[n++];
			int l = indices[n++];
			Triangles[idx++].create(vertices[j], vertices[k], vertices[l]);
		}
		SAFE_DELETE_ARRAY(indices);
		SAFE_DELETE_ARRAY(vertices);

		cntIndices--;
	}
	if (idx != NumTriangles) {
		WARNING(1, "load error collision data ", "");
	}
}


CELL* COLLISION_MAP::SelectCell(const VECTOR* pos)
{
	int y = int((pos->y - MinY) / Len);
	int z = int((pos->z - MinZ) / Len);
	int x = int((pos->x - MinX) / Len);
	if (0 <= y && y < NumY &&
		0 <= z && z < NumZ &&
		0 <= x && x < NumX) {
		return &Cells[y][z][x];
	}
	return nullptr;
}

int COLLISION_MAP::capsule_triangles(VECTOR* pos, float* dirY, int* jumpFlag,
	float speed, float radius, float height, float degree, float* floor)
{
	//�I�u�W�F�N�g��AABB�Əd�Ȃ�CELL�Ƃ̓����蔻��-------------------------
	//�I�u�W�F�N�g��pos����AABB�̍��W�����
	float offset = radius + 0.1f;
	float offsetY = height;
	int flag = 0;
	VECTOR p[8] = {
		VECTOR(pos->x - offset, pos->y + offsetY, pos->z - offset),
		VECTOR(pos->x + offset, pos->y + offsetY, pos->z - offset),
		VECTOR(pos->x - offset, pos->y + offsetY, pos->z + offset),
		VECTOR(pos->x + offset, pos->y + offsetY, pos->z + offset),
		VECTOR(pos->x - offset, pos->y, pos->z - offset),
		VECTOR(pos->x + offset, pos->y, pos->z - offset),
		VECTOR(pos->x - offset, pos->y, pos->z + offset),
		VECTOR(pos->x + offset, pos->y, pos->z + offset),
	};
	//�����ς�CELL�̃|�C���^�z��B�d������������邽�߂̂��́B
	CELL* processedCells[8] = { 0 };
	int step = 0;//�K�i��������Ȃ��t���b�O
	int idx = 0;
	for (int i = 0; i < 8; i++) {
		//p[ i ]������CELL�����肷��
		//AABB�̒��_���ꂼ�ꂪcell��AABB�ɓ����Ă��邩�𔻕ʂ���B�����Ă����瓖���蔻��i���ȊO���j�B
		CELL* cell = SelectCell(&p[i]);
		if (cell) {//p[i]��cell��AABB�ɓ����Ă�����
			//����cell�������ς݂��`�F�b�N����
			int j;
			for (j = 0; j < idx; j++) {
				if (processedCells[j] == cell) {
					break;
				}
			}
			//�����ς݂łȂ��̂œ����蔻��i���ȊO�j
			if (j == idx) {
				flag = cell->capsule_triangles(pos, dirY, jumpFlag,
					radius, height, degree, &step);
				processedCells[idx++] = cell;
			}
		}
	}
	//pos������CELL��I�����A���n�̏������s���i���̂ݓ����蔻��j
	CELL* cell = SelectCell(pos);
	if (cell) {
		flag = cell->capsule_floor_triangles(pos, dirY, jumpFlag, speed,
			radius, height, degree, step, floor);
	}

	return flag;
}

int COLLISION_MAP::capsule_triangles_walls(CharacterActor* actor)
{
	float offset = actor->GetRadius();
	float offsetY = actor->GetHeight();
	VECTOR pos = actor->GetPosition();
	float dirY = 0;
	int jumpFlag = 0;
	int flag = 0;
	float speed = 0.0f;
	float radius = offset;
	float height = offsetY;
	float degree = 0.0f;
	VECTOR p[8] = {
		VECTOR(pos.x - offset, pos.y + offsetY, pos.z - offset),
		VECTOR(pos.x + offset, pos.y + offsetY, pos.z - offset),
		VECTOR(pos.x - offset, pos.y + offsetY, pos.z + offset),
		VECTOR(pos.x + offset, pos.y + offsetY, pos.z + offset),
		VECTOR(pos.x - offset, pos.y, pos.z - offset),
		VECTOR(pos.x + offset, pos.y, pos.z - offset),
		VECTOR(pos.x - offset, pos.y, pos.z + offset),
		VECTOR(pos.x + offset, pos.y, pos.z + offset),
	};
	//�����ς�CELL�̃|�C���^�z��B�d������������邽�߂̂��́B
	CELL* processedCells[8] = { 0 };
	int step = 0;//�K�i��������Ȃ��t���b�O
	int idx = 0;
	for (int i = 0; i < 8; i++) {
		//p[ i ]������CELL�����肷��
		CELL* cell = SelectCell(&p[i]);
		if (cell) {
			//����cell�������ς݂��`�F�b�N����
			int j;
			for (j = 0; j < idx; j++) {
				if (processedCells[j] == cell) {
					break;
				}
			}
			//�����ς݂łȂ��̂œ����蔻��
			if (j == idx) {
				flag = cell->capsule_triangles(&pos, &dirY, &jumpFlag,
					radius, height, degree, &step);
				processedCells[idx++] = cell;
			}
		}
	}

	return flag;
}

int COLLISION_MAP::capsule_triangles(CharacterActor* actor)
{
	//�I�u�W�F�N�g��AABB�Əd�Ȃ�CELL�Ƃ̓����蔻��-------------------------
	//�I�u�W�F�N�g��pos����AABB�̍��W�����
	float offset = actor->GetRadius();
	float offsetY = actor->GetHeight();
	VECTOR pos = actor->GetPosition();
	float dirY = 0;
	int jumpFlag = 0;
	int flag = 0;
	float speed = 0.0f;
	float radius = offset;
	float height = offsetY;
	float degree = 0.0f;
	VECTOR p[8] = {
		VECTOR(pos.x - offset, pos.y + offsetY, pos.z - offset),
		VECTOR(pos.x + offset, pos.y + offsetY, pos.z - offset),
		VECTOR(pos.x - offset, pos.y + offsetY, pos.z + offset),
		VECTOR(pos.x + offset, pos.y + offsetY, pos.z + offset),
		VECTOR(pos.x - offset, pos.y, pos.z - offset),
		VECTOR(pos.x + offset, pos.y, pos.z - offset),
		VECTOR(pos.x - offset, pos.y, pos.z + offset),
		VECTOR(pos.x + offset, pos.y, pos.z + offset),
	};
	//�����ς�CELL�̃|�C���^�z��B�d������������邽�߂̂��́B
	CELL* processedCells[8] = { 0 };
	int step = 0;//�K�i��������Ȃ��t���b�O
	int idx = 0;
	for (int i = 0; i < 8; i++) {
		//p[ i ]������CELL�����肷��
		CELL* cell = SelectCell(&p[i]);
		if (cell) {
			//����cell�������ς݂��`�F�b�N����
			int j;
			for (j = 0; j < idx; j++) {
				if (processedCells[j] == cell) {
					break;
				}
			}
			//�����ς݂łȂ��̂œ����蔻��
			if (j == idx) {
				flag = cell->capsule_triangles(&pos, &dirY, &jumpFlag,
					actor->GetRadius(), actor->GetHeight(), actor->GetSlant(), &step);
				processedCells[idx++] = cell;
			}
		}
	}

	CELL* cell = SelectCell(&pos);
	if (cell) {
		flag = cell->capsule_floor_triangles(&pos, &dirY, &jumpFlag, speed,
			radius, height, degree, step);
	}

	return flag;
}


//�J�������}�b�v�ɐH�����܂Ȃ��悤�ɂ���B�i�|���S���Ƃ̌�_�������̏I�_ep�ɂȂ�j
int COLLISION_MAP::segment_triangles(VECTOR& sp, VECTOR* ep, float correct)
{
	for (int y = 0; y < NumY; y++) {
		for (int z = 0; z < NumZ; z++) {
			for (int x = 0; x < NumX; x++) {
				if (Cells[y][z][x].select(sp, *ep)) {
					if (Cells[y][z][x].segment_triangles(sp, ep, correct))
						return 1;
				}
			}
		}
	}
	return 0;
}
int COLLISION_MAP::segment_trianglesEco(VECTOR& sp, VECTOR* ep, float correct)
{
	//�L�����̂���Z���ƃJ�����̂���Z�����������A�אڂ��Ă���ꍇ�͂��ꂮ�炢�ł���
	int flag = 0;
	CELL* cell = SelectCell(&sp);
	if (cell) {
		if (cell->segment_triangles(sp, ep, correct)) {
			flag = 1;
		}
	}
	if (flag == 0) {
		CELL* cell = SelectCell(ep);
		if (cell) {
			if (cell->segment_triangles(sp, ep, correct)) {
				flag = 1;
			}
		}
	}
	return flag;
}

