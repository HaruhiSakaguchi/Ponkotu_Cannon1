#include "Quit.h"
#include "Game.h"

Quit::Quit(Game* game)
	:UIState(game)
{
	Data = mGame->GetAllData()->quitData;

	mBackPos = Data.mBackPos;
	mTitlePos = Data.mTitlePos;
	mButtonPos = Data.mButtonPos;
	mBackImg = mGame->GetAllData()->mDialog2;
	mTitle = Data.mTitle;
	mTitleColor = Data.mTitleColor;
	mTextSize = Data.mTextSize;

	mGame->SetTmpScene(mGame->GetScene());
	mGame->SetScene(Game::EGameQuit);

	AddButton("‚Í‚¢",
		[this]() {
			ChangeState();
		}
	);

	AddButton("‚¢‚¢‚¦",
		[this]() {
			mGame->SetScene(mGame->GetTmpScene());
	        CloseMe();
		}
	);
}

void Quit::ChangeOption()
{
	mGame->SetState(Game::EQuit);
}


