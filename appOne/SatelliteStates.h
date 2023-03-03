#include "State.h"

class SatelliteNormal :
	public State
{
public:
	SatelliteNormal(class StateComponent* OwnerCompo) :State(OwnerCompo) {}
	const char* GetName() const override { return "Normal"; }
	virtual void OnEnter()override;
	void Update() override;
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
};