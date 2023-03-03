#pragma once
#include "Map.h"
#include <string>

class MODEL_COLLISION
{
public:
    MODEL_COLLISION(class Map* map, int numTriangles, int numVertices, int* indices, VECTOR* vertices, const char* name = "name");
    void allocateMemory();
    virtual ~MODEL_COLLISION();
    class Map* GetMap() { return mMap; }
    void SetNormals(VECTOR* normals) { ONormals = normals; }
    void SetIndices(int* indices) { Indices = indices; }
    void SetVertices(VECTOR* position) { OPositions = position; }
    void SetNumTriangles(int num) { NumTriangles = num; }
    void SetNumVertices(int num) { NumVertices = num; }
    void SetName(const char* Name) { name = Name; }
    const char* GetName() { return name.c_str(); }
    VECTOR* GetOPosition() { return OPositions; }
    int* GetIndices() { return Indices; }
    int GetNumVertices() { return NumVertices; }
    int GetNumIndices() { return NumIndices; }
    int GetNumTriangles() { return NumTriangles; }
private:
    class Map* mMap;
protected:
    bool AddFlag = true;
    int NumVertices = 0;
    int NumTriangles = 0;
    int NumIndices = 0;

    //計算前のOriginalデータ
    VECTOR* OPositions = 0;
    VECTOR TmpPosition = 0;
    VECTOR* ONormals = 0;
    //計算後のデータ
    COLOR* Colors = 0;
    //頂点を選択するインデックス配列
    int* Indices = 0;
    std::string name = "name";
};