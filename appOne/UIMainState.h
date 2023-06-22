#pragma once
#include "UIState.h"

class UIMainState :
    public UIState
{

public:
    UIMainState(class Game* game);
    enum class State : int
    {
        ETitle,
        EGamePlay,
        EStageClear,
        EGameOver,
        EAllClear,
        EQuit,
        EEmpry
    };

    State GetState() { return mState; }

protected:
    State mState;
};

