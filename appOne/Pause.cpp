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

	AddButton("再開する",
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
			, "プレイを再開します"
			);

	auto option = AddButton("オプション",
		[this]() {
			new Option(mGame);
		}
		, 1
			, "オプションを開きます"
			);

	option->SetState(Button::Draw_Enable);

	AddButton("タイトルに戻る",
		[this]() {
			ChangeState();
		}
		, 1
			, "タイトルに戻ります"
			);

	AddButton("ゲームを終わる",
		[this]() {
			mQuitFlag = true;
			new Quit(mGame);
		}
		, 1
			, "ゲームを終了します"
			);

	auto help = AddButton("?",
		[this]() {
			new UIHelp1(mGame);
		}
		, 2
			, "ヘルプを開きます"
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