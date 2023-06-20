#include "Enemy.h"
#include "Game.h"

Enemy::Enemy(class Game* game)
	: CharacterActor(game)
	, mRevivalCnt(1)
	, mRevivalLimmit(0)
{
	GetGame()->GetActorManager()->AddEnemies(this);
}

Enemy::~Enemy()
{
	GetGame()->GetActorManager()->RemoveEnemies(this);
}