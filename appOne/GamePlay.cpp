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

	mGame->SetStage(mMap);
	mGame->GetTransition()->inTrigger();

	setVolume(mBgm, mGame->GetVolume() + mSoundOffset);
	playLoopSound(mBgm);

}

GamePlay::~GamePlay()
{
	stopSound(mBgm);
}

void GamePlay::Update()
{
	if (!mGame->GetEHome() && mGameClearFlag == 0)
	{
		if (mGame->GetScene() == Game::EPlay && (mGame->GetEnemies().empty() && mGame->GetWeapons().empty()))
		{
			bool jumpflag = false;
			/*for (auto actor : mGame->GetActors())
			{
				CharacterActor*chara = static_cast<CharacterActor*>(actor);
				if (chara->GetJumpFlag() == 1)
				{
					jumpflag = true;
				}
			}*/
			if (!jumpflag)
			{
				new StageClear(mGame);
				if (mGame->GetPhase() == Game::FOURTH)
				{
					stopSound(mBgm);
				}
				mGameClearFlag = 1;
			}
		}
	}
	else {
		if (!mGame->GetCannon() && (mGameOverFlag == 0 && mGame->GetScene() == Game::EPlay))
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
