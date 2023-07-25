#include "ItemComponent.h"
#include "window.h"
#include "Game.h"
#include "TransitionFade.h"
#include "Container.h"
#include "rand.h"

ItemComponent::ItemComponent(class CharacterActor* owner)
	: CharacterActor(owner->GetGame())
	, mOwner(owner)
	, mUpTime(0.0f)
	, mMeshName(nullptr)
	, mNextParticleSpawnTime(0)
	, mBeforeParticleSpawnTime(0)
	, mIsSpawnParticle(true)
{
	cData = GetGame()->GetAllData()->itemCompoData;
	SetCategory(ActorsCategory::EObject);
	TimeReset();
}

ItemComponent::~ItemComponent()
{
}

void ItemComponent::UpdateActor()
{
	if (!mOwner || Data.mTime <= 0.0f || GetHp() <= 0)
	{
		SetState(State::EDead);
	}

	if (mOwner)
	{
		if (mUpTime < Data.mInterval)
		{
			auto end = std::chrono::system_clock::now();
			auto dur = end - mStart;
			auto msec = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
			float time = msec / 1000.0f;
			mUpTime = time;
		}

		if (!mIsSpawnParticle && (int)mUpTime == mNextParticleSpawnTime)
		{
			mIsSpawnParticle = true;
		}

		if (mIsSpawnParticle && mUpTime != mBeforeParticleSpawnTime)
		{
			int num = (int)random(1, 10);

			if (mMeshName)
			{
				mOwner->SpawnParticle(GetGame(), mOwner->GetPosition() + mOwner->GetCapsulOffset(), mMeshName, num, 0.5f);
			}

			mBeforeParticleSpawnTime = (int)mUpTime;
			mNextParticleSpawnTime += (int)random(3, 10);
			mIsSpawnParticle = false;
		}
	}

	Data.mTime = Data.mInterval - mUpTime;

}

void ItemComponent::Dead()
{
	if (Data.mMaxHp != 0 && !GetGame()->GetRenderer()->GetTransition()->outEndFlag())
	{
		setVolume(GetGame()->GetAllData()->mCloseSound, GetGame()->GetSoundVolumeManager()->GetEffectVolume());
		playSound(GetGame()->GetAllData()->mCloseSound);
	}

}


