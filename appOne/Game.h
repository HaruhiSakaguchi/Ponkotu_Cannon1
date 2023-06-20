#pragma once
#include "Container.h"
#include <vector>
#include "CharacterActor.h"
#include "UIManager.h"
#include "ActorManager.h"
#include "CameraManager.h"
#include "SoundVolumeManager.h"

class Game
{
public:
	Game();
	bool Initialize();
	void RunLoop();
	void Shutdown();
private:
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();
	void LoadData();
public:
	//ステート系

	enum GameState
	{
		EGameplay,
		EPaused,
		EQuit
	};

	GameState GetState() { return mGameState; }
	void SetState(GameState gameState) { mGameState = gameState; }
	void SetCurState(UIMainState* state) { mCurState = state; }
	UIMainState* GetCurState() { return mCurState; }

private:
	GameState mGameState;
public:

	enum StagePhase
	{
		FIRST = 0,
		SECOND = 1,
		THIRD = 2,
		FOURTH = 3,
		END
	};

	StagePhase GetPhase() { return mPhase; }
	void SetPhase(const StagePhase phase) { mPhase = phase; }
	StagePhase GetInitPhase() { return mInitPhase; }
private:
	StagePhase mPhase;
	StagePhase mInitPhase;
public:
	void MapClear();
public:

	struct Container* GetAllData() { return &mContainer; }
	void SetDisplayColor(const COLOR& color) { mDisplayColor = color; }
	const COLOR& GetDisplayColor() { return mDisplayColor; }
	class Renderer* GetRenderer() { return mRenderer; }
	class TransitionFade* GetTransition() { return mTransition; }
	bool GetContinueFlag() { return mContinueFlag; }
	void SetContinueFlag(bool flag) { mContinueFlag = flag; }

	class CameraManager* GetCameraManager() { return mCameraManager; }
	void SetCameraManager(class CameraManager* manager) { mCameraManager = manager; }

	void SetCollisionMap(class COLLISION_MAP* map) { mMap = map; }

	class COLLISION_MAP* GetCollisionMap() { return mMap; }
	class Map* GetStage() { return mStage; }
	void SetStage(Map* map) { mStage = map; }
	class PlayerHome* GetPHome() { return mPHome; }
	class EnemyHome* GetEHome() { return mEHome; }
	void SetPHome(class PlayerHome* home) { mPHome = home; }
	void SetEHome(class EnemyHome* home) { mEHome = home; }

	class ActorManager* GetActorManager() { return mActorManager; }
	class UIManager* GetUIManager() { return mUIManager; }
	class SoundVolumeManager* GetSoundVolumeManager() { return mSoundVolumeManager; }
private:
	struct Container mContainer;
	class Renderer* mRenderer;
	class TransitionFade* mTransition;
	class CameraManager* mCameraManager;
	class COLLISION_MAP* mMap;
	class Map* mStage;
	class UIMainState* mCurState;
	class ActorManager* mActorManager;
	class UIManager* mUIManager;
	class SoundVolumeManager* mSoundVolumeManager;
	//続けて最後までプレイするか否か
	bool mContinueFlag;
	COLOR mDisplayColor;
	class PlayerHome* mPHome;
	class EnemyHome* mEHome;
};

