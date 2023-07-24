#include "ActorsWeapon.h"
#include "Game.h"

ActorsWeapon::ActorsWeapon(class CharacterActor* owner, const VECTOR& pos, const VECTOR& dir)
	: CharacterActor(owner->GetGame())
	, mOwner(owner)
	, mTmpPos(pos)
	, mTmpDir(dir)
	, mMove(nullptr)
	, mMoveFlag(true)
{
	GetGame()->GetActorManager()->AddWeapons(this);
	SetCategory(Actor::ActorsCategory::EWeapon);
}

ActorsWeapon::~ActorsWeapon()
{
	GetGame()->GetActorManager()->RemoveWeapons(this);
}

int ActorsWeapon::SetUp(const DATA& data)
{
	return 0;
}

void ActorsWeapon::damage()
{
	SetState(State::EDead);
}