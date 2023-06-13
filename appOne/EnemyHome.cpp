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
	, mBattlePoints(0)
	, mMaxBattlePoints(0)
	, mGenerateTamaLevel(0)
	, mGenerateSatelliteLevel(0)
	, mInterval(0.0f)
	, mElapsedTime(0.0f)
	, mCnt(0)
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
	SetMaxHp(Data.mMaxHp);
	SetInitMaxHp(Data.mMaxHp);
	mMaxBattlePoints = 500;
	SetRadius(Data.mRadius);
	SetHeight(Data.mHeight);
	new HpGaugeSpriteComponent(this, Data.mHpGaugeOffset);
	mInterval = 200.0f;
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


	if (mElapsedTime >= mInterval)
	{
		if ((int)(GetGame()->GetEnemies().size() - 1) == (GetLevel()))
		{
			if (mCnt % 2 == 0)
			{
				if (mBattlePoints >= 300 && GetLevel() < GetMaxLevel())
				{
					SetLevel(GetLevel() + 1);
					mBattlePoints -= 300;
					mCnt++;
				}
				mElapsedTime = 0.0f;
			}
			else
			{
				int num = random(0, ((int)(GetGame()->GetEnemies().size()) - 1));
				auto enemy = GetGame()->GetEnemies()[num];
				if (enemy->GetLevel() < GetLevel() && (100 + 50 * enemy->GetLevel()) <= mBattlePoints)
				{
					mBattlePoints -= (100 + 50 * enemy->GetLevel());
					enemy->SetLevel(enemy->GetLevel() + 1);
					mCnt++;
				}
				mElapsedTime = 0.0f;
			}
		}
		else if ((int)(GetGame()->GetEnemies().size()) < (GetLevel() + 1) && (int)(GetGame()->GetEnemies().size()) < GetMaxLevel())
		{
			VECTOR pos = VECTOR(random(GetGame()->GetStage()->GetStageMinX(), GetGame()->GetStage()->GetStageMaxX()), random(4.0f, 7.5f), random(GetGame()->GetStage()->GetStageMinZ(), GetGame()->GetStage()->GetStageMaxZ()));
			int num = random();
			if (num % 4 == 0 || num % 4 == 2 || num % 4 == 3)
			{
				if (PositionOnMap(pos, GetGame()->GetAllData()->tamaData.mRadius) && InEnemyArea(pos) && mBattlePoints >= (150 + mGenerateTamaLevel * 50))
				{
					class Tama* tama = new class Tama(GetGame(), pos);
					tama->SetLevel(mGenerateTamaLevel);
					mBattlePoints -= (150 + mGenerateTamaLevel * 50);
					GetGame()->GetStage()->GetLog()->AddText("が出現。");
					mElapsedTime = 0.0f;
				}
				else
				{
					mElapsedTime = mInterval / 2.0f;
				}
			}
			else
			{
				if (PositionOnMap(pos, GetGame()->GetAllData()->satelliteData.mHeight) && InEnemyArea(pos) && mBattlePoints >= (200 + mGenerateSatelliteLevel * 50))
				{
					class Satellite* satellite = new class Satellite(GetGame(), pos);
					CharacterActor::SEGMENT* seg = new CharacterActor::SEGMENT(satellite);
					satellite->SetSeg(seg);
					satellite->SetLevel(mGenerateSatelliteLevel);
					mBattlePoints -= (200 + mGenerateSatelliteLevel * 50);
					mElapsedTime = 0.0f;
					if (satellite->GetId() == 0)
					{
						GetGame()->GetStage()->GetLog()->AddText("SatelliteAが出現。");
					}
					else
					{
						GetGame()->GetStage()->GetLog()->AddText("SatelliteBが出現。");
					}
				}
				else
				{
					mElapsedTime = mInterval / 2.0f;
				}
			}

		}

	}
	else
	{
		if (mBattlePoints >= mMaxBattlePoints)
		{
			if (rand() % 2 == 0)
			{
				if (mGenerateTamaLevel < GetLevel())
				{
					mGenerateTamaLevel += 1;
					mBattlePoints -= 300;
				}
			}
			else
			{
				if (mGenerateSatelliteLevel < GetLevel())
				{
					mGenerateSatelliteLevel += 1;
					mBattlePoints -= 300;
				}
			}
		}

	}

	mMaxBattlePoints = 500 * (GetLevel() + 1);

	mElapsedTime += delta * 60.0f;
	if (mBattlePoints < mMaxBattlePoints)
	{
		mBattlePoints++;
	}
	if (mBattlePoints < 0)
	{
		mBattlePoints = 0;
	}


	fill(0, 0, 0);
	print("EBP" + (let)mBattlePoints + " / " + (let)mMaxBattlePoints + " ELv : " + (let)GetLevel());
	print("ElapsedTime :" + (let)mElapsedTime);
	print("GeneLv : Tama : " + (let)mGenerateTamaLevel + " Satellite : " + (let)mGenerateSatelliteLevel);

	for (auto enemy : GetGame()->GetEnemies())
	{
		print((let)enemy->GetName().c_str() + (let)" lv : " + (let)enemy->GetLevel());
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
