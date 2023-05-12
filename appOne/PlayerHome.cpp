#include "PlayerHome.h"
#include "TreeMeshComponent.h"
#include "Dore.h"
#include "PlayerFlag.h"
#include "HpGaugeSpriteComponent.h"
#include "Collision_Capsul.h"
#include "Game.h"
#include "window.h"

PlayerHome::PlayerHome(class Game* game)
	: CharacterActor(game)
	, mFlag1(nullptr)
	, mFlag2(nullptr)
	, mDore(nullptr)
{
	SetUp();
	GetGame()->SetPHome(this);
	GetGame()->AddPSide(this);
}

PlayerHome::~PlayerHome()
{
	mDore->SetState(EDead);
	mFlag1->SetState(EDead);
	mFlag2->SetState(EDead);
	GetGame()->SetPHome(nullptr);
	GetGame()->RemovePSide(this);
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

	if (GetDamageInterval() > 0.0f)
	{
		SetDamageInterval(GetDamageInterval() - delta);
	}

	for (auto enemy : GetGame()->GetEnemies())
	{
		if (enemy->GetPosition().y == 0)
		{
			Intersect(this, enemy);
		}
	}
}

void PlayerHome::Damage(int damage)
{
	if (GetDamageInterval() <= 0.0f)
	{
		SetHp(GetHp() - damage);
		SetDamageInterval(Data.mMaxDamageInterval);
	}
	else
	{
		return;
	}

	if (GetHp() <= 0)
	{
		setVolume(mDeadSound, GetGame()->GetEffectVolume());
		playSound(mDeadSound);
		GetGame()->GetStage()->AddText("PlayerHome‚ª‰ó‚ê‚½");
		SetState(EDead);
	}
}