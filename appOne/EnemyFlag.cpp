#include "EnemyFlag.h"
#include "TreeMeshComponent.h"

EnemyFlag::EnemyFlag(Game* game)
	:Actor(game)
{
	SetUp();
}
int EnemyFlag::SetUp()
{
	TreeMeshComponent* tc = new TreeMeshComponent(this);
	tc->SetTree("EnemyFlag");
	return 1;
}

void EnemyFlag::UpdateActor()
{

}