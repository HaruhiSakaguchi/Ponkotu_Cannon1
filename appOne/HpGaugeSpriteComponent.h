#pragma once
#include "MeshComponent.h"
#include "MATRIX.h"
#include "VECTOR.h"
#include "CONTAINER/CONTAINER.h"
#include "CONTAINER/BATCH.h"
#include "CONTAINER/TREE.h"

class HpGaugeSpriteComponent :
    public MeshComponent
{
public:
    HpGaugeSpriteComponent(class Actor* owner, const VECTOR& offset = VECTOR(0, 0, 0));
    ~HpGaugeSpriteComponent();
    void Update()override;
    void Draw()override;
    //getter
    float GetDivmax() { return mDivMaxHp; }
    int GetMaxHp() { return mMaxHp; }
private:
    VECTOR mOffset;
    int mMaxHp;
    float mDivMaxHp;
    float mGaugeWidth;
    float mGaugePosX;
    //matrix
    class MATRIX Master;
    //ƒ‚ƒfƒ‹
    class TREE* mNormal;
    class TREE* mDanger; 
    class TREE* mDying;
    class TREE* mGauge;
};

