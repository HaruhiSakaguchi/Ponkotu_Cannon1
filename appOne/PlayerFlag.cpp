#include "PlayerFlag.h"
#include "TreeMeshComponent.h"

PlayerFlag::PlayerFlag(Game* game)
	:Actor(game)
{
	SetUp();
}
int PlayerFlag::SetUp()
{
	mTc = new TreeMeshComponent(this);
	mTc->SetTree("PlayerFlag");
	return 1;
}

void PlayerFlag::UpdateActor()
{

}