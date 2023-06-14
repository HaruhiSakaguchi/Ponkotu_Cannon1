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

Tama::Tama(Game* game,const VECTOR&pos)
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
	//SetPosition(pos);
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
	mTc->SetDamageTree("TamaDamage");

	mEye = new TamaBlackEye(this);
	new CollisionMapComponent(this);

	mTc->SetOffsetPos(GetCapsulOffset());
	mTp = new TamaPointer(this);
	return 0;
}

void Tama::UpdateActor()
{
	SetScale(VECTOR(mScale, mScale, mScale));

	for (auto enemy : GetGame()->GetEnemies())
	{
		if (enemy != this && enemy->GetTag() == CharacterActor::Satellite)
		{
			auto satellite = static_cast<class Satellite*>(enemy);
			if (satellite->GetStateCompoState()->GetName() != "Generate" && mState->GetName() != "Generate")
			{
				Intersect(this, satellite);
			}
		}
		else if (enemy->GetTag() == CharacterActor::Tama)
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
			setVolume(mDeadSound, GetGame()->GetEffectVolume());
			playSound(mDeadSound);
			GetGame()->GetStage()->SetClearCnt(GetGame()->GetStage()->GetClearCnt() - 1);
			DropItems(GetPosition());
			GetGame()->GetStage()->GetLog()->AddText("Tama‚ğ“|‚µ‚½II");
			SetState(Actor::EDead);
			if (GetGame()->GetPHome())
			{
				GetGame()->GetPHome()->SetBattlePoints(GetGame()->GetPHome()->GetBattlePoints() + 100 + GetLevel() * 50);
			}

			if (GetGame()->GetEHome())
			{
				if (GetGame()->GetEHome()->GetLevel() > GetGame()->GetEHome()->GetTamaGenerateLevel() && GetGame()->GetEHome()->GetBattlePoints() >= 100)
				{
					GetGame()->GetEHome()->SetBattlePoints(GetGame()->GetEHome()->GetBattlePoints() - 100);
					GetGame()->GetEHome()->SetTamaGenerateLevel(GetGame()->GetEHome()->GetTamaGenerateLevel() + 1);
				}
			}
		}
	}
}

void Tama::Damage(int damage)
{
	SetHp(GetHp() - damage);
	SetDamageInterval(Data.mMaxDamageInterval);
}

void Tama::FallOption()
{
	GetGame()->GetStage()->GetLog()->AddText("Tama‚ª“Ş—‚É—‚¿‚½B");
	SetState(EDead);
}

