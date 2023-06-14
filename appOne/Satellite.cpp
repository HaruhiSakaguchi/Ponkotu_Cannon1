#include "Game.h"
#include "COLLISION_MAP.h"
#include "SatelliteStates.h"
#include "Map.h"
#include "window.h"
#include "TreeMeshComponent.h"
#include "UILog.h"
#include "CollisionMapComponent.h"
#include "CharacterActor.h"
#include "PlayerHome.h"
#include "EnemyHome.h"

int Satellite::Num = 0;

Satellite::Satellite(class Game* game)
	: Enemy(game)
	, mHpGauge(nullptr)
	, mState(nullptr)
	, mDeadFlag(false)
	, mRange(0.0f)
{
	SetUp();
	mState = new StateComponent(this);
	mState->RegisterState(new SatelliteNormal(mState));
	mState->RegisterState(new SatelliteGenerate(mState));
	mState->RegisterState(new SatelliteMove(mState));
	mState->RegisterState(new SatelliteRockOn(mState));
	mState->RegisterState(new SatelliteAttack(mState));
	mState->ChangeState("Generate");
}

Satellite::Satellite(class Game* game, const VECTOR& pos)
	: Enemy(game)
	, mHpGauge(nullptr)
	, mState(nullptr)
	, mDeadFlag(false)
	, mRange(0.0f)
{
	SetUp();
	SetInitPosition(pos);
	SetPosition(GetInitPosition());
	mState = new StateComponent(this);
	mState->RegisterState(new SatelliteNormal(mState));
	mState->RegisterState(new SatelliteGenerate(mState));
	mState->RegisterState(new SatelliteMove(mState));
	mState->RegisterState(new SatelliteRockOn(mState));
	mState->RegisterState(new SatelliteAttack(mState));
	mState->ChangeState("Generate");

}

Satellite::~Satellite()
{
	while (!mWings.empty())
	{
		delete mWings.back();
	}
}

int Satellite::SetUp()
{
	Data = GetGame()->GetAllData()->satelliteData;
	SetHeight(Data.mHeight);
	SetRadius(Data.mRadius);
	SetAdvSpeed(Data.mAdvSpeed);
	SetJumpFlag(1);
	SetRange(Data.mMaxRange);
	SetTag(CharacterActor::Satellite);

	Data.mId = Num % 2;
	int maxHp = 0;
	class TreeMeshComponent* tc = new TreeMeshComponent(this);
	if (Data.mId == 0)
	{
		maxHp = Data.mMaxHp;
		tc->SetTree("SatelliteBody0");
		tc->SetDamageTree("SatelliteBody0Damage");
	}
	else
	{
		maxHp = Data.mMaxHp2;
		SetCapsulOffset(Data.mCapsulOffset);
		tc->SetOffsetPos(VECTOR(0.0f, 0.75f, 0.0f));
		tc->SetTree("SatelliteBody1");
		tc->SetDamageTree("SatelliteBody1Damage");
	}


	SetHp(maxHp);
	SetMaxHp(GetHp());
	SetInitMaxHp(GetMaxHp());

	Num++;
	mHpGauge = new HpGaugeSpriteComponent(this, Data.mHpGaugeOffset);

	new SatelliteWing(this);
	new SatelliteWing(this);
	new SatelliteWing(this);
	new SatelliteWing(this);

	SetImageColor(Data.mImageColor);
	return 0;
}

void Satellite::UpdateActor()
{
	if (GetPosition().y > 10.0f)
	{
		SetHp(0);
	}

	if (GetHp() <= 0 && mDeadFlag == false)
	{
		mDeadFlag = true;
		new CollisionMapComponent(this);
	}

	Master.identity();
	Master.mulTranslate(GetPosition());
	Master.mulRotateY(GetRotation().y);

	if (Data.mId == 1)
	{
		SetRotationX(3.1415926f / 2);
	}

	//Master.mulRotateX(GetRotation().x);

	Target.identity();
	Target.mulTranslate(VECTOR(0.0f, 0.0f, 3.0f));

	Target = Master * Target;

	if (GetDamageInterval() > 0.0f)
	{
		SetDamageInterval(GetDamageInterval() - delta);
		DamageOption();
	}

	if (mDeadFlag == true)
	{
		mState->ChangeState("Normal");
		if (GetPosition().y <= 0.0f)
		{
			setVolume(mDeadSound, GetGame()->GetEffectVolume());
			playSound(mDeadSound);
			GetGame()->GetStage()->SetClearCnt(GetGame()->GetStage()->GetClearCnt() - 1);
			DropItems(Data.mDeadPoint);
			if (Data.mId == 0)
			{
				GetGame()->GetStage()->GetLog()->AddText("SatelliteAを倒した！！");
				if (GetGame()->GetPHome())
				{
					GetGame()->GetPHome()->SetBattlePoints(GetGame()->GetPHome()->GetBattlePoints() + 150 + GetLevel() * 50);
				}
			}
			else
			{
				GetGame()->GetStage()->GetLog()->AddText("SatelliteBを倒した！！");
				if (GetGame()->GetPHome())
				{
					GetGame()->GetPHome()->SetBattlePoints(GetGame()->GetPHome()->GetBattlePoints() + 175 + GetLevel() * 50);
				}
			}
			SetState(Actor::EDead);

			if (GetGame()->GetEHome())
			{
				if (GetGame()->GetEHome()->GetLevel() > GetGame()->GetEHome()->GetSatelliteGenerateLevel() && GetGame()->GetEHome()->GetBattlePoints() >= 150)
				{
					GetGame()->GetEHome()->SetBattlePoints(GetGame()->GetEHome()->GetBattlePoints() - 150);
					GetGame()->GetEHome()->SetSatelliteGenerateLevel(GetGame()->GetEHome()->GetSatelliteGenerateLevel() + 1);
				}
			}

		}
	}

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

	//print("(" + (let)GetTargetPosition().x + "," + (let)GetTargetPosition().y + "," + (let)GetTargetPosition().z + ")");
}

const VECTOR& Satellite::GetTargetPosition()
{
	Data.mTargetPos = VECTOR(Target._14, Target._24, Target._34);
	return Data.mTargetPos;
}

void Satellite::Damage(int damage)
{
	SetHp(GetHp() - damage);
	if (GetHp() <= 0)
	{
		Data.mDeadPoint = GetPosition();
		SetJumpFlag(1);
		SetGravity(Data.mGravity);
	}
	SetDamageInterval(Data.mMaxDamageInterval);
	if (mState->GetName() == "Attack")
	{
		mState->ChangeState("Normal");
		return;
	}
}


void Satellite::AddWings(SatelliteWing* wing)
{
	mWings.emplace_back(wing);
}

void Satellite::RemoveWings(SatelliteWing* wing)
{
	auto iter = std::find(mWings.begin(), mWings.end(), wing);
	if (iter != mWings.end())
	{
		mWings.erase(iter);
	}
}