#pragma once
#include "UIScreen.h"
#include "VECTOR.h"

class UIGenerate :
    public UIScreen
{
public:
    void draw()override;
    void Update()override;
    void CreateItemButtons();
    void CreateGenerateActorLvUpandDownButtons();
    void CreateChangeItemGenerateButtons();
    enum class GenerateActor_Id : int
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
    UIGenerate(class UIPlayerHome*owner,Game*game, GenerateActor_Id id = GenerateActor_Id::EEmpty);
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
    class Button* mGenerateActorLvPlusButton;
    class Button* mGenerateActorLvMinusButton;
    class Button* mLvMaxButton;
    class Button* mLvResetButton;
    class MeshComponent* mMesh;

    VECTOR2 mLvTUButtonPosOffset;
    int mItemGeneratePoint;
    static int mGenerateActorLevel;
    const char* mItemNames[5]
    {
        "Power",
        "Barrier",
        "Speed",
        "Rapid",
        "Recover"
    };
};

