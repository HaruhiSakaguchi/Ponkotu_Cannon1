#pragma once
#include "UIScreen.h"
#include "graphic.h"

class UIHelpBase :
    public UIScreen
{
public:
    UIHelpBase(class Game* game);
    void draw()override;
    void SetBackImg(int img) { mBackImg = img; }
protected:
    int mBackImg;
};

