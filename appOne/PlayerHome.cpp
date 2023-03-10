#include "PlayerHome.h"
#include "TreeMeshComponent.h"
#include "Dore.h"
#include "PlayerFlag.h"
#include "HpGaugeSpriteComponent.h"
#include "Collision_Capsul.h"
#include "Game.h"

PlayerHome::PlayerHome(class Game* game)
	:CharacterActor(game)
	, mFlag1(nullptr)
	, mFlag2(nullptr)
	, mDore(nullptr)
{
	SetUp();
	GetGame()->SetPHome(this);
}

PlayerHome::~PlayerHome()
{
	mDore->SetState(EDead);
	mFlag1->SetState(EDead);
	mFlag2->SetState(EDead);
	GetGame()->SetPHome(nullptr);

}

int PlayerHome::SetUp()
{
	TreeMeshComponent* tc = new TreeMeshComponent(this);
	tc->SetTree("Home");
	mDore = new Dore(GetGame());
	mFlag1 = new PlayerFlag(GetGame());
	mFlag2 = new PlayerFlag(GetGame());
	SetHp(Data.mMaxHp);
	SetRadius(Data.mRadius);
	SetHeight(Data.mHeight);
	new HpGaugeSpriteComponent(this, Data.mHpGaugeOffset);
	return 1;
}

void PlayerHome::UpdateActor()
{
	mDore->SetPosition(GetPosition() + Data.mDoreOffset);
	mFlag1->SetPosition(GetPosition() + Data.mFlag1Offset);
	mFlag2->SetPosition(GetPosition() + Data.mFlag2Offset);

	for(auto enemy : GetGame()->GetEnemies())
	{
		Intersect(this, enemy);
	}
}

void PlayerHome::Damage(int damage)
{
	SetHp(GetHp() - damage);
	if (GetHp() <= 0)
	{
		SetState(EDead);
	}
}