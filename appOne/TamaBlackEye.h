#pragma once
#include "Tama.h"
#include "CharacterActor.h"
class TamaBlackEye : public CharacterActor
{
public:
	TamaBlackEye(class Tama* owner);
	~TamaBlackEye() {};
	int SetUp()override;
	void UpdateActor();
	Tama* GetOwner() { return mOwner; }
private:
	VECTOR mOffsetPos;
	Tama*mOwner;
	float mScale;
};

