#pragma once
#include "PSideCharacterActor.h"

class Barricade :
    public PSideCharacterActor
{
public:
    Barricade(Game* game);
    ~Barricade();
    int SetUp()override;
    void UpdateActor()override;
    void Damage(int damage)override;
    void Dead()override;
    void SetBNum(int num) { mNum = num; }
    int GetBNum() { return mNum; }
    struct DATA
    {
        int mHp = 20;
        float mGravity = -9.8f;
        float mRadius = 0.75f;
        float mAdvSpeed = 0.5f;
        int mJumpFlag = 1;
        float mMaxDamageInterval = 0.09f;
        COLOR mImageColor = COLOR(0, 0, 255);
        VECTOR mCapsuleOffset = VECTOR(0.0f, 0.5f, 0.0f);
    };
private:
    DATA Data;
    int mNum;
    class TreeMeshComponent* mTc;
};

