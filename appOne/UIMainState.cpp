#include "UIMainState.h"
#include "Game.h"

UIMainState::UIMainState(Game* game)
	: UIState(game)
	, mState(State::EEmpry)
{
	mGame->SetCurState(this);
}
