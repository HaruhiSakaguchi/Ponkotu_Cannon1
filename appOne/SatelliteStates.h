#include "State.h"
#include "VECTOR.h"

class SatelliteNormal :
	public State
{
public:
	SatelliteNormal(class StateComponent* OwnerCompo) :State(OwnerCompo) {}
	const char* GetName() const override { return "Normal"; }
	virtual void OnEnter()override;
	void Update() override;
};
class SatelliteMove :
	public State
{
public:
	SatelliteMove(class StateComponent* OwnerCompo) :State(OwnerCompo) {}
	const char* GetName() const override { return "Move"; }
	virtual void OnEnter()override;
	void Update() override;
private:
	VECTOR mTarget;
	VECTOR mAdv;
	int mCnt;
};
class SatelliteRockOn :
	public State
{
public:
	SatelliteRockOn(class StateComponent* OwnerCompo) : State(OwnerCompo) {}
	const char* GetName() const override { return "RockOn"; }
	virtual void OnEnter();
	void Update() override;
private:
	VECTOR mTarget;
	VECTOR mAdv;
};
class SatelliteAttack :
	public State
{
public:
	SatelliteAttack(class StateComponent* OwnerCompo) :State(OwnerCompo) {}
	const char* GetName() const override { return "Attack"; }
	virtual void OnEnter()override;
	virtual void OnExit()override;
	void Update() override;
private:
	VECTOR mTarget;
	VECTOR mAdv;
	int mCnt;
	int mMaxCnt;
	int mRotateCnt;
};