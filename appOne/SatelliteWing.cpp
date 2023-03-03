#include "SatelliteWing.h"
#include "Game.h"
#include "Container.h"
#include "window.h"
#include "TreeMeshComponent.h"
#include "UILog.h"

int SatelliteWing::mNum;

SatelliteWing::SatelliteWing(class Satellite* satellite)
	:ActorsWeapon(satellite, satellite->GetPosition(), VECTOR(0, 0, 0))
	, mAttackFlag(false)
	, mTc(nullptr)
{
	Satellite* s = static_cast<Satellite*>(mOwner);

	SetUp();
	s->AddWings(this);
}

SatelliteWing::~SatelliteWing()
{
	Satellite* s = static_cast<Satellite*>(mOwner);
	s->RemoveWings(this);
}

int SatelliteWing::SetUp()
{
	Satellite* s = static_cast<Satellite*>(mOwner);

	Data = GetGame()->GetAllData()->sWingData;
	Data.mId = mNum % 4;

	mNum++;

	SetRadius(Data.mRadius);
	SetHeight(Data.mHeight);
	SetAdvSpeed(Data.mAdvSpeed);
	SetPosition(s->GetPosition());
	SetImageColor(Data.mImageColor);

	mTc = new TreeMeshComponent(this);
	if (s->GetId() == 0)
	{
		mTc->SetTree("SatelliteWing0");
		mTc->SetDamageTree("SatelliteWing0Damage");
	}
	else
	{
		mTc->SetTree("SatelliteWing1");
		mTc->SetDamageTree("SatelliteWing1Damage");
	}

	mTc->SetOffsetPos(VECTOR(0.0f, 0.0f, 0.0f));

	int hp = 0;
	if (Data.mId % 2 == 0)
	{
		hp = 2;
	}
	else
	{
		hp = 3;
	}

	SetHp(hp);
	setVolume(Data.mGurdSound, GetGame()->GetEffectVolume());
	setVolume(Data.mClashSound, GetGame()->GetEffectVolume());

	return 1;
}

