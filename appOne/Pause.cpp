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

	//落下音を止める
	if (mGame->GetCannon())
	{
		static_cast<Cannon*>(mGame->GetCannon())->StopFallSound();
	}

	AddButton("再開する",
		[this]() {
			mGame->SetState(Game::EGameplay);
			CloseMe();
		}
		, 1
			, "プレイを再開します"
			);

	AddButton("オプション",
		[this]() {
			new Option(mGame);
		}
		, 1
			, "オプションを開きます"
			);

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

	AddButton("?",
		[this]() {
			new UIHelp1(mGame);
		}
		, 2
			, "ヘルプを開きます"
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
		//pause前にプレイヤーが落下していたら音を鳴らしなおす
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