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
	mState->RegisterState(new TamaWait(mState));
	mState->RegisterState(new TamaMove(mState));
	mState->RegisterState(new TamaRockOn(mState));
	mState->RegisterState(new TamaSeache(mState));
	mState->RegisterState(new TamaCharge(mState));
	mState->RegisterState(new TamaAttack(mState));
	mState->ChangeState("Wait");
	SetPosition(pos);
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
			GetGame()->GetStage()->GetLog()->AddText("Tama‚ğ“|‚µ‚½II");
			SetState(Actor::EDead);
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

