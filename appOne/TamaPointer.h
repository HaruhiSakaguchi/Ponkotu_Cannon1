#pragma once
#include "CharacterActor.h"
#include "MATRIX.h"

class TamaPointer :
    public CharacterActor
{
public:
    TamaPointer(class Tama* owner);
    void UpdateActor();
    int SetUp();
    bool GetCollisionPSide() { return mCollisionPSide; }
    bool CheckCollisionPSide();
    float GetMaxDist() { return mMaxOffsetPos.z; }
private:
    class Tama* mOwner;
    MATRIX Master;
    VECTOR mMaxOffsetPos = VECTOR(0.0f,0.0f,10.0f);
    bool mCollisionPSide;
    VECTOR mTarget;
};