void SatelliteWing::UpdateActor()
{
	Satellite* s = static_cast<Satellite*>(mOwner);

	SetDamageInterval(s->GetDamageInterval());

	float preOfsetZ = Data.mOffsetPos.z;
	float ofsetZ = 0.0f;

	if (GetDamageInterval() > 0)
	{
		ofsetZ = Data.mMaxOffsetZ;
	}
	else
	{
		ofsetZ = GetGame()->GetAllData()->sWingData.mOffsetPos.z;
	}

	Data.mOffsetPos.z = preOfsetZ + (ofsetZ - preOfsetZ) * Data.mChangeOffsetPosSpeed;

	Master.identity();

	if (GetHp() > 0 || GetOwner()->GetHp() > 0)
	{
		Master.mulTranslate(GetOwner()->GetPosition());
	}
	else
	{
		SetHp(0);
		SetJumpFlag(1);
		Master.mulTranslate(GetPosition());
	}

	Master.mulRotateY(GetRotation().y);

	Wing.identity();

	if (!mAttackFlag)
	{
		if (s->GetId() == 0)
		{
			if (Data.mId == 0)
			{
				Wing.mulTranslate(Data.mS0IdW0IdOffsetPos);
				Wing.mulRotateY(0);
				Wing.mulRotateZ(s->GetAnimAngle());
				Wing.mulScaling(Data.mWingScale2.x, Data.mWingScale2.y, Data.mWingScale2.z);
				SetRotationZ(s->GetAnimAngle());
				SetScale(VECTOR(Data.mWingScale2.x, Data.mWingScale2.y, Data.mWingScale2.z));

			}
			else if (Data.mId == 1)
			{
				Wing.mulTranslate(Data.mS0IdW1IdOffsetPos);
				Wing.mulRotateY(0);
				Wing.mulRotateX(s->GetAnimAngle());
				Wing.mulScaling(Data.mWingScale1.x, Data.mWingScale1.y, Data.mWingScale1.z);
				SetRotationX(s->GetAnimAngle());
				SetScale(VECTOR(Data.mWingScale1.x, Data.mWingScale1.y, Data.mWingScale1.z));

			}
			else if (Data.mId == 2)
			{
				Wing.mulTranslate(Data.mS0IdW2IdOffsetPos);
				Wing.mulRotateY(0);
				Wing.mulRotateX(s->GetAnimAngle());
				Wing.mulScaling(Data.mWingScale1.x, Data.mWingScale1.y, Data.mWingScale1.z);
				SetRotationX(s->GetAnimAngle());
				SetScale(VECTOR(Data.mWingScale1.x, Data.mWingScale1.y, Data.mWingScale1.z));
			}
			else if (Data.mId == 3)
			{
				Wing.mulTranslate(Data.mS0IdW3IdOffsetPos);
				Wing.mulRotateY(0);
				Wing.mulRotateZ(-s->GetAnimAngle());
				Wing.mulScaling(Data.mWingScale2.x, Data.mWingScale2.y, Data.mWingScale2.z);
				SetRotationZ(-s->GetAnimAngle());
				SetScale(VECTOR(Data.mWingScale2.x, Data.mWingScale2.y, Data.mWingScale2.z));

			}
		}
		else
		{
			Wing.mulRotateY(s->GetRotation().y + (Data.mId) * (3.1415926f / 2));
			Wing.mulTranslate(Data.mS1IdOffsetPos);
		}
	}
	else
	{
		if (s->GetId() == 0)
		{
			Wing.mulRotateY(s->GetRotation().y + (Data.mId) * (3.1415926f / 2));
			Wing.mulTranslate(Data.mS0IdAttackOffsetPos);
		}
		else
		{
			Wing.mulRotateY((Data.mId) * (3.1415926f / 2));
			if (Data.mId % 2 == 0)
			{
				Wing.mulTranslate(cosf(s->GetAnimAngle() * 10.0f), cosf(s->GetAnimAngle() * 10.0f), cosf(s->GetAnimAngle() * 10.0f - 1.0f) + sinf(s->GetAnimAngle() * 10.0f));
			}
			else
			{
				Wing.mulTranslate(-cosf(s->GetAnimAngle() * 10.0f), cosf(s->GetAnimAngle() * 10.0f), cosf(s->GetAnimAngle() * 10.0f - 1.0f) + sinf(s->GetAnimAngle() * 10.0f));
			}
			Wing.mulRotateX(GetRotation().y + 0.17f);
		}
	}

	if (s->GetId() == 0)
	{
		SetRotationY(s->GetRotation().y);
	}

	Wing.mulTranslate(Data.mOffsetPos);

	Wing = Master * Wing;

	float posY = GetPosition().y;
	if (GetJumpFlag() == 1)
	{
		posY += GetJumpVel() * delta;
		SetJumpVel(GetJumpVel() + GetGravity() * delta);
	}

	SetPosition(GetPosition().x, posY, GetPosition().z);

	VECTOR prePos = GetPosition();
	VECTOR pos = VECTOR(Wing._14, Wing._24, Wing._34);

	if (GetHp() <= 0)
	{
		SetPosition(prePos + (pos.y - prePos.y) * Data.mChangePosSpeed);
		if (GetPosition().y < 0.0f)
		{
			SetState(Actor::EDead);
			GetGame()->GetStage()->GetLog()->AddText("Wing‚ð”j‰óB");
		}
	}
	else
	{
		SetPosition(pos);
	}

}

void SatelliteWing::damage()
{
	SetHp(GetHp() - 1);
	if (GetHp() > 0)
	{
		setVolume(Data.mGurdSound, GetGame()->GetEffectVolume());
		playSound(Data.mGurdSound);
	}
	else
	{
		setVolume(Data.mClashSound, GetGame()->GetEffectVolume());
		playSound(Data.mClashSound);
		SetGravity(Data.mGravity);
		SetJumpFlag(1);
	}
}