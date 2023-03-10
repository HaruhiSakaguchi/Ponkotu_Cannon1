#pragma once
#include "Actor.h"
class EnemyFlag :
	public Actor
{
public:
	EnemyFlag(Game* game);
	int SetUp();
	void UpdateActor();
};

