#include "PlayerFlag.h"
#include "TreeMeshComponent.h"

PlayerFlag::PlayerFlag(Game* game)
	:Actor(game)
{
	SetUp();
}
int PlayerFlag::SetUp()
{
	TreeMeshComponent*tc = new TreeMeshComponent(this);
	tc->SetTree("PlayerFlag");
	return 1;
}

void PlayerFlag::UpdateActor()
{

}