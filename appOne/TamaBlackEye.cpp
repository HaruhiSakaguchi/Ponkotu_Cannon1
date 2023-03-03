#include "TamaBlackEye.h"
#include "TreeMeshComponent.h"
#include "Game.h"

TamaBlackEye::TamaBlackEye(class Tama* owner)
	: CharacterActor(owner->GetGame())
	, mOwner(owner)
	, mOffsetPos(0.0f, 0.0f, 0.0f)
	, mScale(1.0f)
{
	SetPosition(owner->GetPosition());
	SetUp();
}

int TamaBlackEye::SetUp()
{
	TreeMeshComponent* tc = new TreeMeshComponent(this);
	mOffsetPos = GetGame()->GetAllData()->tamaData.mBlackEyeOffsetPos;
	tc->SetTree("TamaBlackEye");
	tc->SetDamageTree("TamaBlackEyeDamage");
	SetCategory(Actor::Object);
	return 0;
}

void TamaBlackEye::UpdateActor()
{
	if (GetOwner()->GetHp() > 0 && GetScale().x > 0.0f)
	{
		MATRIX Master;
		Master.identity();
		Master.mulTranslate(mOwner->GetPosition() + mOwner->GetCapsulOffset());
		Master.mulScaling(mOwner->GetScale());
		Master.mulRotateY(mOwner->GetRotation().y);

		MATRIX BlackEyeModel;
		BlackEyeModel.identity();
		BlackEyeModel.mulTranslate(mOffsetPos);
		BlackEyeModel.mulScaling(mOwner->GetRadius(), mOwner->GetRadius(), GetScale().z);

		BlackEyeModel = Master * BlackEyeModel;

		SetPosition(BlackEyeModel._14, BlackEyeModel._24, BlackEyeModel._34);
		SetRotationY(mOwner->GetRotation().y);
		SetDamageInterval(mOwner->GetDamageInterval());
		SetScale(VECTOR(mOwner->GetRadius(), mOwner->GetRadius(), mScale) * mOwner->GetScale().x);

		float preEyeOfsetZ = mOffsetPos.z;
		float preEyeScale = mScale;
		float ofsetZ = 0.0f;
		float eyeScale = 0.0f;

		if (GetDamageInterval() > 0.0f)
		{
			ofsetZ = GetGame()->GetAllData()->tamaData.mMaxOffsetZ;
			eyeScale = GetGame()->GetAllData()->tamaData.mMaxScale;
		}
		else
		{
			ofsetZ = GetGame()->GetAllData()->tamaData.mBlackEyeOffsetPos.z;
			eyeScale = GetGame()->GetAllData()->tamaData.mBlackEyeScale;
		}

		mOffsetPos.z = preEyeOfsetZ + (ofsetZ - preEyeOfsetZ) * GetGame()->GetAllData()->tamaData.mChangeScaleSpeed;
		mScale = preEyeScale + (eyeScale - preEyeScale) * GetGame()->GetAllData()->tamaData.mChangeScaleSpeed;
	}
	else
	{
		SetState(EDead);
	}
}