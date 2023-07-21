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
