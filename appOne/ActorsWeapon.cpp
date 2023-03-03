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
	GetGame()->AddWeapons(this);
	SetCategory(Actor::Weapon);
}

ActorsWeapon::~ActorsWeapon()
{
	GetGame()->RemoveWeapons(this);
}

int ActorsWeapon::SetUp(const DATA& data)
{
	return 0;
}

void ActorsWeapon::damage()
{
	SetState(EDead);
}