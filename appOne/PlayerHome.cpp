#include "PlayerHome.h"
#include "TreeMeshComponent.h"
#include "Dore.h"
#include "PlayerFlag.h"
#include "HpGaugeSpriteComponent.h"
#include "Collision_Capsul.h"
#include "Game.h"
#include "window.h"
#include "EnemyHome.h"
#include "UIPSideCharacterStatusClose.h"
#include "UIPlayerHome.h"

PlayerHome::PlayerHome(class Game* game)
	: PSideCharacterActor(game)
	, mFlag1(nullptr)
	, mFlag2(nullptr)
	, mDore(nullptr)
	, mUI(nullptr)
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
	mUI->CloseMe();
}

int PlayerHome::SetUp()
{
	TreeMeshComponent* tc = new TreeMeshComponent(this);
	tc->SetTree("Home");
	mDore = new Dore(GetGame());
	mFlag1 = new PlayerFlag(GetGame());
	mFlag2 = new PlayerFlag(GetGame());
	SetTag(PHome);
	SetHp(Data.mMaxHp);
	SetMaxHp(Data.mMaxHp);

	SetRadius(Data.mRadius);
	SetHeight(Data.mHeight);
	SetName("PlayerHome");
	new HpGaugeSpriteComponent(this, Data.mHpGaugeOffset);
	mUI = new UIPlayerHome(this);
	//new UIPSideCharacterStatusClose(this);
	return 1;
}

void PlayerHome::UpdateActor()
{
	mDore->SetPosition(GetPosition() + Data.mDoreOffset);
	mFlag1->SetPosition(GetPosition() + Data.mFlag1Offset);
	mFlag2->SetPosition(GetPosition() + Data.mFlag2Offset);

	mHomeTargetPoints[0] = GetPosition() + VECTOR(-7.0f, 0.0f, -7.0f);
	mHomeTargetPoints[1] = GetPosition() + VECTOR(7.0f, 0.0f, -7.0f);
	mHomeTargetPoints[2] = GetPosition() + VECTOR(7.0f, 0.0f, 5.0f);
	mHomeTargetPoints[3] = GetPosition() + VECTOR(-7.0f, 0.0f, 5.0f);

	VECTOR PEHomeCenterPos = (GetPosition() + GetGame()->GetEHome()->GetPosition()) / 2;

	mFieldTargetPoints[0] = PEHomeCenterPos + VECTOR(-7.0f, 0.0f, -7.0f);
	mFieldTargetPoints[1] = PEHomeCenterPos + VECTOR(7.0f, 0.0f, -7.0f);
	mFieldTargetPoints[2] = PEHomeCenterPos + VECTOR(7.0f, 0.0f, 7.0f);
	mFieldTargetPoints[3] = PEHomeCenterPos + VECTOR(-7.0f, 0.0f, 7.0f);


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