#pragma once
#include "TreeMeshComponent.h"
#include "CharacterActor.h"
#include "Cannon.h"

class CannonWheelR : public CharacterActor
{
public:
	CannonWheelR(Cannon* mOwner);
	void UpdateActor()override;
	int SetUp()override;
private:
	class Cannon* mOwner;
	class TreeMeshComponent* mTc;
};

