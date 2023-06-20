#include "AllClear.h"
#include "Quit.h"
#include "Title.h"
#include "Game.h"
#include "TransitionFade.h"
#include "Container.h"

AllClear::AllClear(class Game* game)
	:UIMainState(game)
	,mIsChangeTitle(false)
{
	Data = mGame->GetAllData()->allClearData;

	mButtonPos = Data.mButtonPos;
	mGame->GetTransition()->inTrigger();
	mState = State::EAllClear;

	AddButton("タイトルに戻る",
		[this]() {
			mGame->SetPhase(mGame->GetInitPhase());
			mIsChangeTitle = true;
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
	if (mIsChangeTitle)
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