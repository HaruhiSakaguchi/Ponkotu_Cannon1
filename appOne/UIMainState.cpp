#include "UIMainState.h"
#include "Game.h"

UIMainState::UIMainState(Game* game)
	:UIState(game)
{
	mGame->SetCurState(this);
}
