#include "Item.h"
#include "Game.h"
#include "window.h"
#include "Cannon.h"
#include "COLLISION_MAP.h"
#include "Collision_Capsul.h"
#include "TreeMeshComponent.h"
#include <sstream>
#include "UILog.h"
#include "CollisionMapComponent.h"
#include "PlayerHome.h"

Item::Item(Game* game)
	: CharacterActor(game)
	, mTc(nullptr)
	, mOwner(nullptr)
{
	GetGame()->GetActorManager()->AddItems(this);
}

Item::~Item()
{
	GetGame()->GetActorManager()->RemoveItems(this);
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
	setVolume(cData.mDropSound, GetGame()->GetSoundVolumeManager()->GetEffectVolume() + cData.mDropSoundVolumeOffset);
	playSound(cData.mDropSound);
	SetScale(VECTOR(cData.mRadius, cData.mRadius, cData.mRadius) * 4.0f);

	new CollisionMapComponent(this);
	return 0;
}

void Item::UpdateActor()
{
	for (auto cannon : GetGame()->GetActorManager()->GetCannons())
	{
		if (cannon->GetState() == Actor::EActive && Intersect(this, cannon, false))
		{
			mOwner = cannon;
			if (update() && mOwner != nullptr)
			{
				SetHp(GetHp() - 1);
			}
			if (GetHp() <= 0)
			{
				SetState(Actor::EDead);
			}
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
		GetGame()->GetActorManager()->GetStage()->GetLog()->AddText(oss.str());
	}
}
