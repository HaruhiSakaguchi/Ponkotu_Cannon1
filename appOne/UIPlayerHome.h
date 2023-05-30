#pragma once
#include "UIScreen.h"
class UIPlayerHome :
    public UIScreen
{
public:
    UIPlayerHome(class PlayerHome* owner);
    void draw()override;
    void Update()override;
private:
    int mHpGaugeWidth;
    class PSideCharacterActor* mOwner;
    VECTOR2 mPos;
    float mHeight = 50.0f;
    Button* mGoButton;
    Button* mReturnButton;
};

