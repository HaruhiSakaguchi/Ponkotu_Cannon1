#pragma once
#include "Container.h"
#include <vector>
#include "CharacterActor.h"

class Game
{
public:
	Game();
	bool Initialize();
	void RunLoop();
	void Shutdown();

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
private:
	GameState mGameState;
public:

	enum GameScene
	{
		ETitle,
		EPlay,
		EStageClear,
		EGameQuit,
		EGameOver,
	};

	GameScene GetScene() { return mGameScene; }
	void SetScene(GameScene scene) { mGameScene = scene; }
	GameScene GetTmpScene() { return mTmpScene; }
	void SetTmpScene(GameScene scene) { mTmpScene = scene; }
private:
	GameScene mGameScene;
	GameScene mTmpScene;
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
	//Actor
	void AddActor(class Actor* actor);
	void RemoveActor(class Actor* actor);

	void AddCharacter(class CharacterActor* actor);
	void RemoveCharacter(class CharacterActor* actor);
	void AddWeapons(class ActorsWeapon* weapon);
	void RemoveWeapons(class ActorsWeapon* weapon);
	void AddEnemies(class Enemy* enemy);
	void RemoveEnemies(class Enemy* enemy);
	void AddItems(class Item* item);
	void RemoveItems(class Item* item);


	// UI
	void PushUI(class UIScreen* uiScreen);
	void PullUI(class UIScreen* uiScreen);
public:
	void ActorClear();
	void UIClear();
	void MapClear();
public:
	//getter&setter
	const class std::vector<Actor*>& GetActors() { return mActors; }
	const class std::vector<CharacterActor*>& GetCharacters() { return mCharacters; }
	const class std::vector<class UIScreen*>& GetUIStack() { return mUIStack; }
	const class std::vector<Enemy*>& GetEnemies() { return mEnemies; }
	const class std::vector<ActorsWeapon*>& GetWeapons() { return mWeapons; }
	const class std::vector<Item*>& GetItems() { return mItems; }
	struct Container* GetAllData() { return &mContainer; }
	void SetDisplayColor(const COLOR& color) { mDisplayColor = color; }
	const COLOR& GetDisplayColor() { return mDisplayColor; }
	class Renderer* GetRenderer() { return mRenderer; }
	class TransitionFade* GetTransition() { return mTransition; }
	bool GetContinueFlag() { return mContinueFlag; }
	void SetContinueFlag(bool flag) { mContinueFlag = flag;}
	class Actor* GetCamera() { return mCamera; }
	void SetCamera(class Camera* camera) { mCamera = camera; }
	class CharacterActor* GetCannon() { return mCannon; }
	void SetCannon(CharacterActor* cannon) { mCannon = cannon; }
	void SetCollisionMap(class COLLISION_MAP* map) { mMap = map; }

	class COLLISION_MAP* GetCollisionMap() { return mMap; }
	class Map* GetStage() { return mStage; }
	void SetStage(Map* map) { mStage = map; }
	class Title* GetTitle() { return mTitle; }
	class PlayerHome* GetPHome() { return mPHome; }
	class EnemyHome* GetEHome() { return mEHome; }
	void SetPHome(class PlayerHome*home) { mPHome = home; }
	void SetEHome(class EnemyHome*home) { mEHome = home; }

private:
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();
	void LoadData();
private:
	struct Container mContainer;
	class Renderer* mRenderer;
	class TransitionFade* mTransition;
	class Actor* mCamera;
	class CharacterActor* mCannon;
	class COLLISION_MAP* mMap;
	class Map* mStage;
	std::vector<class Actor*>mActors;
	std::vector<class CharacterActor*>mCharacters;
	std::vector<class Enemy*>mEnemies;
	std::vector<class Item*>mItems;

	std::vector<class ActorsWeapon*> mWeapons;
	// Update中フラッグ
	bool mUpdatingActors;
	std::vector<class Actor*> mPendingActors;
	std::vector<class UIScreen*>mUIStack;
	//続けて最後までプレイするか否か
	bool mContinueFlag;
	COLOR mDisplayColor;
	class PlayerHome*mPHome;
	class EnemyHome* mEHome;
	class Title* mTitle;
//音関係
private:
	int mTmpVolume;//音声OFF時にON時のボリュームをとっておく
	int mSetVolume;
	int mEffectVol;
	int mTmpEffectVol;
	bool mSoundFlag;
	bool mBgmFlag;
public:
	int GetEffectVolume() { return mEffectVol; }
	int GetTmpEffectVolume() { return mTmpEffectVol; }
	void SetEffectVolume(int vol) { mEffectVol = vol; }
	void SetTmpEffectVolume(int vol) { mTmpEffectVol = vol; }
	int GetVolume() { return mSetVolume; }
	int GetTmpVolume() { return mTmpVolume; }
	void SetVolume(int vol) { mSetVolume = vol; }
	void SetTmpVolume(int vol) { mTmpVolume = vol; }
	bool GetSoundFlag() { return mSoundFlag; }
	void SetSoundFlag(bool flag) { mSoundFlag = flag; }
	bool GetBgmFlag() { return mBgmFlag; }
	void SetBgmFlag(bool flag) { mBgmFlag = flag; }
//カメラ関係
private:
	//カメラ感度
	float mSensitivityX;
	float mSensitivityY;
public:
	void SetCameraSX(float sx) { mSensitivityX = sx; }
	void SetCameraSY(float sy) { mSensitivityY = sy; }
	float GetCameraSX() { return mSensitivityX; }
	float GetCameraSY() { return mSensitivityY; }

};

