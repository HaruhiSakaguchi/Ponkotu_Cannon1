#pragma once
#include "CharacterActor.h"
#include "UIPSideCharacterStatusBase.h"

class PSideCharacterActor :
    public CharacterActor
{

public:
    PSideCharacterActor(class Game*game);
    ~PSideCharacterActor();
    void SetUI(UIPSideCharacterStatusBase* ui) { mUI = ui; }

    void SetNum(int num) { mNum = num; }
    int GetNum() { return mNum; }
private:
    int mNum;
    UIPSideCharacterStatusBase* mUI;
};

