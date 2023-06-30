#include "ItemComponent.h"
#include "window.h"
#include "Game.h"
#include "TransitionFade.h"
#include "Container.h"

ItemComponent::ItemComponent(class CharacterActor* owner)
	: Actor(owner->GetGame())
	, mOwner(owner)
{
	cData = GetGame()->GetAllData()->itemCompoData;
	class PSideCharacterActor* p = static_cast<class PSideCharacterActor*>(mOwner);
	p->AddItemComponent(this);
	cData.mNumber = (int)p->GetItemComponents().size() - 1;
}

ItemComponent::~ItemComponent()
{
}

void ItemComponent::UpdateActor()
{
	setVolume(GetGame()->GetAllData()->mCloseSound, GetGame()->GetSoundVolumeManager()->GetEffectVolume());

	Data.mTime -= delta;

	if (!mOwner || Data.mTime <= 0.0f || Data.mHp <= 0)
	{
		SetState(EDead);
	}
}

void ItemComponent::Dead()
{
	auto p = static_cast<class PSideCharacterActor*>(mOwner);

	if (Data.mMaxHp != 0 && !GetGame()->GetRenderer()->GetTransition()->outEndFlag())
	{
		setVolume(GetGame()->GetAllData()->mCloseSound, GetGame()->GetSoundVolumeManager()->GetEffectVolume());
		playSound(GetGame()->GetAllData()->mCloseSound);
	}

	if (mOwner->GetState() == CharacterActor::EActive)
	{
		p->RemoveItemComponent(this);

		for (auto item : p->GetItemComponents())
		{
			if (cData.mNumber < item->GetNumber())
			{
				item->SetNumber(item->GetNumber() - 1);
			}
		}
	}
}

