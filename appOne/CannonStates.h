#pragma once
#include "State.h"
#include "VECTOR.h"

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
class CannonLaunch :
    public State
{
public:
    CannonLaunch(class StateComponent*OwnerCompo) : State(OwnerCompo){}
    const char* GetName() const override { return "Launch"; }
    void OnEnter() override;
    void Update()override;
    void OnExit()override;
private:
    VECTOR mTarget;
    VECTOR mAdv;
};

class CannonRotate :
    public State
{
public:
    CannonRotate(class StateComponent*OwnerCompo) : State(OwnerCompo){}
    const char* GetName() const override { return "Rotate"; }
    void OnEnter()override;
    void Update() override;
    void OnExit() override;
private:
    VECTOR mTarget;
    VECTOR mAdv;
};
