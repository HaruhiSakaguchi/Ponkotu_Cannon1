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

Title::Title(Game* game)
	:UIState(game)
{
	Data = mGame->GetAllData()->titleData;
	mTitle = Data.mTitle;
	mTitleColor = Data.mTitleColor;
	mTextSize = Data.mTextSize;
	mBackImg = Data.mBackImg;
	mBackPos = Data.mBackPos;
	mTitlePos = Data.mTitlePos;
	mButtonPos = Data.mButtonPos;

	std::ostringstream oss;
	oss << "�X�e�[�W" << int(mGame->GetPhase() + 1) << "����n�߂܂�";
	mText = oss.str();

	mGame->SetScene(Game::ETitle);
	if (mGame->GetTransition())
	{
		mGame->GetTransition()->inTrigger();
	}

	AddButton("�͂��߂���",
		[this]() {
			mGame->SetPhase(mGame->GetInitPhase());
			ChangeState();
			mGame->SetContinueFlag(true);
		}
		, 1
			, "�ŏ�����Q�[�����n�߂܂�"
			);

	AddButton("�X�e�[�W�Z���N�g",
		[this]() {
			new StageSelect(mGame);
		}
		, 1
			, "�X�e�[�W��I��Ŏn�߂܂�"
			);

	if (mGame->GetPhase() != Game::FIRST)
	{
		AddButton("��������",
			[this]() {
				ChangeState();
				mGame->SetContinueFlag(true);
			}
			, 1
				, mText.c_str()
				);
	}

	AddButton("�I�v�V����",
		[this]()
		{
			new Option(mGame);
		}
		, 1
			, "�I�v�V�������J���܂�"
			);

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

	AddButton("?",
		[this]() {
			new UIHelp1(mGame);
		}
		, 2
			, "�w���v���J���܂�"
			, ofst
			);

	playLoopSound(Data.mBgm);
	setVolume(Data.mBgm, mGame->GetVolume() + Data.mBgmSoundVolumeOffset);

}

Title::~Title()
{
	stopSound(Data.mBgm);
}

void Title::ChangeOption()
{
	if (mGame->GetScene() == Game::ETitle)
	{
		new GamePlay(mGame);
	}
}

void Title::Update()
{
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




