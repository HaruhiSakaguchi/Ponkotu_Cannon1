#include "SphereWeapon.h"
#include "window.h"
#include "Game.h"
#include "Collision_Capsul.h"

SphereWeapon::SphereWeapon(class CharacterActor* owner, const VECTOR& pos, const VECTOR& dir)
	: ActorsWeapon(owner, pos, dir)
{
}

int SphereWeapon::SetUp(const DATA& data)
{
	SetRadius(data.mRadius);
	SetHeight(data.mHeight);
	SetAdvSpeed(data.mAdvSpeed);
	SetPosition(mTmpPos);
	SetImageColor(data.mImageColor);

	SetDir(mTmpDir);

	if (mMoveFlag)
	{
		mMove = new MoveComponent(this);
		mMove->SetSpeed(GetAdvSpeed());
		mMove->SetDirection(GetDir());
	}

	Data.mLifeTime = data.mMaxLifeTime;
	SetScale(VECTOR(data.mSize, data.mSize, data.mSize) * 2);

	return 1;
}


void SphereWeapon::UpdateActor()
{

	Data.mLifeTime -= Data.mLifeTimeDecreaseSpeed * delta;

	//GameSceneがEPlayでないならダメージを与えない
	if (GetGame()->GetScene() != Game::EPlay)
	{
		Data.mDamage = 0;
	}

	//LifeTimeがなくなるか、Ownerが死んだら死ぬ
	if (Data.mLifeTime <= 0.0f || GetOwner()->GetState() != Actor::EActive)
	{
		SetState(Actor::EDead);
	}

};