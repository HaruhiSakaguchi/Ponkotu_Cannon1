#pragma once
#include "Map.h"
class DebugStage :
    public Map
{
public:
    DebugStage(class Game* game);
    int SetUp()override;
private:
    const char* Maptext1 = "Assets\\All_Map\\BackGround\\Collision.txt";
};

