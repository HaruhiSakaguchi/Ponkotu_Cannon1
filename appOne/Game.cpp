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
#include "Enemy.h"
#include "Item.h"
#include "ActorsWeapon.h"
#include "PlayerHome.h"
#include "EnemyHome.h"

Game::Game()
	: mGameState(EGameplay)
	, mGameScene(ETitle)
	, mTmpScene(mGameScene)
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
	, mCannon(nullptr)
	, mMap(nullptr)
	, mStage(nullptr)
	, mPHome(nullptr)
	, mEHome(nullptr)
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
	ActorClear();
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
		if (mGameScene == Game::EPlay)
		{
			mUpdatingActors = true;
			for (auto actor : mActors)
			{
				actor->ProcessInput();
			}
			mUpdatingActors = false;

			//Pause
			if (isTrigger(KEY_ENTER))
			{
				new Pause(this);
			}
			for (auto ui : mUIStack)
			{
				ui->ProcessInput();
			}
		}
		else
		{
			if (!mUIStack.empty())
			{
				mUIStack.back()->ProcessInput();
			}
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

	if (mGameScene != ETitle)
	{
		if (mGameState == EGameplay)
		{
			// mActors更新(更新中にnewされたActorはmPendingActorsに追加される)
			mUpdatingActors = true;
			for (auto actor : mActors)
			{
				actor->Update();
			}
			mUpdatingActors = false;

			// 追加を延期したActorをmActorsに追加する
			for (auto pending : mPendingActors)
			{
				mActors.emplace_back(pending);
			}
			mPendingActors.clear();

			// Dead状態のActorを直下のdeadActorsに抽出する
			std::vector<Actor*> deadActors;
			for (auto actor : mActors)
			{
				if (actor->GetState() == Actor::EDead)
				{
					deadActors.emplace_back(actor);
				}
			}
			// deadActorsを消去する(mActorsからも取り除かれる)
			for (auto actor : deadActors)
			{
				delete actor;
			}
		}

	}

	//uiのupdate
	for (auto ui : mUIStack)
	{
		if (ui->GetState() == UIScreen::EActive)
		{
			ui->Update();
		}
	}

	// UIのdelete
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

	//ゲーム終了
	if (mGameState == EQuit)
	{
		closeWindow();
	}
}

void Game::GenerateOutput()
{
	mRenderer->Draw();
	print(GetScene());
}

void Game::LoadData()
{
	mRenderer = new Renderer(this);
	mTransition = new TransitionFade(this);
	mTransition->create();
	mEffectVol = GetAllData()->mInitAllSoundVolume;
	mSetVolume = GetAllData()->mInitEffectSoundVolume;
	mSensitivityX = GetAllData()->cameraData.mRotSpeedX;
	mSensitivityY = GetAllData()->cameraData.mRotSpeedY;
	mTitle = new Title(this);
}

void Game::PushUI(UIScreen* uiScreen)
{
	mUIStack.emplace_back(uiScreen);
}

void Game::AddActor(Actor* actor)
{
	if (mUpdatingActors)
	{
		mPendingActors.emplace_back(actor);
	}
	else
	{
		mActors.emplace_back(actor);
	}
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

void Game::RemoveActor(Actor* actor)
{
	//このactorがmActorsにあるか探す
	auto iter = std::find(mActors.begin(), mActors.end(), actor);
	if (iter != mActors.end())
	{
		//このActorとケツのActorを入れ替える(消去後コピー処理を避けるため)
		std::iter_swap(iter, mActors.end() - 1);
		mActors.pop_back();
	}
}

void Game::AddCharacter(CharacterActor* actor)
{
	mCharacters.emplace_back(actor);
}

void Game::RemoveCharacter(CharacterActor* actor)
{
	//このactorがmActorsにあるか探す
	auto iter = std::find(mCharacters.begin(), mCharacters.end(), actor);
	if (iter != mCharacters.end())
	{
		//このActorとケツのActorを入れ替える(消去後コピー処理を避けるため)
		std::iter_swap(iter, mCharacters.end() - 1);
		mCharacters.pop_back();
	}
}

void Game::AddPSide(class CharacterActor* actor)
{
	mPSideActors.emplace_back(actor);
}

void Game::RemovePSide(class CharacterActor* actor)
{
	auto iter = std::find(mPSideActors.begin(), mPSideActors.end(), actor);
	if (iter != mPSideActors.end())
	{
		//このActorとケツのActorを入れ替える(消去後コピー処理を避けるため)
		std::iter_swap(iter, mPSideActors.end() - 1);
		mPSideActors.pop_back();
	}
}

void Game::AddWeapons(ActorsWeapon* weapon)
{
	mWeapons.emplace_back(weapon);
}

void Game::RemoveWeapons(ActorsWeapon* weapon)
{
	//このactorがmActorsにあるか探す
	auto iter = std::find(mWeapons.begin(), mWeapons.end(), weapon);
	if (iter != mWeapons.end())
	{
		//このActorとケツのActorを入れ替える(消去後コピー処理を避けるため)
		std::iter_swap(iter, mWeapons.end() - 1);
		mWeapons.pop_back();
	}
}

void Game::AddEnemies(Enemy* enemy)
{
	mEnemies.emplace_back(enemy);
}

void Game::RemoveEnemies(Enemy* enemy)
{
	//このactorがmActorsにあるか探す
	auto iter = std::find(mEnemies.begin(), mEnemies.end(), enemy);
	if (iter != mEnemies.end())
	{
		//このActorとケツのActorを入れ替える(消去後コピー処理を避けるため)
		std::iter_swap(iter, mEnemies.end() - 1);
		mEnemies.pop_back();
	}
}

void Game::AddItems(Item* item)
{
	mItems.emplace_back(item);
}

void Game::RemoveItems(Item* item)
{
	//このactorがmActorsにあるか探す
	auto iter = std::find(mItems.begin(), mItems.end(), item);
	if (iter != mItems.end())
	{
		//このActorとケツのActorを入れ替える(消去後コピー処理を避けるため)
		std::iter_swap(iter, mItems.end() - 1);
		mItems.pop_back();
	}
}

void Game::AddCannon(class Cannon* cannon)
{
	mCannons.emplace_back(cannon);
}

void Game::RemoveCannon(class Cannon* cannon)
{
	//このactorがmActorsにあるか探す
	auto iter = std::find(mCannons.begin(), mCannons.end(), cannon);
	if (iter != mCannons.end())
	{
		//このActorとケツのActorを入れ替える(消去後コピー処理を避けるため)
		std::iter_swap(iter, mCannons.end() - 1);
		mCannons.pop_back();
	}
}


void Game::ActorClear()
{
	while (!mActors.empty())
	{
		delete mActors.back();
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
