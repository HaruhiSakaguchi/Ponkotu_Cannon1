#include "MODEL_COLLISION.h"
#include "graphic.h"
#include "mathUtil.h"

//ミニマップデータを作る用のクラス　作ったら消去する。
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
		print("頂点数を設定していない");
	}
	if (NumTriangles == 0) {
		print("三角ポリゴン数を設定していない");
	}
	//位置と法線ベクトル（頂点バッファ）
	OPositions = new VECTOR[NumVertices];
	ONormals = new VECTOR[NumVertices];
	//計算後の頂点の色（明るさ）
	Colors = new COLOR[NumVertices];
	//インデックスバッファ
	NumIndices = NumTriangles * 3;
	Indices = new int[NumIndices];
}
