#include "PSideCharacterActor.h"
#include "Game.h"

PSideCharacterActor::PSideCharacterActor(class Game* game)
	: CharacterActor(game)
	, mUI(nullptr)
	, mNum(0)
	, mPower(nullptr)
	, mSpeed(nullptr)
	, mBarrier(nullptr)
	, mRapid(nullptr)
{
	GetGame()->GetActorManager()->AddPSide(this);
	mNum = (int)(GetGame()->GetActorManager()->GetPSide().size()) - 1;
}

PSideCharacterActor::~PSideCharacterActor()
{
	if (mUI)
	{
		mUI->CloseMe();
	}

	GetGame()->GetActorManager()->RemovePSide(this);

	for (auto pSide : GetGame()->GetActorManager()->GetPSide())
	{
		if (pSide->GetNum() > GetNum())
		{
			pSide->SetNum(pSide->GetNum() - 1);
		}
	}
}

//void PSideCharacterActor::AddItemComponent(class ItemComponent* component)
//{
//	mItemComponents.emplace_back(component);
//}
//
//void PSideCharacterActor::RemoveItemComponent(class ItemComponent* component)
//{
//	auto iter = std::find(mItemComponents.begin(), mItemComponents.end(), component);
//	if (iter != mItemComponents.end())
//	{
//		//このActorとケツのActorを入れ替える(消去後コピー処理を避けるため)
//		std::iter_swap(iter, mItemComponents.end() - 1);
//		mItemComponents.pop_back();
//	}
//}