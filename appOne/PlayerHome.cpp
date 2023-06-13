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
	, mGenerateCannonLevel(0)
	, mGenerateBarricadeLevel(0)
	, mCurMyTpIdx(0)
	, mCloseComplete(true)
	, mOpenComplete(false)
	, mBeginCloseFlag(false)
	, mBeginOpenFlag(true)
	, mGenerateFlag(true)
	, mBattlePoints(0)
	, mMaxBattlePoints(0)

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
	SetInitMaxHp(Data.mMaxHp);

	mDore->SetRotationX(0.0f);
	SetRadius(Data.mRadius);
	SetHeight(Data.mHeight);
	SetName("PlayerHome");
	new HpGaugeSpriteComponent(this, Data.mHpGaugeOffset);
	mUI = new UIPlayerHome(this);
	mMaxBattlePoints = 500;
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

	if (GetGame()->GetEHome())
	{
		VECTOR PEHomeCenterPos = (GetPosition() + GetGame()->GetEHome()->GetPosition()) / 2;

		mFieldTargetPoints[0] = PEHomeCenterPos + VECTOR(-7.0f, 0.0f, -7.0f);
		mFieldTargetPoints[1] = PEHomeCenterPos + VECTOR(7.0f, 0.0f, -7.0f);
		mFieldTargetPoints[2] = PEHomeCenterPos + VECTOR(7.0f, 0.0f, 7.0f);
		mFieldTargetPoints[3] = PEHomeCenterPos + VECTOR(-7.0f, 0.0f, 7.0f);
	}


	mMyTargetPoints[0] = VECTOR(0.0f, 0.0f, -3.0f);
	mMyTargetPoints[1] = VECTOR(0.0f, 0.0f, (-3.0f + (GetGame()->GetStage()->GetStageMaxZ() + GetGame()->GetStage()->GetStageMinZ())) / 4.0f);
	mMyTargetPoints[2] = VECTOR(0.0f, 0.0f, (-3.0f + (GetGame()->GetStage()->GetStageMaxZ() + GetGame()->GetStage()->GetStageMinZ())) / 2.0f);


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

	//if (mMoveCompleteFlag == false)
	{
		mMoveCompleteFlag = GoToTargetPoint(mHomeTargetPoint);

		if (mMoveCompleteFlag && mGenerateFlag)
		{
			if (mBeginOpenFlag && mCloseComplete)
			{
				OpenDore();
			}
			if (mBeginCloseFlag && mOpenComplete)
			{
				CloseDore();
			}
		}
		//mCloseComplete = CloseDore();

	}

	//OpenDore();

	int cnt = 0;
	for (auto Actor : GetGame()->GetPSide())
	{
		if (CollisionCircle(GetRadius(), Actor->GetRadius(), GetPosition(), Actor->GetPosition()))
		{
			cnt++;
		}
	}

	if (mGenerateFlag && cnt != 0)
	{
		Close();
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

bool PlayerHome::OpenDore()
{
	if (mDore->GetRotation().x <= 3.1415926f / 2)
	{
		mDore->SetRotationX(mDore->GetRotation().x + 0.017f);
		return false;
	}
	else
	{
		mBeginOpenFlag = false;
		mOpenComplete = true;
		mCloseComplete = false;
		Close();
		return true;
	}
}

bool PlayerHome::CloseDore()
{
	if (mDore->GetRotation().x >= 0.0f)
	{
		mDore->SetRotationX(mDore->GetRotation().x - 0.017f);
		return false;
	}
	else
	{
		mBeginCloseFlag = false;
		mCloseComplete = true;
		mOpenComplete = false;
		mGenerateFlag = false;
		return true;
	}
}
