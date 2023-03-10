#include "Game.h"
#include "COLLISION_MAP.h"
#include "SatelliteStates.h"
#include "Map.h"
#include "window.h"
#include "TreeMeshComponent.h"
#include "UILog.h"

int Satellite::Num = 0;

Satellite::Satellite(class Game* game)
	: Enemy(game)
	, mHpGauge(nullptr)
	, mState(nullptr)
{
	SetUp();
	mState = new StateComponent(this);
	mState->RegisterState(new SatelliteNormal(mState));
	mState->RegisterState(new SatelliteAttack(mState));
	mState->ChangeState("Normal");
}

Satellite::Satellite(class Game* game, const VECTOR& pos)
	:Enemy(game)
	, mHpGauge(nullptr)
	, mState(nullptr)
{
	NoCollsionMap();
	SetUp();
	SetPosition(pos);
	SetInitPosition(pos);
	mState = new StateComponent(this);
	mState->RegisterState(new SatelliteNormal(mState));
	mState->RegisterState(new SatelliteAttack(mState));
	mState->ChangeState("Normal");
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
	VECTOR pos = GetPosition();
	float jumpVel = GetJumpVel();
	int jumpFlag = 0;
	float floorY = 0.0f;
	if (GetHp() > 0)
	{
		GetGame()->GetCollisionMap()->capsule_triangles
		(&pos, &jumpVel, &jumpFlag, GetAdvSpeed(), GetHeight(), GetCapsulOffset().y, GetSlant(), &floorY
		);
	}

	SetPosition(pos);

	Master.identity();
	Master.mulTranslate(GetPosition());
	Master.mulRotateY(GetRotation().y);

	if (Data.mId == 1)
	{
		SetRotationX(3.1415926f / 2);
	}

	Master.mulRotateX(GetRotation().x);

	Target.identity();

	Target.mulTranslate(Data.mTargetOffsetPos);


	if (GetDamageInterval() > 0.0f)
	{
		SetDamageInterval(GetDamageInterval() - delta);
		DamageOption();
	}

	if (GetHp() <= 0)
	{
		mState->ChangeState("Normal");
		if (GetPosition().y < 0.0f)
		{
			setVolume(mDeadSound, GetGame()->GetEffectVolume());
			playSound(mDeadSound);
			GetGame()->GetStage()->SetClearCnt(GetGame()->GetStage()->GetClearCnt() - 1);
			DropItems(Data.mDeadPoint);
			if (Data.mId == 0)
			{
				GetGame()->GetStage()->GetLog()->AddText("SatelliteA???|?????I?I");
			}
			else
			{
				GetGame()->GetStage()->GetLog()->AddText("SatelliteB???|?????I?I");
			}
			SetState(Actor::EDead);
		}
	}

	for (auto enemy : GetGame()->GetEnemies())
	{
		if (enemy != this)
		{
			Intersect(this, enemy);
		}
	}
}

const VECTOR& Satellite::GetTargetPosition()
{
	Target = Master * Target;
	Data.mTargetPos = VECTOR(Target._14, Target._24, Target._34) + Data.mBulletOffsetPos;
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