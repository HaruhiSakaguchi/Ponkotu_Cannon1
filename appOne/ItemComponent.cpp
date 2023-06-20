#include "ItemComponent.h"
#include "window.h"
#include "Game.h"
#include "TransitionFade.h"
#include "Container.h"

ItemComponent::ItemComponent(class Actor* cannon)
	: Component(cannon)
{
	cData = mOwner->GetGame()->GetAllData()->itemCompoData;
	class Cannon* c = static_cast<class Cannon*>(mOwner);
	c->AddItemComponent(this);
	cData.mNumber = (int)c->GetItemComponents().size() - 1;
	cData.mUIPosY = cData.mUIOffsetPosY * cData.mNumber;
	cData.mPosition = cData.mUIInitPos + VECTOR2(0.0f,cData.mUIPosY);
}

ItemComponent::~ItemComponent()
{
	class Cannon* c = static_cast<class Cannon*>(mOwner);

	if (Data.mMaxHp != 0 && !mOwner->GetGame()->GetTransition()->outEndFlag())
	{
		setVolume(mOwner->GetGame()->GetAllData()->mCloseSound, mOwner->GetGame()->GetSoundVolumeManager()->GetEffectVolume());
		playSound(mOwner->GetGame()->GetAllData()->mCloseSound);
	}

	c->RemoveItemComponent(this);

	for (auto item : c->GetItemComponents())
	{
		if (cData.mNumber < item->GetNumber())
		{
			item->SetNumber(item->GetNumber() - 1);
		}
	}
}

void ItemComponent::Update()
{
	setVolume(mOwner->GetGame()->GetAllData()->mCloseSound, mOwner->GetGame()->GetSoundVolumeManager()->GetEffectVolume());

	Data.mTime -= delta;

	if (cData.mPosition.x > cData.mUIMinPosX)
	{
		cData.mPosition.x += cData.mUIPosAdvSpeed;
	}

	cData.mUIPosY = cData.mUIOffsetPosY * cData.mNumber;
	cData.mPosition.y = cData.mUIInitPos.y + cData.mUIPosY;

	VECTOR2 prePos = cData.mPosition;
	VECTOR2 pos = prePos + (cData.mPosition - prePos) * cData.mUIPosChangeSpeed;
	cData.mPosition = pos;

	if (Data.mTime <= 0.0f || Data.mHp <= 0)
	{
		delete this;
	}
}

