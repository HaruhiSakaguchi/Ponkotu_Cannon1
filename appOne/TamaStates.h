#pragma once
#include "State.h"
#include "VECTOR.h"
#include "sound.h"

class TamaWait :
	public State
{
public:
	TamaWait(class StateComponent* OwnerCompo) :State(OwnerCompo) {}
	const char* GetName() const override { return "Wait"; }
	virtual void OnEnter()override;
	void Update() override;
private:
	float elapsedTime = 0;

};
class TamaMove :
	public State
{
public:
	TamaMove(class StateComponent* OwnerCompo) :State(OwnerCompo) {}
	const char* GetName() const override { return "Move"; }
	virtual void OnEnter()override;
	void Update() override;
private:
	VECTOR mAdv;
	VECTOR mTarget;
	VECTOR mTmpAngle;
	int mCnt = 0;
};
class TamaChase :
	public State
{
public:
	TamaChase(class StateComponent* OwnerCompo) :State(OwnerCompo) {}
	const char* GetName() const override { return "Chace"; }
	virtual void OnEnter()override;
	void Update() override;
private:
	VECTOR mAdv;
	VECTOR mTarget;
	VECTOR mTmpAngle;
	bool mDushFlag = true;
	int mCnt = 0;
};
class TamaAttack :
	public State
{
public:
	TamaAttack(class StateComponent* OwnerCompo) :State(OwnerCompo) {}
	const char* GetName() const override { return "Attack"; }
	void Update() override;
	virtual void OnEnter() override;
	virtual void OnExit() override;
private:
	float mTime = 0;
	float mMaxTime = 120.0f;
	VECTOR mTarget;
	VECTOR mRotation;
};

