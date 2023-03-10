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

Tama::Tama(Game* game)
	: Enemy(game)
	, mState(nullptr)
	, mScale(1.0f)
	, mTc(nullptr)
	, mEye(nullptr)
{
	SetUp();
	mState = new StateComponent(this);
	mState->RegisterState(new TamaWait(mState));
	mState->RegisterState(new TamaMove(mState));
	mState->RegisterState(new TamaChase(mState));
	mState->RegisterState(new TamaAttack(mState));
	mState->ChangeState("Wait");
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
	SetAdvSpeed(Data.mAdvSpeed);
	SetJumpVel(Data.mJumpVelocity);
	SetJumpFlag(Data.mJumpFlag);
	SetImageColor(Data.mImageColor);
	SetGravity(Data.mGravity);
	SetCapsulOffset(Data.mCapsulOffset);

	new HpGaugeSpriteComponent(this, Data.mHpGaugeOffset);

	mTc = new TreeMeshComponent(this);
	mTc->SetTree("Tama");
	mTc->SetDamageTree("TamaDamage");

	mEye = new TamaBlackEye(this);

	mTc->SetOffsetPos(GetCapsulOffset());
	return 0;
}

void Tama::UpdateActor()
{
	SetScale(VECTOR(mScale, mScale, mScale));

	for (auto enemy : GetGame()->GetEnemies())
	{
		if (enemy != this)
		{
			Intersect(this, enemy);
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
			GetGame()->GetStage()->GetLog()->AddText("Tamaを倒した！！");
			SetState(Actor::EDead);
		}
	}

}

void Tama::Damage(int damage)
{
	SetHp(GetHp() - damage);

	if (mState->GetName() == "Chace")
	{
		mState->ChangeState("Wait");
	}

	SetDamageInterval(Data.mMaxDamageInterval);
}

void Tama::FallOption()
{
	GetGame()->GetStage()->GetLog()->AddText("Tamaが奈落に落ちた。");
	SetState(EDead);
}

