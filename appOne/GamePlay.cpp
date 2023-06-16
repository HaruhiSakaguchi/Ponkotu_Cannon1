#include "GamePlay.h"
#include "Stage1.h"
#include "Stage2.h"
#include "Stage3.h"
#include "StageClear.h"
#include "GameOver.h"
#include "input.h"
#include "Game.h"
#include "TransitionFade.h"
#include "Container.h"
#include "DebugStage.h"


GamePlay::GamePlay(Game* game) :
	UIState(game)
	, mGameOverFlag(0)
	, mGameClearFlag(0)
	, mMap(nullptr)
	, mPlayBgmFlag(false)
	, mBgm(0)
{
	Data = mGame->GetAllData()->playData;
	mGame->SetScene(Game::EPlay);

	mBgm = Data.mBgm1;
	mSoundOffset = Data.mBgm1SoundVolumeOffset;

#ifdef _DEBUG
	mMap = new DebugStage(mGame);
#endif
#ifdef _RELEASE

	if (mGame->GetPhase() == Game::THIRD)
	{
		mMap = new Stage3(mGame);
		mBgm = Data.mBgm2;
		mSoundOffset = Data.mBgm2SoundVolumeOffset;
	}
	else
	{
		if (mGame->GetPhase() == Game::FIRST)
		{
			mMap = new Stage1(mGame);
		}
		else if (mGame->GetPhase() == Game::SECOND)
		{
			mMap = new Stage2(mGame);
		}
	}
#endif

	mGame->GetTransition()->inTrigger();
	mGame->SetStage(mMap);

	setVolume(mBgm, mGame->GetVolume() + mSoundOffset);
	playLoopSound(mBgm);

}

GamePlay::~GamePlay()
{
	stopSound(mBgm);
}

void GamePlay::Update()
{
	if (mGame->GetScene() == Game::EPlay && mGameClearFlag == 0 && mGameOverFlag == 0)
	{
		if ((mGame->GetEnemies().empty() && mGame->GetWeapons().empty()) && !mGame->GetEHome())
		{
			new StageClear(mGame);
			stopSound(mBgm);
			mGameClearFlag = 1;
		}
		else if (mGame->GetPSide().empty())
		{
			new GameOver(mGame);
			stopSound(mBgm);
			mGameOverFlag = 1;

		}
	}

	if (mGame->GetBgmFlag())
	{
		setVolume(mBgm, mGame->GetVolume() + mSoundOffset);
		if (!mPlayBgmFlag)
		{
			playLoopSound(mBgm);
			mPlayBgmFlag = true;
		}
	}
	else
	{
		stopSound(mBgm);
		mPlayBgmFlag = false;
	}

	if (mGame->GetScene() == Game::ETitle)
	{
		CloseMe();
	}

	UIState::Update();

}
