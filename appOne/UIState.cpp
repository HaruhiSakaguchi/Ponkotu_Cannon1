#include "UIState.h"
#include "Game.h"
#include "TransitionFade.h"

UIState::UIState(class Game* game)
	: UIScreen(game)
{
	SetUp();
};

void UIState::Update()
{
	if (mGame->GetTransition())
	{
		if (mGame->GetTransition()->outEndFlag())
		{
			ChangeOption();
			CloseMe();
		}
	}
}

void UIState::ChangeState()
{
	mGame->GetTransition()->outTrigger();
}


