#include "Game.h"
#include "Cannon.h"
#include "CannonStates.h"
#include "window.h"
#include "Collision_Capsul.h"
#include "InputComponent.h"
#include "Items.h"
#include "UIItemStatus.h"
#include "input.h"
#include "TreeMeshComponent.h"
#include "CannonWheelL.h"
#include "CannonWheelR.h"
#include "UILog.h"
#include "Map.h"

Cannon::Cannon(Game* game) :
	CharacterActor(game)
	, mIn(nullptr)
	, mState(nullptr)
	, mPower(nullptr)
	, mSpeed(nullptr)
	, mBarrier(nullptr)
	, mRapid(nullptr)
	, mHpGauge(nullptr)
	, mScope(nullptr)
	, mUIItem(nullptr)
	, mJumpSoundFlag(false)
	, mCnt(0)
	, mSlideCnt(0)
	, mOnMap(false)
	, mScale(1.0f)
	, mLaunchTime(0.0f)
	, mWheelL(nullptr)
	, mWheelR(nullptr)
{
	SetUp();
}

Cannon::~Cannon()
{
	stopSound(Data.mJumpSound);
	delete mHpGauge;
	delete mScope;
	delete mUIItem;
	GetGame()->SetDisplayColor(GetGame()->GetDisplayColor());
	if (GetGame()->GetCamera())
	{
		GetGame()->GetCamera()->SetPosition(0.0f, 0.0f, 0.0f);
	}

	mWheelL->SetState(EDead);
	mWheelR->SetState(EDead);
	while (!mItemNums.empty())
	{
		mItemNums.pop_back();
	}

	stopSound(Data.mFallSound);
	GetGame()->SetCannon(nullptr);

}

int Cannon::SetUp()
{
	//Data
	Data = GetGame()->GetAllData()->cannonData;

	//StateMacine
	mState = new StateComponent(this);
	mState->RegisterState(new CannonWait(mState));
	mState->RegisterState(new CannonMove(mState));
	mState->RegisterState(new CannonJump(mState));
	mState->ChangeState("Wait");

	//基底クラスのデータにセット
	SetPosition(Data.mInitPos);
	SetRadius(Data.mCannonCapsulRadius);
	SetHeight(Data.mCannonCapsulHeight);
	SetHp(Data.mMaxHp);
	SetInitPosition(Data.mInitPos);
	SetGravity(Data.mGravity);
	SetJumpFlag(Data.mJumpFlag);
	SetJumpVel(Data.mJumpVelocity);
	SetAdvSpeed(Data.mAdvSpeed);
	SetCapsulOffset(Data.mCapsulOffset);
	SetImageColor(Data.mImageColor);

	SetRDamage(1);

	//UI
	mScope = new UIScope(GetGame(), this);
	mHpGauge = new UIHpGauge(GetGame(), this);
	mIn = new InputComponent(this);

	class TreeMeshComponent* bc = new TreeMeshComponent(this);
	bc->SetTree("CannonBarrel");
	bc->SetDamageTree("CannonBarrelDamage");
	bc->SetOffsetAngle(VECTOR(0.0f, 3.141592f, 0.0f));
	bc->SetOffsetPos(Data.mBodyOffsetPos + GetCapsulOffset());

	mWheelL = new CannonWheelL(this);
	mWheelR = new CannonWheelR(this);

	mUIItem = new UIItemStatus(this);

	//砲弾のナンバー0をあらかじめセットしておく
	mItemNums.emplace_back(0);

	return 0;
}

void Cannon::UpdateActor()
{
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

	//空中にいるときの処理
	if (GetJumpFlag() == 1)
	{
		mJumpSoundFlag = true;
	}

	if (GetGame()->GetScene() == Game::EPlay && GetGame()->GetState() == Game::EGameplay)
	{
		if (mIn->Jump() && GetJumpFlag() == 0)
		{
			setVolume(Data.mJumpSound, GetGame()->GetEffectVolume() + Data.mJumpSoundVolumeOffset);
			playSound(Data.mJumpSound);
		}
		if (GetJumpFlag() == 0 && mJumpSoundFlag)
		{
			setVolume(Data.mCyakuchi, GetGame()->GetEffectVolume());
			if (GetGame()->GetState() == Game::EGameplay)
			{
				playSound(Data.mCyakuchi);
				stopSound(Data.mFallSound);
				mJumpSoundFlag = false;
			}
		}
		if (mOnMap && mJumpSoundFlag)
		{
			setVolume(Data.mFallSound, GetGame()->GetEffectVolume() + Data.mFallSoundVolumeOffset);
			playSound(Data.mFallSound);
		}
	}

	//回転
	SetRotation(VECTOR(GetGame()->GetCamera()->GetRotation().x, GetGame()->GetCamera()->GetRotation().y + 3.14159264f, GetGame()->GetCamera()->GetRotation().z));

	//行列
	Master.identity();

	Master.mulTranslate(GetPosition() + GetCapsulOffset());
	Master.mulRotateY(GetRotation().y);
	Master.mulRotateY(3.141592f);

	Target.identity();
	Target.mulTranslate(0.0f, 0.0f, -Data.mRange);

	Body.identity();
	Body.mulTranslate(Data.mBodyOffsetPos);
	Body.mulScaling(mScale, mScale, mScale);
	Body.mulRotateX(-GetRotation().x);

	SetScale(VECTOR(mScale, mScale, mScale));

	if (GetGame()->GetScene() != Game::EPlay && GetGame()->GetScene() != Game::EQuit)
	{
		Data.mRDamage = 0;
	}

	if (GetGame()->GetScene() == Game::EStageClear)
	{
		while (!mItemComponents.empty())
		{
			delete mItemComponents.back();
		}
		mState->ChangeState("Wait");
	}

	for (auto enemy : GetGame()->GetEnemies())
	{
		if (enemy->GetHp() > 0)
		{
			Intersect(this, enemy);
		}
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
		SetHp(GetHp() - Data.mRDamage);

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
		if (actor)
		{
			static_cast<class Camera*>(GetGame()->GetCamera())->SetCannonKillerPos(actor->GetPosition());
		}
		setVolume(mDeadSound, GetGame()->GetEffectVolume());
		playSound(mDeadSound);
		GetGame()->GetStage()->AddText("Cannonは死んでしまった...。");
		SetState(Actor::EDead);
	}
}

void Cannon::DamageOption()
{
	if (GetDamageInterval() > 0)
	{
		if (Data.mRDamage != 0)
		{
			GetGame()->SetDisplayColor(GetDamageColor());
			setVolume(Data.mDamageSound, GetGame()->GetEffectVolume());
			playSound(Data.mDamageSound);
		}
		else
		{
			GetGame()->SetDisplayColor(Data.mGurdColor);
			setVolume(Data.mGurdSound, GetGame()->GetEffectVolume());
			playSound(Data.mGurdSound);
		}
	}
	else
	{
		GetGame()->SetDisplayColor(Data.mDisplayColor);
	}
}

void Cannon::FallOption()
{
	SetJumpVel(0.0f);
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

void Cannon::AddItemComponent(ItemComponent* component)
{
	mItemComponents.emplace_back(component);
}

void Cannon::RemoveItemComponent(ItemComponent* component)
{
	auto iter = std::find(mItemComponents.begin(), mItemComponents.end(), component);
	if (iter != mItemComponents.end())
	{
		mItemComponents.erase(iter);
	}
}



