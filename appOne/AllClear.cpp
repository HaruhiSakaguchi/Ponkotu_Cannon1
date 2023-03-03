#include "AllClear.h"
#include "Quit.h"
#include "Title.h"
#include "Game.h"
#include "TransitionFade.h"
#include "Container.h"

AllClear::AllClear(class Game* game)
	:UIState(game)
{
	Data = mGame->GetAllData()->allClearData;

	mButtonPos = Data.mButtonPos;
	mGame->GetTransition()->inTrigger();

	mGame->SetScene(Game::EStageClear);

	AddButton("タイトルに戻る",
		[this]() {
			mGame->SetPhase(mGame->GetInitPhase());
			mGame->SetScene(Game::ETitle);
			ChangeState();
		}
	);

	AddButton("終わる",
		[this]() {
			new Quit(mGame);
		}
	);

	if (mGame->GetBgmFlag())
	{
		playLoopSound(Data.mBgm);
		setVolume(Data.mBgm, mGame->GetVolume());
	}
}

AllClear::~AllClear()
{
	stopSound(Data.mBgm);
}

void AllClear::ChangeOption()
{
	if (mGame->GetScene() == Game::ETitle)
	{
		new Title(mGame);
	}
}

void AllClear::draw()
{
	rectMode(CORNER);
	image(Data.mImg, 0.0f, 0.0f);
	rectMode(CENTER);
}

void AllClear::Update()
{
	setVolume(Data.mBgm, mGame->GetVolume());
	UIState::Update();
}