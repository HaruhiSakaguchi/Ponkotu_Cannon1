#include "Game.h"
#include "framework.h"
#include "window.h"
#include "graphic.h"
#include "input.h"

#include "Renderer.h"
#include "UIScreen.h"
#include "TransitionFade.h"
#include "Title.h"
#include "Pause.h"
#include "Actor.h"
#include "Camera.h"
#include "COLLISION_MAP.h"
#include "Map.h"
#include "PlayerHome.h"
#include "EnemyHome.h"
#include "UIPSideCharacterStatusBase.h"
#include "ActorManager.h"

Game::Game()
	: mGameState(EGameplay)
	, mPhase(FIRST)
	, mInitPhase(mPhase)
	, mEffectVol(0)
	, mSetVolume(0)
	, mTmpVolume(0)
	, mTmpEffectVol(0)
	, mSensitivityX(0)
	, mSensitivityY(0)
	, mSoundFlag(true)
	, mBgmFlag(true)
	, mContinueFlag(false)
	, mRenderer(nullptr)
	, mTransition(nullptr)
	, mDisplayColor(0, 0, 0, 0)
	, mCamera(nullptr)
	, mMap(nullptr)
	, mStage(nullptr)
	, mPHome(nullptr)
	, mEHome(nullptr)
	, mCurState(nullptr)
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
	MapClear();
	UIClear();
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

		mActorManager->ProcessInput();

		for (auto ui : mUIStack)
		{
			ui->ProcessInput();
		}

		//Pause
		if (isTrigger(KEY_ENTER))
		{
			new Pause(this);
		}

	}
	else if (!mUIStack.empty())
	{
		mUIStack.back()->ProcessInput();
	}
}

void Game::UpdateGame()
{
	setDeltaTime();

	mActorManager->Update();

	//ui‚Ìupdate
	for (auto ui : mUIStack)
	{
		if (ui->GetState() == UIScreen::EActive)
		{
			ui->Update();
		}
	}

	// UI‚Ìdelete
	std::vector<UIScreen*> deadUis;
	for (auto ui : mUIStack)
	{
		if (ui->GetState() == UIScreen::EClosing)
		{
			deadUis.emplace_back(ui);
		}
	}

	for (auto ui : deadUis)
	{
		delete ui;
	}

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
	mRenderer = new Renderer(this);
	mTransition = new TransitionFade(this);
	mTransition->create();
	mEffectVol = GetAllData()->mInitAllSoundVolume;
	mSetVolume = GetAllData()->mInitEffectSoundVolume;
	mSensitivityX = GetAllData()->cameraData.mRotSpeedX;
	mSensitivityY = GetAllData()->cameraData.mRotSpeedY;

	new GamePlay(this);

}


void Game::PushUI(UIScreen* uiScreen)
{
	mUIStack.emplace_back(uiScreen);
}

void Game::PullUI(UIScreen* uiScreen)
{
	auto iter = std::find(mUIStack.begin(), mUIStack.end(), uiScreen);
	if (iter != mUIStack.end())
	{
		std::iter_swap(iter, mUIStack.end() - 1);
		mUIStack.pop_back();
	}
}

void Game::UIClear()
{
	while (!mUIStack.empty())
	{
		delete mUIStack.back();
	}
}

void Game::MapClear()
{
	if (mMap)
	{
		delete mMap;
	}
}

void Game::AddUIPSide(UIPSideCharacterStatusBase* uiScreen)
{
	mUIStatus.emplace_back(uiScreen);
}

void Game::RemoveUIPSide(UIPSideCharacterStatusBase* uiScreen)
{
	auto iter = std::find(mUIStatus.begin(), mUIStatus.end(), uiScreen);
	if (iter != mUIStatus.end())
	{
		std::iter_swap(iter, mUIStatus.end() - 1);
		mUIStatus.pop_back();
	}
}

