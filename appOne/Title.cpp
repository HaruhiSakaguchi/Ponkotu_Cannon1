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
	oss << "�X�e�[�W" << int(mGame->GetPhase() + 1) << "����n�߂܂�";
	mText = oss.str();

//#ifdef RELEASE
	if (mGame->GetTransition())
	{
		mGame->GetTransition()->inTrigger();
	}


	AddButton("�͂��߂���",
		[this]() {
			mGame->SetPhase(mGame->GetInitPhase());
			ChangeState();
			mGame->SetContinueFlag(true);
			mIsChangePlay = true;
		}
		, 1
			, "�ŏ�����Q�[�����n�߂܂�"
			);

	auto select = AddButton("�X�e�[�W�Z���N�g",
		[this]() {
			new StageSelect(mGame);
		}
		, 1
			, "�X�e�[�W��I��Ŏn�߂܂�"
			);

	select->SetState(Button::Draw_Enable);

	if (mGame->GetPhase() != Game::FIRST)
	{
		auto Continue = AddButton("��������",
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

	auto option = AddButton("�I�v�V����",
		[this]()
		{
			new Option(mGame);
		}
		, 1
			, "�I�v�V�������J���܂�"
			);

	option->SetState(Button::Draw_Enable);

	AddButton("�I���",
		[this]() {
			new Quit(mGame);
		}
		, 1
			, "�Q�[�����I���܂�"
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
			, "�w���v���J���܂�"
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




