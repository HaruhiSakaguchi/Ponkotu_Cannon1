#include "Game.h"
#include "Tama.h"
#include "Collision_Capsul.h"
#include "window.h"
#include "COLLISION_MAP.h"
#include "HpGaugeSpriteComponent.h"
#include "TamaStates.h"
#include "StateComponent.h"
#include "TreeMeshComponent.h"
#include "TamaBlackEye.h"
#include "CollisionMapComponent.h"
#include "PlayerHome.h"
#include "TamaPointer.h"
#include "EnemyHome.h"

Tama::Tama(Game* game)
	: Enemy(game)
	, mState(nullptr)
	, mScale(1.0f)
	, mTc(nullptr)
	, mEye(nullptr)
	, mTp(nullptr)
{
	SetUp();
	mState = new StateComponent(this);
	mState->RegisterState(new TamaWait(mState));
	mState->RegisterState(new TamaMove(mState));
	mState->RegisterState(new TamaRockOn(mState));
	mState->RegisterState(new TamaSeache(mState));
	mState->RegisterState(new TamaCharge(mState));
	mState->RegisterState(new TamaAttack(mState));
	mState->ChangeState("Wait");
}

Tama::Tama(Game* game, const VECTOR& pos)
	: Enemy(game)
	, mState(nullptr)
	, mScale(1.0f)
	, mTc(nullptr)
	, mEye(nullptr)
	, mTp(nullptr)

{
	SetUp();
	mState = new StateComponent(this);
	mState->RegisterState(new TamaGenerate(mState));
	mState->RegisterState(new TamaWait(mState));
	mState->RegisterState(new TamaMove(mState));
	mState->RegisterState(new TamaRockOn(mState));
	mState->RegisterState(new TamaSeache(mState));
	mState->RegisterState(new TamaCharge(mState));
	mState->RegisterState(new TamaAttack(mState));
	mState->ChangeState("Generate");
	SetInitPosition(pos);
}

Tama::~Tama()
{
	mEye->SetState(EDead);
}

int Tama::SetUp()
{
	Data = GetGame()->GetAllData()->tamaData;

	SetHeight(Data.mHeight);
	SetRadius(Data.mRadius);
	SetHp(Data.mMaxHp);
	SetMaxHp(GetHp());
	SetInitMaxHp(GetMaxHp());
	SetAdvSpeed(Data.mAdvSpeed);
	SetJumpVel(Data.mJumpVelocity);
	SetJumpFlag(Data.mJumpFlag);
	SetImageColor(Data.mImageColor);
	SetGravity(Data.mGravity);
	SetCapsulOffset(Data.mCapsulOffset);
	SetTag(CharacterActor::Tama);

	new HpGaugeSpriteComponent(this, Data.mHpGaugeOffset);


	mTc = new TreeMeshComponent(this);
	mTc->SetTree("Tama");
	mTc->SetOffsetPos(GetCapsulOffset());
	SetNormalMesh(mTc);
	mTc = new TreeMeshComponent(this, false);
	mTc->SetTree("TamaDamage");
	mTc->SetOffsetPos(GetCapsulOffset());
	SetDamageMesh(mTc);

	mEye = new TamaBlackEye(this);
	new CollisionMapComponent(this);

	mTp = new TamaPointer(this);
	return 0;
}

void Tama::UpdateActor()
{
	SetScale(VECTOR(mScale, mScale, mScale));

	for (auto enemy : GetGame()->GetActorManager()->GetEnemies())
	{
		if (enemy->GetTag() == CharacterActor::Tama)
		{
			auto tama = static_cast<class Tama*>(enemy);
			if (tama->GetStateCompoState()->GetName() != "Generate" && mState->GetName() != "Generate")
			{
				Intersect(this, tama);
			}
		}
	}

	if (GetDamageInterval() > 0.0f)
	{
		SetDamageInterval(GetDamageInterval() - delta);
		DamageOption();
	}

	if (GetHp() <= 0)
	{
		mScale -= 0.01f;
		SetRotationY(GetRotation().y + 0.17f);
		mState->ChangeState("Wait");

		if (mScale <= 0.0f)
		{
			SetState(Actor::EDead);
		}
	}
}

void Tama::Damage(int damage)
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
}

void Tama::FallOption()
{
	GetGame()->GetActorManager()->GetStage()->GetLog()->AddText("Tamaが奈落に落ちた。");
	SetState(EDead);
}

void Tama::Dead()
{
	SpawnParticle(GetPosition(), "TamaTamaSphere", 10);
	SpawnParticle(GetPosition(), "TamaBlackEyeCylinder", 10);

	setVolume(mDeadSound, GetGame()->GetSoundVolumeManager()->GetEffectVolume());
	playSound(mDeadSound);
	GetGame()->GetActorManager()->GetStage()->SetClearCnt(GetGame()->GetActorManager()->GetStage()->GetClearCnt() - 1);
	DropItems(GetPosition());
	GetGame()->GetActorManager()->GetStage()->GetLog()->AddText("Tamaを倒した！！");
	if (GetGame()->GetActorManager()->GetPHome())
	{
		GetGame()->GetActorManager()->GetPHome()->SetBattlePoints(GetGame()->GetActorManager()->GetPHome()->GetBattlePoints() + 100 + GetLevel() * 50);
		if (GetGame()->GetActorManager()->GetPHome()->GetMaxBattlePoints() < GetGame()->GetActorManager()->GetPHome()->GetBattlePoints())
		{
			GetGame()->GetActorManager()->GetPHome()->SetBattlePoints(GetGame()->GetActorManager()->GetPHome()->GetMaxBattlePoints());
		}
	}

	if (GetGame()->GetActorManager()->GetEHome())
	{
		if (GetGame()->GetActorManager()->GetEHome()->GetLevel() > GetGame()->GetActorManager()->GetEHome()->GetTamaGenerateLevel() && GetGame()->GetActorManager()->GetEHome()->GetBattlePoints() >= 100)
		{
			GetGame()->GetActorManager()->GetEHome()->SetBattlePoints(GetGame()->GetActorManager()->GetEHome()->GetBattlePoints() - 100);
			GetGame()->GetActorManager()->GetEHome()->SetTamaGenerateLevel(GetGame()->GetActorManager()->GetEHome()->GetTamaGenerateLevel() + 1);
		}
	}
}

