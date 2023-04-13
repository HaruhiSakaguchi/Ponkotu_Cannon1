#pragma once
#include "CharacterActor.h"

class Barricade :
    public CharacterActor
{
public:
    Barricade(Game* game);
    int SetUp()override;
    void UpdateActor()override;
    void Damage(int damage)override;
    struct DATA
    {
        int mHp = 10;
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
};
