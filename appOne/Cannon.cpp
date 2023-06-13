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
#include "CollisionMapComponent.h"
#include "PlayerHome.h"
#include "HpGaugeSpriteComponent.h"
#include <iostream>
#include <sstream>
#include "UIPSideCharacterStatusClose.h"

Cannon::Cannon(Game* game) :
	PSideCharacterActor(game)
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
	, mRange(0.0f)
	, mMoveState(Stay)
	, mTPIndex(0)
	, mCNum(0)
{
	//SetUp();
	GetGame()->AddCannon(this);
}

Cannon::~Cannon()
{
	stopSound(Data.mJumpSound);

	/*if (this == GetGame()->GetCannon())
	{
		delete mHpGauge;
		delete mScope;
		delete mUIItem;
	}*/

	GetGame()->SetDisplayColor(GetGame()->GetDisplayColor());

	mWheelL->SetState(EDead);
	mWheelR->SetState(EDead);

	while (!mItemNums.empty())
	{
		mItemNums.pop_back();
	}

	if (this == GetGame()->GetCannon())
	{
		GetGame()->SetCannon(nullptr);
	}

	stopSound(Data.mFallSound);
	GetGame()->RemoveCannon(this);

	for (auto cannon : GetGame()->GetCannons())
	{
		if (cannon->GetCNum() > GetCNum())
		{
			cannon->SetCNum(cannon->GetCNum() - 1);
		}
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
	mState->RegisterState(new CannonJump(mState));
	mState->RegisterState(new CannonMoveReturnHome(mState));
	mState->RegisterState(new CannonMoveHomePatroll(mState));
	mState->RegisterState(new CannonMoveFieldPatroll(mState));
	mState->ChangeState("Generate");

	//基底クラスのデータにセット
	SetTag(CharacterActor::Cannon);
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
	SetInitMaxHp(GetMaxHp());


	new HpGaugeSpriteComponent(this, GetCapsulOffset());
	new UIPSideCharacterStatusClose(this);

	mIn = new InputComponent(this);

	class TreeMeshComponent* bc = new TreeMeshComponent(this);
	bc->SetTree("CannonBarrel");
	bc->SetDamageTree("CannonBarrelDamage");
	bc->SetOffsetAngle(VECTOR(0.0f, 3.141592f, 0.0f));
	bc->SetOffsetPos(Data.mBodyOffsetPos + GetCapsulOffset());

	mWheelL = new CannonWheelL(this);
	mWheelR = new CannonWheelR(this);

	new CollisionMapComponent(this);

	//砲弾のナンバー0をあらかじめセットしておく
	mItemNums.emplace_back(0);

	mCNum = (int)(GetGame()->GetCannons().size()) - 1;

	std::ostringstream oss;
	oss << "Cannon" << mCNum;

	SetName(oss.str().c_str());
	return 0;
}

void Cannon::ActorInput()
{
	if (this == GetGame()->GetCannon())
	{
		if (isTrigger(KEY_W))
		{
			SetMoveState(Stay);
		}
		else if (isTrigger(KEY_R))
		{
			SetMoveState(Return);
		}
		else if (isTrigger(KEY_H))
		{
			SetMoveState(HomePatroll);
		}
		else if (isTrigger(KEY_F))
		{
			SetMoveState(FieldPatroll);
		}
	}
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

	////空中にいるときの処理
	//if (GetJumpFlag() == 1)
	//{
	//	mJumpSoundFlag = true;
	//}

	//if (GetGame()->GetScene() == Game::EPlay && GetGame()->GetState() == Game::EGameplay)
	//{
	//	if (GetJumpFlag() == 0)
	//	{
	//		setVolume(Data.mJumpSound, GetGame()->GetEffectVolume() + Data.mJumpSoundVolumeOffset);
	//		playSound(Data.mJumpSound);
	//	}
	//	if (GetJumpFlag() == 0 && mJumpSoundFlag)
	//	{
	//		setVolume(Data.mCyakuchi, GetGame()->GetEffectVolume());
	//		if (GetGame()->GetState() == Game::EGameplay)
	//		{
	//			playSound(Data.mCyakuchi);
	//			stopSound(Data.mFallSound);
	//			mJumpSoundFlag = false;
	//		}
	//	}
	//	if (mOnMap && mJumpSoundFlag)
	//	{
	//		setVolume(Data.mFallSound, GetGame()->GetEffectVolume() + Data.mFallSoundVolumeOffset);
	//		playSound(Data.mFallSound);
	//	}
	//}

	//回転
	//SetRotation(VECTOR(GetGame()->GetCamera()->GetRotation().x, GetGame()->GetCamera()->GetRotation().y + 3.14159264f, GetGame()->GetCamera()->GetRotation().z));

	//SetRotationY(3.415926f);
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
		if (enemy->GetHp() > 0 && mState->GetName() != "Generate")
		{
			Intersect(this, enemy);
		}
	}

	for (auto pSide : GetGame()->GetCannons())
	{
		if (pSide != this && mState->GetName() != "Generate" && pSide->GetStateCompoState()->GetName() != "Generate")
		{
			Intersect(this, pSide);
		}
	}

	if (GetDamageInterval() > 0.0f)
	{
		SetDamageInterval(GetDamageInterval() - delta);
		DamageOption();
	}

	//	print("Target(" + (let)Data.mTargetPos.x + "," + (let)Data.mTargetPos.y + "," + (let)Data.mTargetPos.z + ")");
		//print(GetRange());
		//print("posY :" + (let)GetPosition().y);
		/*VECTOR2 textPos = VECTOR2((GetPosition().x / (GetGame()->GetStage()->GetStageMaxX() + -1.0f * GetGame()->GetStage()->GetStageMinX())) * 1920.0f * 100.0f, (GetPosition().z / (GetGame()->GetStage()->GetStageMaxZ() + -1.0f * GetGame()->GetStage()->GetStageMinZ())) * 1080.0f * 100.0f);
	*/
	//fill(0.0f, 0.0f, 0.0f);
	float t = (GetPosition().x / (GetGame()->GetStage()->GetStageMaxX() + -1.0f * GetGame()->GetStage()->GetStageMinX())) * width;
	float k = -1 * (GetPosition().z / (GetGame()->GetStage()->GetStageMaxZ() + -1.0f * GetGame()->GetStage()->GetStageMinZ())) * height;
	float tt = width / t + t;
	float kk = height / 2 - k;


	//if (this == GetGame()->GetCannon())
	{
		//	text(k, width / 2, height / 2);

	}

	float tSize = 30.0f;
	textSize(tSize);
	float cPosx = (GetGame()->GetStage()->GetStageMaxX() + GetGame()->GetStage()->GetStageMinX()) / 2.0f;
	float cPosz = (GetGame()->GetStage()->GetStageMaxZ() + GetGame()->GetStage()->GetStageMinZ()) / 2.0f;

	float p = (cPosx - GetPosition().x) / 9.0f;
	float s = 340.0f + 260.0f;
	float r = height - 520.0f;
	float indispMapz = 60.0f;
	float h = (cPosz - GetPosition().z) / (indispMapz / 2.0f);
	float camx = (cPosx - GetGame()->GetCamera()->GetPosition().x) / 9.0f * 340.0 - 340.0f / (cPosx - GetGame()->GetCamera()->GetPosition().x);
	float camz = (cPosz - GetGame()->GetCamera()->GetPosition().z) / indispMapz / 2 * height + height / (cPosz - GetGame()->GetCamera()->GetPosition().z);
	float camy = GetGame()->GetCamera()->GetPosition().y / 90.0f;
	//  print("p(" + (let)p + ") h(" + (let)h + ")");x
	float x = p * (1.0f - h);
	float px = (s * p + 790.0f) - tSize / 2.0f * 3.0f - camx + 200.0f * (p + h) * cos(GetGame()->GetCamera()->GetRotation().y) - 50.0f * (p)-150.0f * (1 - h);
	float py = height - (h * r) - tSize / 2.0f - camz + height + 600.0f * (p + h) * sin(GetGame()->GetCamera()->GetRotation().x) - 150.0f * p + (1 - h) * 250.0f;
	s = 340.0f / 2.0f;
	r = height / 2.0f;

	float defx = s + 790.0f - tSize / 2.0f * 3.0f;
	px = (s * (1.0f - p)) + camx;
	if (h < 0.0f)
	{
		r = 720.0f;
	}
	else
	{
		r = 320.0f;
	}

	py = -(h * r) + tSize / 2.0f + camz + ((1 - h) * sin(GetGame()->GetCamera()->GetRotation().x) * r);

	//	py = height / 2 - tSize / 2.0f;

	MATRIX mat;
	mat.identity();
	mat.mulTranslate(defx, 0.0f, height);
	mat.mulRotateX(-GetGame()->GetCamera()->GetRotation().x);
	mat.mulRotateY(-GetGame()->GetCamera()->GetRotation().y);
	mat.mulTranslate(px, 0.0f, py);

	px = mat._14;
	py = mat._34;

	px -= 340.0f * p - 340.0f;
	py -= 420.0f * (1.0f - h);

	text(GetName().c_str(), px, py);

	//print("CurTp :" + (let)mTPIndex + "NextTp :" + (let)GetNextTpIndex());
	//print("(" + (let)px + "," + (let)py + ")");
	//print(GetGame()->GetCamera()->GetRotation().x);
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
			//static_cast<class Camera*>(GetGame()->GetCamera())->SetCannonKillerPos(actor->GetPosition());
		}
		setVolume(mDeadSound, GetGame()->GetEffectVolume());
		playSound(mDeadSound);
		GetGame()->GetStage()->AddText("Cannonは死んでしまった...。");
		SetState(Actor::EDead);
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

		SetHp(GetHp() - Data.mRDamage);
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
		GetGame()->GetStage()->AddText("Cannonは死んでしまった...。");
		SetState(Actor::EDead);
	}
}

void Cannon::DamageOption()
{
	if (this == GetGame()->GetCannon())
	{
		if (GetDamageInterval() > 0)
		{
			if (Data.mRDamage != 0)
			{
				//GetGame()->SetDisplayColor(GetDamageColor());
				setVolume(Data.mDamageSound, GetGame()->GetEffectVolume());
				playSound(Data.mDamageSound);
			}
			else
			{
				//GetGame()->SetDisplayColor(Data.mGurdColor);
				setVolume(Data.mGurdSound, GetGame()->GetEffectVolume());
				playSound(Data.mGurdSound);
			}
		}
		else
		{
			GetGame()->SetDisplayColor(Data.mDisplayColor);
		}
	}
	else
	{
		if (GetDamageInterval() > 0)
		{
			setVolume(Data.mDamageSound, GetGame()->GetEffectVolume());
			playSound(Data.mDamageSound);
		}
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

int Cannon::GetNextTpIndex()
{
	int idx = (mTPIndex + 1) % 4;
	return idx;
}


