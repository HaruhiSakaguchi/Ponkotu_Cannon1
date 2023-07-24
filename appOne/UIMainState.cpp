#include "UIMainState.h"
#include "Game.h"

UIMainState::UIMainState(Game* game)
	: UIState(game)
	, mState(State::EEmpty)
{
	mGame->SetCurState(this);
}
