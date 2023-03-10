#include "Item.h"
#include "Game.h"
#include "window.h"
#include "Cannon.h"
#include "COLLISION_MAP.h"
#include "Collision_Capsul.h"
#include "TreeMeshComponent.h"
#include <sstream>
#include "UILog.h"

Item::Item(Game* game)
	: CharacterActor(game)
	, mTc(nullptr)
{
	GetGame()->AddItems(this);
}

Item::~Item()
{
	GetGame()->RemoveItems(this);
}

int Item::SetUp()
{
	cData = GetGame()->GetAllData()->itemCommonData;
	SetRadius(cData.mRadius);
	SetGravity(cData.mGravity);
	SetJumpFlag(cData.mJumpFlag);
	SetJumpVel(cData.mJumpVelocity);
	SetAdvSpeed(cData.mAdvSpeed);
	SetHp(cData.mHp);
	SetCapsulOffset(cData.mCapsulOffset);
	SetCategory(Actor::Item);
	cData.mLifeTime = cData.mMaxLifeTime;
	setVolume(cData.mDropSound, GetGame()->GetEffectVolume() + cData.mDropSoundVolumeOffset);
	playSound(cData.mDropSound);
	SetScale(VECTOR(cData.mRadius, cData.mRadius,cData.mRadius) * 4.0f);

	return 0;
}

void Item::UpdateActor()
{
	if (GetGame()->GetCannon() && Intersect(this, GetGame()->GetCannon(), false))
	{
		if (update())
		{
			SetHp(GetHp() - 1);
		}
		if (GetHp() <= 0)
		{
			SetState(Actor::EDead);
		}
	}

	cData.mLifeTime -= delta;
	if (cData.mLifeTime <= cData.mMaxLifeTime * 0.3f)
	{
		if ((int)cData.mLifeTime % 2 == 0)
		{
			SetImageColor(iData.mColor);
			SetDamageInterval(0.0f);
		}
		else
		{
			SetImageColor(COLOR(iData.mColor.r, iData.mColor.g, iData.mColor.b, 128));
			SetDamageInterval(1.0f);
		}
	}

	if (cData.mLifeTime <= 0.0f || GetPosition().y < -5.0f)
	{
		SetState(Actor::EDead);
		std::ostringstream oss;
		oss << GetName() << "アイテムが消滅。";
		GetGame()->GetStage()->GetLog()->AddText(oss.str());
	}

	for (auto bullet : GetGame()->GetWeapons())
	{
		if (bullet->GetOwner() == GetGame()->GetCannon() && CollisionCircle(4, bullet->GetRadius(), GetPosition(), bullet->GetPosition()))
		{
			VECTOR vec = bullet->GetPosition() - GetPosition();
			SetPosition(GetPosition() + vec.normalize() * delta);
		}
	}

}
