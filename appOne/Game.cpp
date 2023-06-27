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
	, mUpdatingManagers(false)
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
	while (!mManagers.empty())
	{
		delete mManagers.back();
	}

	delete mRenderer;
}

void Game::ProcessInput()
{
	if (mGameState == EGameplay)
	{
		if (isTrigger(KEY_ESCAPE))
		{
			mGameState = Game::EQuit;
		}

		if (mGameState == EGameplay)
		{
			for (auto manager : mManagers)
			{
				manager->ProcessInput();
			}

			//Pause
			if (isTrigger(KEY_ENTER))
			{
				new Pause(this);
			}
		}

	}
	else if (mGameState == EPaused)
	{
		mUIManager->ProcessInput();
	}

}

void Game::UpdateGame()
{
	setDeltaTime();

	mUpdatingManagers = true;

	for (auto manager : mManagers)
	{
		manager->Update();
	}
	mUpdatingManagers = false;

	for (auto pending : mPendingManagers)
	{
		mManagers.emplace_back(pending);
	}

	mPendingManagers.clear();

	//ゲーム終了
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
	mUIManager = new UIManager(this);
	mActorManager = new ActorManager(this);
	mRenderer = new Renderer(this);
	mSoundVolumeManager = new SoundVolumeManager(this);
	new Title(this);
}

void Game::AddManager(class Manager* manager)
{
	if (mUpdatingManagers)
	{
		mPendingManagers.emplace_back(manager);
	}
	else
	{
		mManagers.emplace_back(manager);
	}
}

void Game::RemoveManager(class Manager* manager)
{
	//このactorがmActorsにあるか探す
	auto iter = std::find(mManagers.begin(), mManagers.end(), manager);
	if (iter != mManagers.end())
	{
		//このActorとケツのActorを入れ替える(消去後コピー処理を避けるため)
		std::iter_swap(iter, mManagers.end() - 1);
		mManagers.pop_back();
	}
}

