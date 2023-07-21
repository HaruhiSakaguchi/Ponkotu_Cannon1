#include "CannonWheelR.h"
#include "Game.h"

CannonWheelR::CannonWheelR(class Cannon* owner)
	: CharacterActor(owner->GetGame())
	, mOwner(owner)
	, mTc(nullptr)
{
	SetUp();
}

int CannonWheelR::SetUp()
{
	mTc = new TreeMeshComponent(this);
	mTc->SetTree("CannonWheel");

	SetNormalMesh(mTc);
	//mTc = new TreeMeshComponent(this);

	//mTc->SetTree("CannonWheelDamage");
	//SetDamageMesh(mTc);
	SetCategory(Actor::Object);
	return 0;
}

void CannonWheelR::UpdateActor()
{
	if (mOwner->GetState() == EActive)
	{
		MATRIX Master;
		Master.identity();

		Master.mulTranslate(mOwner->GetPosition() + mOwner->GetCapsulOffset());
		Master.mulRotateY(mOwner->GetRotation().y);
		Master.mulRotateY(3.141592f);

		MATRIX WheelLModel;
		WheelLModel.identity();
		WheelLModel.mulTranslate(-GetGame()->GetAllData()->cannonData.mWheelOffsetPos);

		WheelLModel.mulRotateY(-GetGame()->GetAllData()->cannonData.mWheelAngleOffset);
		WheelLModel.mulRotateZ(mOwner->GetWheelRotateZ());
		WheelLModel.mulRotateX(mOwner->GetWheelRotateX());

		WheelLModel = Master * WheelLModel;
		SetPosition(WheelLModel._14, WheelLModel._24, WheelLModel._34);
		SetRotation(VECTOR(mOwner->GetWheelRotateX(), mOwner->GetRotation().y + 3.1415926f / 2.0f, mOwner->GetWheelRotateZ()));
		SetDamageInterval(mOwner->GetDamageInterval());
	}
	else
	{
		SetState(EDead);
	}

}