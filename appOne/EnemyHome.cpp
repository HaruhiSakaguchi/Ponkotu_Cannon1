#include "EnemyHome.h"
#include "TreeMeshComponent.h"
#include "EnemyFlag.h"
#include "HpGaugeSpriteComponent.h"
#include "Game.h"
#include "Collision_Capsul.h"
#include "rand.h"
#include "window.h"


EnemyHome::EnemyHome(class Game* game)
	: CharacterActor(game)
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
	GetGame()->GetActorManager()->SetEHome(this);
}

EnemyHome::~EnemyHome()
{
	mDore->SetState(EDead);
	mFlag1->SetState(EDead);
	mFlag2->SetState(EDead);
	GetGame()->GetActorManager()->SetEHome(nullptr);
}

int EnemyHome::SetUp()
{
	TreeMeshComponent* tc = new TreeMeshComponent(this);
	tc->SetTree("Home");
	mDore = new Dore(GetGame());
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
		if ((int)(GetGame()->GetActorManager()->GetEnemies().size() - 1) == (GetLevel()))
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
				int num = random(0, ((int)(GetGame()->GetActorManager()->GetEnemies().size()) - 1));
				auto enemy = GetGame()->GetActorManager()->GetEnemies()[num];
				if (enemy->GetLevel() < GetLevel() && (100 + 50 * enemy->GetLevel()) <= mBattlePoints)
				{
					mBattlePoints -= (100 + 50 * enemy->GetLevel());
					enemy->SetLevel(enemy->GetLevel() + 1);
					mCnt++;
					int curMaxHp = enemy->GetMaxHp();
					enemy->SetMaxHp((int)(enemy->GetInitMaxHp() * ((enemy->GetLevel() + enemy->GetMaxLevel()) / 10.0f)));
					enemy->SetHp((int)(round(enemy->GetMaxHp() * (float)enemy->GetHp() / (float)curMaxHp)));
				}
				mElapsedTime = 0.0f;
			}
		}
		else if ((int)(GetGame()->GetActorManager()->GetEnemies().size()) < (GetLevel() + 1) && (int)(GetGame()->GetActorManager()->GetEnemies().size()) < GetMaxLevel() && mDore->GetCloseComplete())
		{
			VECTOR pos = VECTOR(random(GetGame()->GetActorManager()->GetStage()->GetStageMinX(), GetGame()->GetActorManager()->GetStage()->GetStageMaxX()), random(4.0f, 7.5f), random(GetGame()->GetActorManager()->GetStage()->GetStageMinZ(), GetGame()->GetActorManager()->GetStage()->GetCenterPos().z));
			int num = random();
			if (num % 4 == 0 || num % 4 == 2 || num % 4 == 3)
			{
				if (PositionOnMap(pos, GetGame()->GetAllData()->tamaData.mRadius) && InEnemyArea(pos) && mBattlePoints >= (150 + mGenerateTamaLevel * 50) && pos.z >= GetPosition().z)
				{
					pos.y = 0.0f;
					class Tama* tama = new class Tama(GetGame(), pos);
					tama->SetPosition(GetPosition());
					tama->SetLevel(mGenerateTamaLevel);
					tama->SetMaxHp((int)(tama->GetInitMaxHp() * ((tama->GetLevel() + tama->GetMaxLevel()) / 10.0f)));
					tama->SetHp(tama->GetMaxHp());
					mBattlePoints -= (150 + mGenerateTamaLevel * 50);
					GetGame()->GetActorManager()->GetStage()->GetLog()->AddText("が出現。");
					mElapsedTime = 0.0f;
					mGenerateFlag = true;
					mDore->Open();
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
					satellite->SetMaxHp((int)(satellite->GetInitMaxHp() * ((satellite->GetLevel() + satellite->GetMaxLevel()) / 10.0f)));
					satellite->SetHp(satellite->GetMaxHp());
					VECTOR EPos = GetPosition();
					if (satellite->GetId() == 0)
					{
						EPos.y = 0.5f;
					}
					else
					{
						EPos.y = 0.5f;
					}

					satellite->SetPosition(EPos);
					mBattlePoints -= (200 + mGenerateSatelliteLevel * 50);
					mElapsedTime = 0.0f;
					if (satellite->GetId() == 0)
					{
						GetGame()->GetActorManager()->GetStage()->GetLog()->AddText("SatelliteAが出現。");
					}
					else
					{
						GetGame()->GetActorManager()->GetStage()->GetLog()->AddText("SatelliteBが出現。");
					}
					mGenerateFlag = true;
					mDore->Open();
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

	if (mGenerateFlag)
	{
		mDore->SetIsRotate(true);
	}

	int cnt = 0;

	for (auto Actor : GetGame()->GetActorManager()->GetEnemies())
	{
		if (CollisionCircle(GetRadius(), Actor->GetRadius(), GetPosition(), Actor->GetPosition()))
		{
			cnt++;
		}
	}

	if (mGenerateFlag && cnt == 0)
	{
		mDore->Close();
		mGenerateFlag = false;
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

