#include "Game.h"
#include "framework.h"
#include "window.h"
#include "graphic.h"
#include "input.h"

#include "Renderer.h"
#include "Title.h"
#include "Pause.h"


Game::Game()
	: mGameState(EGameplay)
	, mPhase(FIRST)
	, mInitPhase(mPhase)
	, mContinueFlag(false)
	, mRenderer(nullptr)
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
	delete mActorManager;
	delete mCameraManager;
	delete mUIManager;
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
}

void Game::LoadData()
{
	mActorManager = new ActorManager(this);
	mUIManager = new UIManager(this);
	mRenderer = new Renderer(this);
	mSoundVolumeManager = new SoundVolumeManager(this);
	new GamePlay(this);
}

