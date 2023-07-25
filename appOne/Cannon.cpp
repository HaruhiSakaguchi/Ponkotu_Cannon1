#include "Game.h"
#include "Cannon.h"
#include "CannonStates.h"
#include "window.h"
#include "Collision_Capsul.h"
#include "InputComponent.h"
#include "UIItemStatus.h"
#include "TreeMeshComponent.h"
#include "CannonWheelL.h"
#include "CannonWheelR.h"
#include "UILog.h"
#include "Map.h"
#include "CollisionMapComponent.h"
#include <sstream>
#include "UIPSideCharacterStatusClose.h"
#include "CameraManager.h"
#include "input.h"
#include "CapsuleComponent.h"

Cannon::Cannon(class Game* game)
	: PSideCharacterActor(game)
	, mIn(nullptr)
	, mState(nullptr)
	, mJumpSoundFlag(false)
	, mCnt(0)
	, mSlideCnt(0)
	, mOnMap(false)
	, mScale(1.0f)
	, mLaunchTime(0.0f)
	, mWheelL(nullptr)
	, mWheelR(nullptr)
	, mRange(0.0f)
	, mMoveState(MoveState::EStay)
	, mTPIndex(0)
	, mCNum(0)
{
	//SetUp();
	GetGame()->GetActorManager()->AddCannon(this);
}

Cannon::~Cannon()
{

	GetGame()->GetActorManager()->RemoveCannon(this);

	while (!mItemNums.empty())
	{
		mItemNums.pop_back();
	}

	
}

int Cannon::SetUp()
{
	//Data
	Data = GetGame()->GetAllData()->cannonData;

	//StateMacine
	mState = new StateComponent(this);
	mState->RegisterState(new CannonGenerate(mState));
	mState->RegisterState(new CannonWait(mState));
	mState->RegisterState(new CannonLaunch(mState));
	mState->RegisterState(new CannonMove(mState));
	mState->RegisterState(new CannonRotate(mState));
	mState->RegisterState(new CannonMoveReturnHome(mState));
	mState->RegisterState(new CannonMoveHomePatroll(mState));
	mState->RegisterState(new CannonMoveFieldPatroll(mState));
	mState->ChangeState("Generate");

	//基底クラスのデータにセット
	SetTag(CharacterActor::CharactersTag::ECannon);
	SetPosition(Data.mInitPos);
	SetRadius(Data.mCannonCapsulRadius);
	SetHeight(Data.mCannonCapsulHeight);
	SetHp(Data.mMaxHp);
	SetMaxHp(GetHp());
	SetInitPosition(Data.mInitPos);
	SetGravity(Data.mGravity);
	SetJumpFlag(Data.mJumpFlag);
	SetJumpVel(Data.mJumpVelocity);
	SetAdvSpeed(Data.mAdvSpeed);
	SetCapsulOffset(Data.mCapsulOffset);
	SetImageColor(Data.mImageColor);
	SetRange(Data.mRange);
	SetRDamage(1);
	SetDamage(1);
	SetInitMaxHp(GetMaxHp());
	//new HpGauge2DSpriteComponent(this);

	auto ui = new UIPSideCharacterStatusClose(this);
	ui->UIOpenNexrOffsetPlus();

	mIn = new InputComponent(this);

	auto bc = new TreeMeshComponent(this, false);
	bc->SetTree("CannonBarrel");
	bc->SetOffsetAngle(VECTOR(0.0f, 3.141592f, 0.0f));
	bc->SetOffsetPos(Data.mBodyOffsetPos + GetCapsulOffset());
	SetNormalMesh(bc);

	bc = new TreeMeshComponent(this, false);
	bc->SetTree("CannonBarrelDamage");
	bc->SetOffsetAngle(VECTOR(0.0f, 3.141592f, 0.0f));
	bc->SetOffsetPos(Data.mBodyOffsetPos + GetCapsulOffset());
	SetDamageMesh(bc);


	mWheelL = new CannonWheelL(this);
	mWheelR = new CannonWheelR(this);

	new CollisionMapComponent(this);

	//砲弾のナンバー0をあらかじめセットしておく
	mItemNums.emplace_back(0);

	mCNum = (int)(GetGame()->GetActorManager()->GetCannons().size()) - 1;

	for (auto cannon : GetGame()->GetActorManager()->GetCannons())
	{
		if (cannon != this && cannon->GetCNum() == mCNum)
		{
			mCNum++;
		}
	}

	std::ostringstream oss;
	oss << "Cannon" << mCNum;
	mCapsule = new CapsuleComponent(this);
	mCapsule->SetIsCollision(false);
	mCapsule->AddNotCollisionTags((int)CharacterActor::CharactersTag::EPHome);
	mCapsule->AddNotCollisionTags((int)CharacterActor::CharactersTag::EBarricade);
	mCapsule->AddNotCollisionTags((int)CharacterActor::CharactersTag::EEHome);

	SetName(oss.str().c_str());

	oss << "[Lv." << GetLevel() << "]" << "が出撃。";
	GetGame()->GetActorManager()->GetStage()->GetLog()->AddText(oss.str());

	return 0;
}

