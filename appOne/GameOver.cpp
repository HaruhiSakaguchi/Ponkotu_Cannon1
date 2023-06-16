#include "GameOver.h"
#include "Title.h"
#include "Quit.h"
#include <vector>
#include "GamePlay.h"
#include "Reconfirm.h"
#include "Game.h"
#include "TransitionFade.h"
#include "Container.h"

GameOver::GameOver(Game* game)
	: UIState(game)
	, mTime(0.0f)
	, mPlayFlag(false)
	, mTitleFlag(false)
{
	Data = mGame->GetAllData()->overData;

	mTitle = Data.mTitle;
	mTitleColor = Data.mTitleColor;
	mTextSize = Data.mTitleTextSize;
	mTitlePos = Data.mTitlePos;
	mButtonPos = Data.mButtonPos;

	mGame->SetScene(Game::EGameOver);
	mGame->GetTransition()->inTrigger();

	mPlayFlag = false;
	mTitleFlag = false;

	AddButton("タイトルに戻る",
		[this]() {
			mTitleFlag = true;
			ChangeState();
		}
		, 1
			, "タイトルに戻ります"
			);

	AddButton("リトライ",
		[this]() {
			mPlayFlag = true;
			ChangeState();
		}
		, 1
			, "もう一度ステージをやり直します"
			, VECTOR2(0.0f, 0.0f)
			, Data.mRetryButtonPosOffset
			);

	AddButton("ゲームを終わる",
		[this]() {
			new Quit(mGame);
		}
		, 1
			, "ゲームを終わります"
			);

	if (mGame->GetBgmFlag())
	{
		playSound(Data.mSound);
		setVolume(Data.mSound, mGame->GetVolume());
	}
}

void GameOver::Update()
{
	setVolume(Data.mSound, mGame->GetVolume());
	mTime += Data.mTitlePosAdvSpeed;
	VECTOR2 prePos = mTitlePos;
	VECTOR2 pos = VECTOR2(mTitlePos.x, mTitlePos.y + sinf(mTime) * Data.mMaxOffsetPosY);
	mTitlePos = prePos + (pos - prePos) * Data.mTitlePosAdvSpeed;
	UIState::Update();
}

void GameOver::ChangeOption()
{
	if (!mTitleFlag && mPlayFlag)
	{
		mGame->ActorClear();
		mGame->MapClear();
		new GamePlay(mGame);
		mGame->SetState(Game::EGameplay);
		mGame->SetScene(Game::EPlay);
	}
	else if (!mPlayFlag && mTitleFlag)
	{
		mGame->ActorClear();
		mGame->MapClear();
		new Title(mGame);
		mGame->SetState(Game::EGameplay);
		mGame->SetScene(Game::ETitle);
	}
}


