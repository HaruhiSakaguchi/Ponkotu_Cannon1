#pragma once
#include "MeshComponent.h"
class BatchMeshComponent :
    public MeshComponent
{
public:
    BatchMeshComponent(class Actor* owner);
    ~BatchMeshComponent();
    void SetBatch(const char* name);
    void Draw()override;
    void Update()override;
private:
    class BATCH* mBatch;
};