void Cannon::UpdateActor()
{
	if (isTrigger(KEY_A)) { SetPosition(GetPosition() + VECTOR(-1.0f, 0.0f, 0.0f)); }
	if (isTrigger(KEY_D)) { SetPosition(GetPosition() + VECTOR(1.0f, 0.0f, 0.0f)); }
	if (isTrigger(KEY_W)) { SetPosition(GetPosition() + VECTOR(0.0f, 0.0f, -1.0f)); }
	if (isTrigger(KEY_S)) { SetPosition(GetPosition() + VECTOR(0.0f, 0.0f, 1.0f)); }

	float preScale = mScale;
	float Scale = preScale;
	mOnMap = GetOnMapFlag();
	if (mLaunchTime > 0.0f)
	{
		mLaunchTime -= delta * Data.mLaunchTimeDecreaseAdvSpeed;
		Scale = Data.mLaunchBodyScale;
	}
	else
	{
		if (mLaunchTime < 0.0f)
		{
			mLaunchTime = 0.0f;
		}
		Scale = Data.mNormalBodyScale;
	}

	mScale = preScale + (Scale - preScale) * Data.mChangeScaleSpeed;

	if (mScale < 0.0f)
	{
		mScale = Data.mNormalBodyScale;
	}
	if (mState->GetName() != "Generate")
	{
		mCapsule->SetIsCollision(true);
	}

	//行列
	Master.identity();

	Master.mulTranslate(GetPosition());
	Master.mulRotateY(GetRotation().y);
	Master.mulRotateY(3.141592f);

	Target.identity();
	Target.mulTranslate(0.0f, 0.0f, -GetRange());

	Body.identity();
	Body.mulTranslate(Data.mBodyOffsetPos);
	Body.mulScaling(mScale, mScale, mScale);
	Body.mulRotateX(-GetRotation().x);

	SetScale(VECTOR(mScale, mScale, mScale));

	if (GetGame()->GetActorManager()->GetEnemies().empty() && !GetGame()->GetActorManager()->GetEHome())
	{
		Data.mRDamage = 0;
		mState->ChangeState("Wait");
	}

	if (GetDamageInterval() > 0.0f)
	{
		SetDamageInterval(GetDamageInterval() - delta);
		DamageOption();
	}

}

const VECTOR& Cannon::GetTargetPosition()
{
	Target = Master * Body * Target;
	Data.mTarGetPos = VECTOR(Target._14, Target._24, Target._34);
	return Data.mTarGetPos;
}

void Cannon::Damage(Actor* actor)
{
	if (GetDamageInterval() <= 0.0f)
	{
		SetHp(GetHp() - GetRDamage());

		if (GetBarrier())
		{
			GetBarrier()->SetHp(GetBarrier()->GetHp() - 1);
		}

		SetDamageInterval(Data.mMaxDamageInterval);
	}
	else
	{
		return;
	}

	if (GetHp() <= 0)
	{
		setVolume(mDeadSound, GetGame()->GetSoundVolumeManager()->GetEffectVolume());
		playSound(mDeadSound);
		SetState(Actor::State::EDead);
	}
}

void Cannon::Damage(int damage)
{
	if (GetDamageInterval() <= 0.0f)
	{
		if (GetBarrier())
		{
			GetBarrier()->SetHp(GetBarrier()->GetHp() - 1);
		}

		SetHp(GetHp() - GetRDamage());
		SetDamageInterval(Data.mMaxDamageInterval);
	}
	else
	{
		return;
	}

	if (GetHp() <= 0)
	{
		SetState(Actor::State::EDead);
	}
}

void Cannon::DamageOption()
{
	if (GetDamageInterval() > 0)
	{
		if (GetRDamage() != 0)
		{
			setVolume(Data.mDamageSound, GetGame()->GetSoundVolumeManager()->GetEffectVolume());
			playSound(Data.mDamageSound);
		}
		else
		{
			setVolume(Data.mGurdSound, GetGame()->GetSoundVolumeManager()->GetEffectVolume());
			playSound(Data.mGurdSound);
		}
	}
}

void Cannon::FallOption()
{
	SetJumpVel(0.0f);
}

void Cannon::Dead()
{
	PSideCharacterActor::Dead();

	stopSound(Data.mJumpSound);
	stopSound(Data.mFallSound);

	for (auto cannon : GetGame()->GetActorManager()->GetCannons())
	{
		if (cannon->GetCNum() > GetCNum())
		{
			cannon->SetCNum(cannon->GetCNum() - 1);
		}
	}

	SpawnParticle(GetGame(), GetPosition(), "CannonBarrelBarrel", 10);
	SpawnParticle(GetGame(), GetPosition(), "CannonWheelCylinder", 20);
	setVolume(mDeadSound, GetGame()->GetSoundVolumeManager()->GetEffectVolume());
	playSound(mDeadSound);
	GetGame()->GetActorManager()->GetStage()->GetLog()->AddText("Cannonは死んでしまった...。");
}


void Cannon::AddItemNum(int num)
{
	mItemNums.emplace_back(num);
	//6個以上は保存しない
	if (Data.mItemStockMaxSize < (int)mItemNums.size())
	{
		//0は砲弾なので除外から除外
		//ランダムで一つ取り除く
		int i = 0;
		while (i == 0)
		{
			i = rand() % Data.mItemStockMaxSize;
		}
		RemoveItemNum(i);
	}
}

void Cannon::RemoveItemNum(int num)
{
	//num番目のナンバーを取り除く
	auto iter = std::find(mItemNums.begin(), mItemNums.end(), mItemNums[num]);
	if (iter != mItemNums.end())
	{
		mItemNums.erase(iter);
	}
}

int Cannon::GetNextTpIndex()
{
	int idx = (mTPIndex + 1) % 4;
	return idx;
}
