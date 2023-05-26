#pragma once
#include "CharacterActor.h"
class PSideCharacterActor :
    public CharacterActor
{

public:
    PSideCharacterActor(Game*game);
    ~PSideCharacterActor();

    void SetNum(int num) { mNum = num; }
    int GetNum() { return mNum; }
private:
    int mNum;

};

