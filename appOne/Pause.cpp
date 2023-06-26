#include "Pause.h"
#include "Game.h"
#include "graphic.h"
#include "input.h"
#include "Quit.h"
#include "Title.h"
#include "Option.h"
#include "UIHelp1.h"
#include "Container.h"
#include "UIMainState.h"

Pause::Pause(Game* game)
	: UIState(game)
	, mQuitFlag(false)
{
	Data = mGame->GetAllData()->pauseData;
	mGame->SetState(Game::EPaused);

	mTitle = Data.mTitle;
	mTitlePos = Data.mTitlePos;
	mButtonPos = Data.mButtonPos;

	AddButton("�ĊJ����",
		[this]() {
			mGame->SetState(Game::EGameplay);
			CloseMe();
			for (auto ui : mGame->GetUIManager()->GetUIStack())
			{
				if (ui != this)
				{
					for (auto button : ui->GetButtons())
					{
						button->SetState(Button::Enable);
					}
				}
			}
		}
		, 1
			, "�v���C���ĊJ���܂�"
			);

	auto option = AddButton("�I�v�V����",
		[this]() {
			new Option(mGame);
		}
		, 1
			, "�I�v�V�������J���܂�"
			);

	option->SetState(Button::Draw_Enable);

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

	auto help = AddButton("?",
		[this]() {
			new UIHelp1(mGame);
		}
		, 2
			, "�w���v���J���܂�"
			, Data.mHelpButtonPosOffset
			);

	help->SetState(Button::Draw_Enable);

	setVolume(Data.mSound, mGame->GetSoundVolumeManager()->GetEffectVolume() + Data.mPauseSoundVolumeOffset);
	playSound(Data.mSound);

	for (auto ui : mGame->GetUIManager()->GetUIStack())
	{
		if (ui != this)
		{
			for (auto button : ui->GetButtons())
			{
				button->SetState(Button::Disable);
			}
		}
	}
}

Pause::~Pause()
{
	mGame->GetRenderer()->SetDisplayColor(mGame->GetAllData()->mNormalDisplayColor);
	if (mGame->GetCurState()->GetState() == UIMainState::State::EGamePlay)
	{
		setVolume(Data.mSound, mGame->GetSoundVolumeManager()->GetEffectVolume() + Data.mPauseSoundVolumeOffset);
		playSound(Data.mSound);
	}

}

void Pause::ProcessInput()
{
	UIScreen::ProcessInput();

	if (isTrigger(KEY_ENTER))
	{
		mGame->SetState(Game::EGameplay);
		CloseMe();
		for (auto ui : mGame->GetUIManager()->GetUIStack())
		{
			if (ui != this)
			{
				for (auto button : ui->GetButtons())
				{
					button->SetState(Button::Enable);
				}
			}
		}
	}
}

void Pause::Draw()
{
	mGame->GetRenderer()->SetDisplayColor(mGame->GetAllData()->mPauseDisplayColor);
	mGame->GetRenderer()->DrawDisplay();
	UIScreen::Draw();
}

void Pause::ChangeOption()
{
	if (!mQuitFlag)
	{
		mGame->GetActorManager()->ActorClear();
		new Title(mGame);
		mGame->SetState(Game::EGameplay);
	}

}