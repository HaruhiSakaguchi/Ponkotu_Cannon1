#include "Pause.h"
#include "Game.h"
#include "graphic.h"
#include "input.h"
#include "Quit.h"
#include "Title.h"
#include "Option.h"
#include "UIHelp1.h"
#include "Container.h"

Pause::Pause(Game* game)
	: UIState(game)
	, mQuitFlag(false)
{
	Data = mGame->GetAllData()->pauseData;
	mGame->SetState(Game::EPaused);

	mTitle = Data.mTitle;
	mTitlePos = Data.mTitlePos;
	mButtonPos = Data.mButtonPos;

	//���������~�߂�
	if (mGame->GetCannon())
	{
		static_cast<Cannon*>(mGame->GetCannon())->StopFallSound();
	}

	AddButton("�ĊJ����",
		[this]() {
			mGame->SetState(Game::EGameplay);
			CloseMe();
		}
		, 1
			, "�v���C���ĊJ���܂�"
			);

	AddButton("�I�v�V����",
		[this]() {
			new Option(mGame);
		}
		, 1
			, "�I�v�V�������J���܂�"
			);

	AddButton("�^�C�g���ɖ߂�",
		[this]() {
			ChangeState();
		}
		, 1
			, "�^�C�g���ɖ߂�܂�"
			);

	AddButton("�Q�[�����I���",
		[this]() {
			mQuitFlag = true;
			new Quit(mGame);
		}
		, 1
			, "�Q�[�����I�����܂�"
			);

	AddButton("?",
		[this]() {
			new UIHelp1(mGame);
		}
		, 2
			, "�w���v���J���܂�"
			, Data.mHelpButtonPosOffset
			);

	setVolume(Data.mSound, mGame->GetEffectVolume() + Data.mPauseSoundVolumeOffset);
	playSound(Data.mSound);
}

Pause::~Pause()
{
	mGame->SetDisplayColor(mGame->GetAllData()->mNormalDisplayColor);
	if (mGame->GetScene() == Game::EPlay)
	{
		setVolume(Data.mSound, mGame->GetEffectVolume() + Data.mPauseSoundVolumeOffset);
		playSound(Data.mSound);
	}

	if (mGame->GetCannon() && mGame->GetCannon()->GetJumpFlag() == 1)
	{
		//pause�O�Ƀv���C���[���������Ă����特��炵�Ȃ���
		static_cast<Cannon*>(mGame->GetCannon())->PlayFallSound();
	}
}

void Pause::ProcessInput()
{
	UIScreen::ProcessInput();

	if (isTrigger(KEY_ENTER))
	{
		mGame->SetState(Game::EGameplay);
		CloseMe();
	}
}

void Pause::Draw()
{
	mGame->SetDisplayColor(mGame->GetAllData()->mPauseDisplayColor);
	UIScreen::Draw();
}

void Pause::ChangeOption()
{
	if (!mQuitFlag)
	{
		mGame->ActorClear();
		mGame->MapClear();
		new Title(mGame);
		mGame->SetState(Game::EGameplay);
	}

}