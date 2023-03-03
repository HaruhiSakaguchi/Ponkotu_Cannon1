#include "MODEL_COLLISION.h"
#include "graphic.h"
#include "mathUtil.h"

//�~�j�}�b�v�f�[�^�����p�̃N���X�@��������������B
MODEL_COLLISION::MODEL_COLLISION(class Map* map, int numTriangles, int numVertices, int* indices, VECTOR* vertices, const char* name)
	: mMap(map)
	, OPositions(nullptr)
{
	mMap->AddCollisions(this);

	SetName(name);

	NumTriangles = numTriangles;
	NumVertices = numVertices;

	allocateMemory();

	for (int i = 0; i < NumVertices; i++)
	{
		OPositions[i] = vertices[i];
	}

	for (int i = 0; i < NumIndices; i++)
	{
		Indices[i] = indices[i];
	}

	for (int i = 0; i < NumVertices; i++)
	{
		ONormals[i].set(0, 1, 0);
	}

}


MODEL_COLLISION::~MODEL_COLLISION()
{
	if (OPositions)delete[] OPositions;
	if (ONormals)delete[] ONormals;
	if (Indices)delete[] Indices;
	if (Colors)delete[] Colors;
	mMap->RemoveCollisions(this);
}

void MODEL_COLLISION::allocateMemory()
{
	if (NumVertices == 0) {
		print("���_����ݒ肵�Ă��Ȃ�");
	}
	if (NumTriangles == 0) {
		print("�O�p�|���S������ݒ肵�Ă��Ȃ�");
	}
	//�ʒu�Ɩ@���x�N�g���i���_�o�b�t�@�j
	OPositions = new VECTOR[NumVertices];
	ONormals = new VECTOR[NumVertices];
	//�v�Z��̒��_�̐F�i���邳�j
	Colors = new COLOR[NumVertices];
	//�C���f�b�N�X�o�b�t�@
	NumIndices = NumTriangles * 3;
	Indices = new int[NumIndices];
}
