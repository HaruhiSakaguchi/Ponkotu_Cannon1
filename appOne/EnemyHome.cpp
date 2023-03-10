#include "EnemyHome.h"
#include "TreeMeshComponent.h"
#include "Dore.h"
#include "EnemyFlag.h"
#include "HpGaugeSpriteComponent.h"
#include "Game.h"
#include "Collision_Capsul.h"

EnemyHome::EnemyHome(class Game* game)
	:CharacterActor(game)
	, mFlag1(nullptr)
	, mFlag2(nullptr)
	, mDore(nullptr)
{
	SetUp();
	GetGame()->SetEHome(this);
}

EnemyHome::~EnemyHome()
{
	mDore->SetState(EDead);
	mFlag1->SetState(EDead);
	mFlag2->SetState(EDead);
	GetGame()->SetEHome(nullptr);
}

int EnemyHome::SetUp()
{
	TreeMeshComponent* tc = new TreeMeshComponent(this);
	tc->SetTree("Home");
	mDore = new Dore(GetGame());
	mFlag1 = new EnemyFlag(GetGame());
	mFlag2 = new EnemyFlag(GetGame());
	SetHp(Data.mMaxHp);
	SetRadius(Data.mRadius);
	SetHeight(Data.mHeight);
	new HpGaugeSpriteComponent(this, Data.mHpGaugeOffset);
	return 1;
}

void EnemyHome::UpdateActor()
{
	mDore->SetPosition(GetPosition() + Data.mDoreOffset);
	mFlag1->SetPosition(GetPosition() + Data.mFlag1Offset);
	mFlag2->SetPosition(GetPosition() + Data.mFlag2Offset);

	if (GetGame()->GetCannon())
	{
		Intersect(this, GetGame()->GetCannon());
	}
}

void EnemyHome::Damage(int damage)
{
	SetHp(GetHp() - damage);
	if (GetHp() <= 0)
	{
		SetState(EDead);
	}
}