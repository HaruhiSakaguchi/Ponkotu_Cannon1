#include "Game.h"
#include "TRIANGLE.h"
#include "CharacterActor.h"

class COLLISION_MAP {
public:
	COLLISION_MAP(float len, const char* filename);
	~COLLISION_MAP();
	//�����蔻��p�f�[�^�����[�h���O�p�`������Ă���
	void loadData(const char* fileName);
	int capsule_triangles(CharacterActor* actor);
	//�L�����N�^���ރJ�v�Z���ƃX�e�[�W�̎O�p�`�Ƃ̂����蔻��
	int capsule_triangles(class VECTOR* pos, float* dirY, int* jumpFlag, float speed, float radius, float height, float degree, float* floorY = 0);
	//�ǂ݂̂̓����蔻���Ԃ�
	int capsule_triangles_walls(CharacterActor* actor);
	//�����ƃX�e�[�W�̎O�p�`�Ƃ̓����蔻��
	int segment_triangles(class VECTOR& sp, class VECTOR* ep, float correct = 1.0f);
	int segment_trianglesEco(class VECTOR& sp, class VECTOR* ep, float correct = 1.0f);
	//Getter
	float minX() { return MinX; }
	float minY() { return MinY; }
	float minZ() { return MinZ; }
	float maxX() { return MaxX; }
	float maxY() { return MaxY; }
	float maxZ() { return MaxZ; }
	int numX() { return NumX; }
	int numY() { return NumY; }
	int numZ() { return NumZ; }

	std::vector<class CELL*>& GetCells() { return CellS; }
	std::vector<class TRIANGLE*>& GetTriangles() { return TriangleS; }
	class TRIANGLE* GetTriangle() { return Triangles; }
	int GetNumTriangles() { return NumTriangles; }
private:
	/// <summary>
	/// pos������cell��Ԃ����Ȃ�������nullptr���Ԃ�
	/// </summary>
	class CELL* SelectCell(const class VECTOR* pos);
	std::vector<class CELL*>CellS;

	std::vector<class TRIANGLE*>TriangleS;
	//�S�Z��
	class CELL*** Cells = 0;
	float Len = 0;
	int NumX = 0, NumZ = 0, NumY = 0;

	//�e�L�X�g�Ń}�b�v���Ă񂾂��A���f������}�b�v���������
	bool AddCollisionMapsFlag = false;

	//�S�O�p�|���S��
	class TRIANGLE* Triangles = 0;
	int NumTriangles = 0;
	//�|���S�����_���W�̍ŏ��l�A�ő�l
	float MinX = 1000;
	float MaxX = -1000;
	float MinZ = 1000;
	float MaxZ = -1000;
	float MinY = 1000;
	float MaxY = -1000;

	class Game* mGame;
};