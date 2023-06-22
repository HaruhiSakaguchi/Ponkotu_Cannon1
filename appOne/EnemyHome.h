#pragma once
#include "CharacterActor.h"
#include "Dore.h"

class EnemyHome :
	public CharacterActor
{
public:
	EnemyHome(class Game* game);
	~EnemyHome();
	int SetUp()override;
	void UpdateActor()override;
	void Damage(int damage = 1);
	bool InEnemyArea(const VECTOR& pos);

	void SetTamaGenerateLevel(int lv) { mGenerateTamaLevel = lv; }
	void SetSatelliteGenerateLevel(int lv) { mGenerateSatelliteLevel = lv; }
	int GetTamaGenerateLevel() { return mGenerateTamaLevel; }
	int GetSatelliteGenerateLevel() { return mGenerateSatelliteLevel; }
	int GetBattlePoints() { return mBattlePoints; }
	void SetBattlePoints(int pt) { mBattlePoints = pt; }

	void SetGenerateFlag(bool flag) { mGenerateFlag = flag; }
	bool GetGenerateFlag() { return mGenerateFlag; }

	class Dore* GetDore() { return mDore; }

	struct DATA
	{
		VECTOR mFlag1Offset = VECTOR(-2.0f, 2.0f, 3.0f + 0.25f);
		VECTOR mFlag2Offset = VECTOR(2.0f, 2.0f, 3.0f + 0.25f);
		VECTOR mDoreOffset = VECTOR(0.0f, 1.8f, 1.9f);
		VECTOR mHpGaugeOffset = VECTOR(0.0f, 3.0f, 0.0f);
		int mMaxHp = 20;
		float mRadius = 3.0f;
		float mHeight = 0.0f;
		float mMaxDamageInterval = 0.09f;

	};
private:
	DATA Data;
	class TreeMeshComponent* mTc;
	class EnemyFlag* mFlag1;
	class EnemyFlag* mFlag2;
	class Dore* mDore;
	int mBattlePoints;
	int mMaxBattlePoints;
	int mGenerateTamaLevel;
	int mGenerateSatelliteLevel;
	float mInterval;
	float mElapsedTime;
	int mCnt;
	bool mGenerateFlag;
};

