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
	GetGame()->GetActorManager()->RemovePSide(this);

}

void PSideCharacterActor::Dead()
{
	if (mUI)
	{
		mUI->CloseMe();
	}

	for (auto pSide : GetGame()->GetActorManager()->GetPSide())
	{
		if (pSide->GetNum() > GetNum())
		{
			pSide->SetNum(pSide->GetNum() - 1);
		}
	}

	if (mPower)
	{
		delete mPower;
	}
	if (mSpeed)
	{
		delete mSpeed;
	}
	if (mRapid)
	{
		delete mRapid;
	}
	if (mBarrier)
	{
		delete mBarrier;
	}

}


