#include "ActorManager.h"
#include "Game.h"
#include "Map.h"
#include "Enemy.h"
#include "Item.h"
#include "ActorsWeapon.h"
#include "PlayerHome.h"
#include "EnemyHome.h"
#include "Actor.h"
#include "Camera.h"

ActorManager::ActorManager(class Game* game) :
	mGame(game)
	, mUpdatingActors(false)
	, mStage(nullptr)
    , mPHome(nullptr)
    , mEHome(nullptr)
{
};

ActorManager::~ActorManager()
{
	ActorClear();
}

void ActorManager::Update()
{
	if (mGame->GetState() == Game::EGameplay)
	{
		// mActors�X�V(�X�V����new���ꂽActor��mPendingActors�ɒǉ������)
		mUpdatingActors = true;
		for (auto actor : mActors)
		{
			actor->Update();
		}
		mUpdatingActors = false;

		// �ǉ�����������Actor��mActors�ɒǉ�����
		for (auto pending : mPendingActors)
		{
			mActors.emplace_back(pending);
		}
		mPendingActors.clear();

		// Dead��Ԃ�Actor�𒼉���deadActors�ɒ��o����
		std::vector<Actor*> deadActors;
		for (auto actor : mActors)
		{
			if (actor->GetState() == Actor::EDead)
			{
				deadActors.emplace_back(actor);
			}
		}
		// deadActors����������(mActors�������菜�����)
		for (auto actor : deadActors)
		{
			delete actor;
		}
	}
}

void ActorManager::ProcessInput()
{
	mUpdatingActors = true;

	for (auto actor : mActors)
	{
		actor->ProcessInput();
	}

	mUpdatingActors = false;
}

void ActorManager::AddActor(Actor* actor)
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

void ActorManager::RemoveActor(Actor* actor)
{
	//����actor��mActors�ɂ��邩�T��
	auto iter = std::find(mActors.begin(), mActors.end(), actor);
	if (iter != mActors.end())
	{
		//����Actor�ƃP�c��Actor�����ւ���(������R�s�[����������邽��)
		std::iter_swap(iter, mActors.end() - 1);
		mActors.pop_back();
	}
}

void ActorManager::AddCharacter(CharacterActor* actor)
{
	mCharacters.emplace_back(actor);
}

void ActorManager::RemoveCharacter(CharacterActor* actor)
{
	//����actor��mActors�ɂ��邩�T��
	auto iter = std::find(mCharacters.begin(), mCharacters.end(), actor);
	if (iter != mCharacters.end())
	{
		//����Actor�ƃP�c��Actor�����ւ���(������R�s�[����������邽��)
		std::iter_swap(iter, mCharacters.end() - 1);
		mCharacters.pop_back();
	}
}

void ActorManager::AddPSide(class PSideCharacterActor* actor)
{
	mPSideActors.emplace_back(actor);
}

void ActorManager::RemovePSide(class PSideCharacterActor* actor)
{
	auto iter = std::find(mPSideActors.begin(), mPSideActors.end(), actor);
	if (iter != mPSideActors.end())
	{
		//����Actor�ƃP�c��Actor�����ւ���(������R�s�[����������邽��)
		std::iter_swap(iter, mPSideActors.end() - 1);
		mPSideActors.pop_back();
	}
}

void ActorManager::AddWeapons(ActorsWeapon* weapon)
{
	mWeapons.emplace_back(weapon);
}

void ActorManager::RemoveWeapons(ActorsWeapon* weapon)
{
	//����actor��mActors�ɂ��邩�T��
	auto iter = std::find(mWeapons.begin(), mWeapons.end(), weapon);
	if (iter != mWeapons.end())
	{
		//����Actor�ƃP�c��Actor�����ւ���(������R�s�[����������邽��)
		std::iter_swap(iter, mWeapons.end() - 1);
		mWeapons.pop_back();
	}
}

void ActorManager::AddEnemies(Enemy* enemy)
{
	mEnemies.emplace_back(enemy);
}

void ActorManager::RemoveEnemies(Enemy* enemy)
{
	//����actor��mActors�ɂ��邩�T��
	auto iter = std::find(mEnemies.begin(), mEnemies.end(), enemy);
	if (iter != mEnemies.end())
	{
		//����Actor�ƃP�c��Actor�����ւ���(������R�s�[����������邽��)
		std::iter_swap(iter, mEnemies.end() - 1);
		mEnemies.pop_back();
	}
}

void ActorManager::AddItems(Item* item)
{
	mItems.emplace_back(item);
}

void ActorManager::RemoveItems(Item* item)
{
	//����actor��mActors�ɂ��邩�T��
	auto iter = std::find(mItems.begin(), mItems.end(), item);
	if (iter != mItems.end())
	{
		//����Actor�ƃP�c��Actor�����ւ���(������R�s�[����������邽��)
		std::iter_swap(iter, mItems.end() - 1);
		mItems.pop_back();
	}
}

void ActorManager::AddCannon(class Cannon* cannon)
{
	mCannons.emplace_back(cannon);
}

void ActorManager::RemoveCannon(class Cannon* cannon)
{
	//����actor��mActors�ɂ��邩�T��
	auto iter = std::find(mCannons.begin(), mCannons.end(), cannon);
	if (iter != mCannons.end())
	{
		//����Actor�ƃP�c��Actor�����ւ���(������R�s�[����������邽��)
		std::iter_swap(iter, mCannons.end() - 1);
		mCannons.pop_back();
	}
}

void ActorManager::ActorClear()
{
	while (!mActors.empty())
	{
		delete mActors.back();
	}
}
