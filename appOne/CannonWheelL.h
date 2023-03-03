#pragma once
#include "TreeMeshComponent.h"
#include "Actor.h"
#include "Cannon.h"

class CannonWheelL : public CharacterActor
{
public:
	CannonWheelL(Cannon* mOwner);
	void UpdateActor()override;
	int SetUp()override;
private:
	class Cannon* mOwner;
	class TreeMeshComponent* mTc;
};

