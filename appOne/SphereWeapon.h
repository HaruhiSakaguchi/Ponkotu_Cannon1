#pragma once
#include "ActorsWeapon.h"

class SphereWeapon :
    public ActorsWeapon
{
public:
    SphereWeapon(class CharacterActor* owner, const VECTOR& pos = VECTOR(0, 0, 0), const VECTOR& dir = VECTOR(0, 0, 0));
    virtual void UpdateActor()override;
    virtual int SetUp(const DATA& data)override;
};

