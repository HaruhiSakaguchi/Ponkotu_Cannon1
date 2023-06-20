#include "Game.h"
#include "framework.h"
#include "window.h"
#include "graphic.h"
#include "input.h"

#include "Renderer.h"
#include "TransitionFade.h"
#include "Title.h"
#include "Pause.h"
#include "Actor.h"
#include "Camera.h"
#include "COLLISION_MAP.h"
#include "Map.h"
#include "PlayerHome.h"
#include "EnemyHome.h"

Game::Game()
	: mGameState(EGameplay)
	, mPhase(FIRST)
	, mInitPhase(mPhase)
	, mContinueFlag(false)
	, mRenderer(nullptr)
	, mTransition(nullptr)
	, mDisplayColor(0, 0, 0, 0)
	, mMap(nullptr)
	, mStage(nullptr)
	, mPHome(nullptr)
	, mEHome(nullptr)
	, mCurState(nullptr)
	, mCameraManager(nullptr)
	, mActorManager(nullptr)
	, mUIManager(nullptr)
	, mSoundVolumeManager(nullptr)
{

}

bool Game::Initialize()
{
	SetAllData(mContainer);
	window(mContainer.mWidth, mContainer.mHeight, mContainer.mFullScreenFlag);
	loadGraphic(mContainer);
	LoadData();
	initDeltaTime();
	return true;
}

void Game::RunLoop()
{
	while (notQuit)
	{
		clear(mContainer.mBackGroundColor);
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}

void Game::Shutdown()
{
	delete mTransition;
	delete mActorManager;
	delete mCameraManager;
	delete mUIManager;
	MapClear();
	delete mRenderer;
	delete mSoundVolumeManager;
}

void Game::ProcessInput()
{
	if (mGameState == EGameplay)
	{
		if (isTrigger(KEY_ESCAPE))
		{
			mGameState = Game::EQuit;
		}

		mActorManager->ProcessInput();
		mUIManager->ProcessInput();

		//Pause
		if (isTrigger(KEY_ENTER))
		{
			new Pause(this);
		}

	}
	else
	{
		mUIManager->ProcessInput();
	}

}

void Game::UpdateGame()
{
	setDeltaTime();

	mCameraManager->Update();
	mActorManager->Update();
	mUIManager->Update();

	//ƒQ[ƒ€I—¹
	if (mGameState == EQuit)
	{
		closeWindow();
	}
}

void Game::GenerateOutput()
{
	mRenderer->Draw();
	//print(GetScene());
	//print("UI :" + (let)GetUIStack().size());
}

void Game::LoadData()
{
	mActorManager = new ActorManager(this);
	mUIManager = new UIManager(this);
	mRenderer = new Renderer(this);
	mTransition = new TransitionFade(this);
	mTransition->create();
	mSoundVolumeManager = new SoundVolumeManager(this);
	new GamePlay(this);
}

void Game::MapClear()
{
	if (mMap)
	{
		delete mMap;
	}
}

