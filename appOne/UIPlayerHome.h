#pragma once
#include "UIScreen.h"

class UIPlayerHome :
    public UIScreen
{
public:
    UIPlayerHome(class PlayerHome* owner);
    void draw()override;
    void Update()override;
    void DrawAfterButton()override;
    void SetGenerate(class UIGenerate*gene) { mGenerate = gene; }
private:
    float mHpGaugeWidth;
    float mBarrierHpGaugeWidth;
    class PSideCharacterActor* mOwner;
    VECTOR2 mPos;
    float mHeight = 50.0f;
    Button* mGoButton;
    Button* mReturnButton;
    Button* mHomeLvUpButton;
    Button* mGenerateCannonButton;
    Button* mGenerateBarricadeButton;
    Button* mGenerateCannonLvUpButton;
    Button* mGenerateBarricadeLvUpButton;
    Button* mGenerateItemButton;
    class UIGenerate* mGenerate;
};

