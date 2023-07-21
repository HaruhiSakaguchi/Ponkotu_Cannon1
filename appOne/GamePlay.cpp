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
	UIMainState(game)
	, mGameOverFlag(0)
	, mGameClearFlag(0)
	, mMap(nullptr)
	, mPlayBgmFlag(false)
	, mBgm(0)
{
	Data = mGame->GetAllData()->playData;

	mBgm = Data.mBgm1;
	mSoundOffset = Data.mBgm1SoundVolumeOffset;
	mState = State::EGamePlay;


#ifdef _DEBUG
	mMap = new DebugStage(mGame);
	mGame->GetActorManager()->SetStage(mMap);
	mMap->SetUp();
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

	mGame->GetRenderer()->GetTransition()->inTrigger();

	setVolume(mBgm, mGame->GetSoundVolumeManager()->GetVolume() + mSoundOffset);
	playLoopSound(mBgm);

}

GamePlay::~GamePlay()
{
	stopSound(mBgm);
}

void GamePlay::Update()
{
	if (mGameClearFlag == 0 && mGameOverFlag == 0)
	{
		if ((mGame->GetActorManager()->GetEnemies().empty() && mGame->GetActorManager()->GetWeapons().empty()) && !mGame->GetActorManager()->GetEHome())
		{
			new StageClear(mGame);
			stopSound(mBgm);
			mGameClearFlag = 1;
			CloseMe();
		}
		else if (mGame->GetActorManager()->GetPSide().empty())
		{
			new GameOver(mGame);
			stopSound(mBgm);
			mGameOverFlag = 1;
			CloseMe();
		}
	}

	if (mGame->GetSoundVolumeManager()->GetBgmFlag())
	{
		setVolume(mBgm, mGame->GetSoundVolumeManager()->GetVolume() + mSoundOffset);
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


	UIState::Update();

}
