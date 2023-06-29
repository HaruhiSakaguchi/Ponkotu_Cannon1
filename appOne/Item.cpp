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
	, mTime(0.0f)
	, mIsSpawnParticle(false)
	, mBeforeTime(0)
{
	GetGame()->GetActorManager()->AddItems(this);
	mStart = std::chrono::system_clock::now();

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
	if (mTime < cData.mMaxLifeTime)
	{
		auto end = std::chrono::system_clock::now();
		auto dur = end - mStart;
		auto msec = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
		float time = msec / 1000.0f;
		mTime = time;
	}

	if ((int)mTime % 3 == 0 && (int)mTime != mBeforeTime)
	{
		mIsSpawnParticle = true;
	}

	if (mIsSpawnParticle)
	{
		SpawnParticle(GetPosition(), mNormalMesh->GetMeshName(), 5, 1.0f, Particle::MeshType::ETree);
		mIsSpawnParticle = false;
		mBeforeTime = (int)mTime;
	}

	for (auto pSide : GetGame()->GetActorManager()->GetCannons())
	{
		if (pSide->GetState() == Actor::EActive && Intersect(this, pSide, false))
		{
			mOwner = pSide;
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

	if (mTime > cData.mMaxLifeTime * 0.3f)
	{
		if ((int)mTime % 2 == 0)
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

	if (mTime >= cData.mMaxLifeTime || GetPosition().y < -5.0f)
	{
		SetState(Actor::EDead);
		std::ostringstream oss;
		oss << GetName() << "アイテムが消滅。";
		GetGame()->GetActorManager()->GetStage()->GetLog()->AddText(oss.str());
	}
}
