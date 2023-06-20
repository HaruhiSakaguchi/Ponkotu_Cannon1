#include "Title.h"
#include "Game.h"
#include "input.h"
#include <sstream>
#include <cstdio>
#include "sound.h" 
#include "Container.h"
#include "TransitionFade.h"
#include "Quit.h"
#include "Option.h"
#include "StageSelect.h"
#include "GamePlay.h"
#define D

Title::Title(Game* game)
	:UIMainState(game)
	, mChangeStateFlag(false)
	,mIsChangePlay(false)
{
	Data = mGame->GetAllData()->titleData;
	mTitle = Data.mTitle;
	mTitleColor = Data.mTitleColor;
	mTextSize = Data.mTextSize;
	mBackImg = Data.mBackImg;
	mBackPos = Data.mBackPos;
	mTitlePos = Data.mTitlePos;
	mButtonPos = Data.mButtonPos;
	mState = State::ETitle;
	mGame->SetCurState(this);


	std::ostringstream oss;
	oss << "ステージ" << int(mGame->GetPhase() + 1) << "から始めます";
	mText = oss.str();

//#ifdef RELEASE
	if (mGame->GetTransition())
	{
		mGame->GetTransition()->inTrigger();
	}


	AddButton("はじめから",
		[this]() {
			mGame->SetPhase(mGame->GetInitPhase());
			ChangeState();
			mGame->SetContinueFlag(true);
			mIsChangePlay = true;
		}
		, 1
			, "最初からゲームを始めます"
			);

	auto select = AddButton("ステージセレクト",
		[this]() {
			new StageSelect(mGame);
		}
		, 1
			, "ステージを選んで始めます"
			);

	select->SetState(Button::Draw_Enable);

	if (mGame->GetPhase() != Game::FIRST)
	{
		auto Continue = AddButton("続きから",
			[this]() {
				ChangeState();
				mGame->SetContinueFlag(true);
				mIsChangePlay = true;
			}
			, 1
				, mText.c_str()
				);

		Continue->SetState(Button::Draw_Enable);
	}

	auto option = AddButton("オプション",
		[this]()
		{
			new Option(mGame);
		}
		, 1
			, "オプションを開きます"
			);

	option->SetState(Button::Draw_Enable);

	AddButton("終わる",
		[this]() {
			new Quit(mGame);
		}
		, 1
			, "ゲームを終わります"
			);

	VECTOR2 ofst;
	if (mGame->GetPhase() != Game::FIRST)
	{
		ofst = Data.mPhaseNotFirstButtonOffsetPos;
	}
	else
	{
		ofst = Data.mPhaseFirstButtonOffsetPos;
	}

	auto help = AddButton("?",
		[this]() {
			new UIHelp1(mGame);
		}
		, 2
			, "ヘルプを開きます"
			, ofst
			);

	help->SetState(Button::Draw_Enable);

	playLoopSound(Data.mBgm);
	setVolume(Data.mBgm, mGame->GetVolume() + Data.mBgmSoundVolumeOffset);

//#endif
}

Title::~Title()
{
	stopSound(Data.mBgm);
}

void Title::ChangeOption()
{
	if (mIsChangePlay)
	{
		new GamePlay(mGame);
	}
}

void Title::Update()
{
//#ifdef _DEBUG
	if (!mChangeStateFlag)
	{
		mChangeStateFlag = true;
		ChangeState();
	}
//#endif

	if (mGame->GetBgmFlag())
	{
		setVolume(Data.mBgm, mGame->GetVolume() + Data.mBgmSoundVolumeOffset);
		if (!mPlayBgmFlag)
		{
			playLoopSound(Data.mBgm);
			mPlayBgmFlag = true;
		}
	}
	else
	{
		stopSound(Data.mBgm);
		mPlayBgmFlag = false;
	}


	UIState::Update();

}




