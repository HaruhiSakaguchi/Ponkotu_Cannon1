#pragma once
#include <vector>
class ActorManager
{
public:
	ActorManager(class Game* game) :mGame(game),mUpdatingActors(false){};
	~ActorManager();
	void Update();
	void ProcessInput();
	//getter&setter
	const class std::vector<class Actor*>& GetActors() { return mActors; }
	const class std::vector<class CharacterActor*>& GetCharacters() { return mCharacters; }
	const class std::vector<class Enemy*>& GetEnemies() { return mEnemies; }
	const class std::vector<class ActorsWeapon*>& GetWeapons() { return mWeapons; }
	const class std::vector<class Item*>& GetItems() { return mItems; }
	const class std::vector<class PSideCharacterActor*>& GetPSide() { return mPSideActors; }
	const class std::vector<class Cannon*>& GetCannons() { return mCannons; }

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
	void AddPSide(class PSideCharacterActor* actor);
	void RemovePSide(class PSideCharacterActor* actor);
	void AddCannon(class Cannon* cannon);
	void RemoveCannon(class Cannon* cannon);

	void ActorClear();

private:
	Game* mGame;
	std::vector<class Actor*>mActors;
	std::vector<class CharacterActor*>mCharacters;
	std::vector<class Enemy*>mEnemies;
	std::vector<class Item*>mItems;
	std::vector<class Cannon*>mCannons;
	std::vector<class PSideCharacterActor*>mPSideActors;
	std::vector<class ActorsWeapon*> mWeapons;

	bool mUpdatingActors;
	std::vector<class Actor*> mPendingActors;
};

