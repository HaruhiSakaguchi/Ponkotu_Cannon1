#pragma once
#include "State.h"

class CannonWait :
    public State
{
public:
    CannonWait(class StateComponent* OwnerCompo) :State(OwnerCompo) {}
    const char* GetName() const override { return "Wait"; }
    void Update() override;
};
class CannonMove :
    public State
{
public:
    CannonMove(class StateComponent* OwnerCompo) :State(OwnerCompo) {}
    const char* GetName() const override { return "Move"; }
    void Update() override;
};
class CannonJump :
    public State
{
public:
    CannonJump(class StateComponent* OwnerCompo) : State(OwnerCompo) {}
    const char* GetName() const override { return "Jump"; }
    void OnEnter() override;
    void Update() override;
};
