#include "EnemyHome.h"
#include "TreeMeshComponent.h"
#include "Dore.h"
#include "EnemyFlag.h"
#include "HpGaugeSpriteComponent.h"
#include "Game.h"
#include "Collision_Capsul.h"
#include "rand.h"
#include "window.h"

EnemyHome::EnemyHome(class Game* game)
	:CharacterActor(game)
	, mFlag1(nullptr)
	, mFlag2(nullptr)
	, mDore(nullptr)
{
	SetUp();
	GetGame()->SetEHome(this);
}

EnemyHome::~EnemyHome()
{
	mDore->SetState(EDead);
	mFlag1->SetState(EDead);
	mFlag2->SetState(EDead);
	//delete mDore;
	//delete mFlag1;
	//delete mFlag2;
	GetGame()->SetEHome(nullptr);
}

int EnemyHome::SetUp()
{
	TreeMeshComponent* tc = new TreeMeshComponent(this);
	tc->SetTree("Home");
	mDore = new Dore(GetGame());
	mDore->SetRotationY(3.1415926f);
	mFlag1 = new EnemyFlag(GetGame());
	mFlag1->SetRotationY(3.1415926f);
	mFlag2 = new EnemyFlag(GetGame());
	mFlag2->SetRotationY(3.1415926f);
	SetTag(EHome);
	SetHp(Data.mMaxHp);
	SetRadius(Data.mRadius);
	SetHeight(Data.mHeight);
	new HpGaugeSpriteComponent(this, Data.mHpGaugeOffset);
	return 1;
}

void EnemyHome::UpdateActor()
{
	mDore->SetPosition(GetPosition() + Data.mDoreOffset);
	mFlag1->SetPosition(GetPosition() + Data.mFlag1Offset);
	mFlag2->SetPosition(GetPosition() + Data.mFlag2Offset);

	if (GetDamageInterval() > 0.0f)
	{
		SetDamageInterval(GetDamageInterval() - delta);
	}

	if (GetGame()->GetCannon())
	{
		Intersect(this, GetGame()->GetCannon());
	}

	if (GetGame()->GetEnemies().size() != GetGame()->GetStage()->GetStageCharacterCapa())
	{
		VECTOR pos = VECTOR(random(GetGame()->GetStage()->GetStageMinX(), GetGame()->GetStage()->GetStageMaxX()), random(4.0f, 7.5f), random(GetGame()->GetStage()->GetStageMinZ(), GetGame()->GetStage()->GetStageMaxZ()));
		int num = random();
		if (num % 4 == 0 || num % 4 == 2 || num % 4 == 3)
		{
			if (PositionOnMap(pos, GetGame()->GetAllData()->tamaData.mRadius) && InEnemyArea(pos))
			{
				Tama* tama = new Tama(GetGame(), pos);
				GetGame()->GetStage()->GetLog()->AddText("Tamaが出現。");
			}
		}
		else
		{
			if (PositionOnMap(pos, GetGame()->GetAllData()->satelliteData.mHeight) && InEnemyArea(pos))
			{
				Satellite* satellite = new Satellite(GetGame(), pos);
				CharacterActor::SEGMENT* seg = new CharacterActor::SEGMENT(satellite);
				satellite->SetSeg(seg);
				if (satellite->GetId() == 0)
				{
					GetGame()->GetStage()->GetLog()->AddText("SatelliteAが出現。");
				}
				else
				{
					GetGame()->GetStage()->GetLog()->AddText("SatelliteBが出現。");
				}
			}
		}
	}
}

void EnemyHome::Damage(int damage)
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
		SetState(EDead);
	}
}

bool EnemyHome::InEnemyArea(const VECTOR& pos)
{
	float distX = pos.x - GetPosition().x;
	float distZ = pos.z - GetPosition().z;

	float dist = sqrtf(distX * distX + distZ * distZ);

	bool in = (dist <= 7.0f);
	return in;
}
