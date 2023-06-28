#include "PlayerHome.h"
#include "TreeMeshComponent.h"
#include "PlayerFlag.h"
#include "HpGaugeSpriteComponent.h"
#include "Collision_Capsul.h"
#include "Game.h"
#include "window.h"
#include "EnemyHome.h"
#include "UIPSideCharacterStatusClose.h"
#include "UIPlayerHome.h"

PlayerHome::PlayerHome(class Game* game, const VECTOR& pos)
	: PSideCharacterActor(game)
	, mTc(nullptr)
	, mFlag1(nullptr)
	, mFlag2(nullptr)
	, mDore(nullptr)
	, mUI(nullptr)
	, mGenerateCannonLevel(0)
	, mGenerateBarricadeLevel(0)
	, mCurMyTpIdx(0)
	, mGenerateFlag(true)
	, mBattlePoints(0)
	, mMaxBattlePoints(0)

{
	SetInitPosition(pos);
	SetPosition(pos);
	SetUp();
	GetGame()->GetActorManager()->SetPHome(this);

}

PlayerHome::~PlayerHome()
{
	mDore->SetState(EDead);
	mFlag1->SetState(EDead);
	mFlag2->SetState(EDead);
	GetGame()->GetActorManager()->SetPHome(nullptr);
	mUI->CloseMe();
}

int PlayerHome::SetUp()
{
	mTc = new TreeMeshComponent(this);
	mTc->SetTree("Home");
	SetNormalMesh(mTc);
	mDore = new Dore(GetGame());
	mFlag1 = new PlayerFlag(GetGame());
	mFlag2 = new PlayerFlag(GetGame());
	SetTag(PHome);
	SetHp(Data.mMaxHp);
	SetMaxHp(Data.mMaxHp);
	SetInitMaxHp(Data.mMaxHp);

	mDore->SetRotationX(0.0f);
	mDore->SetRotationY(-3.1415926f);
	mDore->SetIsRotate(true);
	mDore->Open();
	mDore->SetCloseEvent([this]() { SetGenerateFlag(false); });

	SetRadius(Data.mRadius);
	SetHeight(Data.mHeight);
	SetName("PlayerHome");
	new HpGaugeSpriteComponent(this, Data.mHpGaugeOffset);
	mUI = new UIPlayerHome(this);
	mMaxBattlePoints = 500;
	mHomeTargetPoint = GetInitPosition();
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

	if (GetGame()->GetActorManager()->GetEHome())
	{
		VECTOR PEHomeCenterPos = (GetPosition() + GetGame()->GetActorManager()->GetEHome()->GetPosition()) / 2;

		mFieldTargetPoints[0] = PEHomeCenterPos + VECTOR(-7.0f, 0.0f, -7.0f);
		mFieldTargetPoints[1] = PEHomeCenterPos + VECTOR(7.0f, 0.0f, -7.0f);
		mFieldTargetPoints[2] = PEHomeCenterPos + VECTOR(7.0f, 0.0f, 7.0f);
		mFieldTargetPoints[3] = PEHomeCenterPos + VECTOR(-7.0f, 0.0f, 7.0f);
	}


	mMyTargetPoints[0] = GetInitPosition();
	mMyTargetPoints[1] = GetInitPosition() + (VECTOR(0.0f, 0.0f, (GetGame()->GetActorManager()->GetStage()->GetStageMaxZ() + GetGame()->GetActorManager()->GetStage()->GetStageMinZ())) / 4.0f);
	mMyTargetPoints[2] = GetInitPosition() + (VECTOR(0.0f, 0.0f, (GetGame()->GetActorManager()->GetStage()->GetStageMaxZ() + GetGame()->GetActorManager()->GetStage()->GetStageMinZ())) / 2.0f);


	if (GetDamageInterval() > 0.0f)
	{
		SetDamageInterval(GetDamageInterval() - delta);
	}

	for (auto enemy : GetGame()->GetActorManager()->GetEnemies())
	{
		if (enemy->GetTag() != CharacterActor::Satellite && enemy->GetHp() > 0)
		{
			if (Intersect(this, enemy) && !mMoveCompleteFlag)
			{
				enemy->Damage(1 + (int)(GetLevel() / 2));
			}
		}
	}

	mMoveCompleteFlag = GoToTargetPoint(mHomeTargetPoint);

	if (mMoveCompleteFlag && mGenerateFlag)
	{
		mDore->SetIsRotate(true);
	}

	int cnt = 0;
	for (auto Actor : GetGame()->GetActorManager()->GetPSide())
	{
		if (Actor != this && CollisionCircle(GetRadius(), Actor->GetRadius(), GetPosition(), Actor->GetPosition()))
		{
			cnt++;
		}
	}

	if (mGenerateFlag && cnt == 0)
	{
		mDore->Close();
	}

	mMaxBattlePoints = 500 * (GetLevel() + 1);

	if (mBattlePoints < mMaxBattlePoints)
	{
		mBattlePoints++;
	}
	if (mBattlePoints < 0)
	{
		mBattlePoints = 0;
	}

	if (GetDamageInterval() > 0)
	{
		mDore->GetMesh()->SetDrawFlag(false);
		mFlag1->GetMesh()->SetDrawFlag(false);
		mFlag2->GetMesh()->SetDrawFlag(false);
	}
	else
	{
		mDore->GetMesh()->SetDrawFlag(true);
		mFlag1->GetMesh()->SetDrawFlag(true);
		mFlag2->GetMesh()->SetDrawFlag(true);
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
		SetState(EDead);
	}
}

void PlayerHome::Dead()
{
	setVolume(mDeadSound, GetGame()->GetSoundVolumeManager()->GetEffectVolume());
	playSound(mDeadSound);
	GetGame()->GetActorManager()->GetStage()->AddText("PlayerHome‚ª‰ó‚ê‚½");
	SpawnParticle(GetPosition(), "HomeHouse", 20);
	SpawnParticle(GetPosition(), "DoreDore", 20);
	SpawnParticle(GetPosition(), "PlayerFlagFlag", 40);
}

bool PlayerHome::GoToTargetPoint(const VECTOR& pos)
{
	VECTOR dir = pos - GetPosition();
	dir.normalize();

	if (CollisionCircle(0.5f, 0.5f, GetPosition(), pos))
	{
		return true;
	}
	else
	{
		SetPosition(GetPosition() + dir * delta * 6.0f);
		return false;
	}
}

