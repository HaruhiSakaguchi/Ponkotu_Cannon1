#pragma once
#include "UIScreen.h"
#include "VECTOR.h"

class UIGenerate :
    public UIScreen
{
public:
    void draw()override;
    void Update()override;
    enum GenerateActor_Id
    {
        ECannon,
        EBarricade,
        EPower,
        EBarrier,
        ESpeed,
        ERapid,
        ERecover,
        EEmpty
    };
    UIGenerate(class UIPlayerHome*owner,Game*game, GenerateActor_Id id = EEmpty);
    ~UIGenerate();
private:
    GenerateActor_Id mId;
    VECTOR mGenePos;
    float mMouseXPerWidth;
    float mMouseYPerHeight;
    UIPlayerHome *mOwner;
    int mGenerateUsingPoints;
    class CharacterActor* mGenerateActor;
    class Button* mChangeButton;
    class Button* mCancellButton;
    int mItemGeneratePoint;
};

