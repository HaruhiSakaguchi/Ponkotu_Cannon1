#include "Quit.h"
#include "Game.h"

Quit::Quit(Game* game)
	: UIMainState(game)
{
	Data = mGame->GetAllData()->quitData;

	mBackPos = Data.mBackPos;
	mTitlePos = Data.mTitlePos;
	mButtonPos = Data.mButtonPos;
	mBackImg = mGame->GetAllData()->mDialog2;
	mTitle = Data.mTitle;
	mTitleColor = Data.mTitleColor;
	mTextSize = Data.mTextSize;

	AddButton("‚Í‚¢",
		[this]() {
			mState = State::EQuit;
			ChangeState();
		}
	);

	AddButton("‚¢‚¢‚¦",
		[this]() {
			CloseMe();
		}
	);
}

void Quit::ChangeOption()
{
	mGame->SetState(Game::GameState::EQuit);
}


