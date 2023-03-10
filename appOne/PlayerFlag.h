#pragma once
#include "Actor.h"
class PlayerFlag :
    public Actor
{
public:
    PlayerFlag(Game* game);
    int SetUp();
    void UpdateActor();
};

