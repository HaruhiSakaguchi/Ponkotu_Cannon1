#include "ItemComponent.h"
#include "window.h"
#include "Game.h"
#include "TransitionFade.h"
#include "Container.h"
#include "rand.h"

ItemComponent::ItemComponent(class CharacterActor* owner)
	: Actor(owner->GetGame())
	, mOwner(owner)
	, mUpTime(0.0f)
	, mMeshName(nullptr)
	, mNextParticleSpawnTime(0)
	, mBeforeParticleSpawnTime(0)
	, mIsSpawnParticle(true)
{
	cData = GetGame()->GetAllData()->itemCompoData;
	class PSideCharacterActor* p = static_cast<class PSideCharacterActor*>(mOwner);
	p->AddItemComponent(this);
	cData.mNumber = (int)p->GetItemComponents().size() - 1;
	TimeReset();
}

ItemComponent::~ItemComponent()
{
}

void ItemComponent::UpdateActor()
{
	if (!mOwner || Data.mTime <= 0.0f || Data.mHp <= 0)
	{
		SetState(EDead);
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
	auto p = static_cast<class PSideCharacterActor*>(mOwner);

	if (Data.mMaxHp != 0 && !GetGame()->GetRenderer()->GetTransition()->outEndFlag())
	{
		setVolume(GetGame()->GetAllData()->mCloseSound, GetGame()->GetSoundVolumeManager()->GetEffectVolume());
		playSound(GetGame()->GetAllData()->mCloseSound);
	}

	if (p->GetState() == CharacterActor::EActive)
	{
		p->RemoveItemComponent(this);

		/*for (auto item : p->GetItemComponents())
		{
			if (cData.mNumber < item->GetNumber())
			{
				item->SetNumber(item->GetNumber() - 1);
			}
		}*/
	}
}


