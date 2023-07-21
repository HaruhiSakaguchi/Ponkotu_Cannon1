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
#include "CapsuleComponent.h"

int Satellite::mNum = 0;

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

	Data.mId = mNum % 2;
	int maxHp = 0;
	auto ntc = new TreeMeshComponent(this, false);

	if (Data.mId == 0)
	{
		maxHp = Data.mMaxHp;
		ntc->SetTree("SatelliteBody0");
	}
	else
	{
		maxHp = Data.mMaxHp2;
		SetCapsulOffset(Data.mCapsulOffset);
		ntc->SetOffsetPos(VECTOR(0.0f, 0.75f, 0.0f));
		ntc->SetTree("SatelliteBody1");
	}

	SetNormalMesh(ntc);
	
	SetHp(maxHp);
	SetMaxHp(GetHp());
	SetInitMaxHp(GetMaxHp());

	mNum++;

	new SatelliteWing(this);
	new SatelliteWing(this);
	new SatelliteWing(this);
	new SatelliteWing(this);

	SetImageColor(Data.mImageColor);
	mCapsule = new CapsuleComponent(this);
	mCapsule->SetIsCollision(false);
	mCapsule->SetSpandEp(VECTOR(0.0f, 0.0f, GetHeight()), VECTOR(0.0f, 0.0f, -GetHeight()));
	mCapsule->AddNotCollisionTags(PHome);
	mCapsule->AddNotCollisionTags(EHome);
	mCapsule->AddNotCollisionTags(Barricade);
	std::ostringstream oss;

	const char* type = nullptr;
	if (Data.mId == 0)
	{
		type = "SatelliteA";
	}
	else
	{
		type = "SatelliteB";
	}

	oss << type << (int)(mNum / 2);

	SetName(oss.str().c_str());

	return 0;
}

void Satellite::UpdateActor()
{
	if (GetPosition().y >= 10.0f)
	{
		SetHp(0);
	}

	if (mState->GetName() != "Generate")
	{
		mCapsule->SetIsCollision(true);
	}

	if (GetHp() <= 0 && mDeadFlag == false)
	{
		mDeadFlag = true;
		auto Collision = new CollisionMapComponent(this);
		Collision->NoCollisionMap();
		SetJumpFlag(1);
		SetGravity(Data.mGravity);
		Data.mDeadPoint = GetPosition();
	}

	Master.identity();
	Master.mulTranslate(GetPosition());
	Master.mulRotateY(GetRotation().y);

	if (Data.mId == 1)
	{
		SetRotationX(3.1415926f / 2);
	}

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
			SetState(Actor::EDead);
		}
	}
}

const VECTOR& Satellite::GetTargetPosition()
{
	Data.mTargetPos = VECTOR(Target._14, Target._24, Target._34);
	return Data.mTargetPos;
}

void Satellite::Damage(int damage)
{
	if (GetDamageInterval() <= 0)
	{
		SetHp(GetHp() - damage);
		SetDamageInterval(Data.mMaxDamageInterval);
	}
	else
	{
		return;
	}

	if (mState->GetName() == "Attack")
	{
		mState->ChangeState("Normal");
		return;
	}
}

void Satellite::Dead()
{
	if (Data.mId == 0)
	{
		SpawnParticle(GetGame(), GetPosition(), "SatelliteBody0Cylinder", 10);
		SpawnParticle(GetGame(), GetPosition(), "SatelliteWing0Square", 40);
	}
	else
	{
		SpawnParticle(GetGame(), GetPosition(), "SatelliteBody1Cylinder", 10);
		SpawnParticle(GetGame(), GetPosition(), "SatelliteWing1Square", 40);
	}


	setVolume(mDeadSound, GetGame()->GetSoundVolumeManager()->GetEffectVolume());
	playSound(mDeadSound);
	DropItems(Data.mDeadPoint);

	std::ostringstream oss;
	oss << GetName().c_str() << "‚ð“|‚µ‚½II";

	if (Data.mId == 0)
	{
		GetGame()->GetActorManager()->GetStage()->GetLog()->AddText(oss.str().c_str());
		if (GetGame()->GetActorManager()->GetPHome())
		{
			GetGame()->GetActorManager()->GetPHome()->SetBattlePoints(GetGame()->GetActorManager()->GetPHome()->GetBattlePoints() + 150 + GetLevel() * 50);
			if (GetGame()->GetActorManager()->GetPHome()->GetMaxBattlePoints() < GetGame()->GetActorManager()->GetPHome()->GetBattlePoints())
			{
				GetGame()->GetActorManager()->GetPHome()->SetBattlePoints(GetGame()->GetActorManager()->GetPHome()->GetMaxBattlePoints());
			}
		}
	}
	else
	{
		GetGame()->GetActorManager()->GetStage()->GetLog()->AddText(oss.str().c_str());
		if (GetGame()->GetActorManager()->GetPHome())
		{
			GetGame()->GetActorManager()->GetPHome()->SetBattlePoints(GetGame()->GetActorManager()->GetPHome()->GetBattlePoints() + 100 + GetLevel() * 50);
			if (GetGame()->GetActorManager()->GetPHome()->GetMaxBattlePoints() < GetGame()->GetActorManager()->GetPHome()->GetBattlePoints())
			{
				GetGame()->GetActorManager()->GetPHome()->SetBattlePoints(GetGame()->GetActorManager()->GetPHome()->GetMaxBattlePoints());
			}
		}
	}
	if (GetGame()->GetActorManager()->GetEHome())
	{
		if (GetGame()->GetActorManager()->GetEHome()->GetLevel() > GetGame()->GetActorManager()->GetEHome()->GetSatelliteGenerateLevel() && GetGame()->GetActorManager()->GetEHome()->GetBattlePoints() >= 150)
		{
			GetGame()->GetActorManager()->GetEHome()->SetBattlePoints(GetGame()->GetActorManager()->GetEHome()->GetBattlePoints() - 150);
			GetGame()->GetActorManager()->GetEHome()->SetSatelliteGenerateLevel(GetGame()->GetActorManager()->GetEHome()->GetSatelliteGenerateLevel() + 1);
		}
	}

	for (auto wing : mWings)
	{
		wing->SetState(EDead);
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