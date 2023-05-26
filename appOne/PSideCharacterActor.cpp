#include "PSideCharacterActor.h"
#include "Game.h"

PSideCharacterActor::PSideCharacterActor(Game*game)
	:CharacterActor(game)
{
	GetGame()->AddPSide(this);
	mNum = (int)(GetGame()->GetPSide().size()) - 1;
}

PSideCharacterActor::~PSideCharacterActor()
{
	GetGame()->RemovePSide(this);
	for (auto pSide : GetGame()->GetPSide())
	{
		if (pSide->GetNum() > GetNum())
		{
			pSide->SetNum(pSide->GetNum() - 1);
		}
	}
}
