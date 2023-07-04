#pragma once
#include "PSideCharacterActor.h"
#include "Dore.h"

class PlayerHome :
	public PSideCharacterActor
{
public:
	PlayerHome(class Game* game,const VECTOR&pos = VECTOR(0.0f,0.0f,0.0f));
	~PlayerHome();
	int SetUp()override;
	void UpdateActor()override;
	void Damage(int damage = 1);
	void Dead()override;

	const int GetGenerateCannonLv() { return mGenerateCannonLevel; }
	const int GetGenerateBarricadeLv() { return mGenerateBarricadeLevel; }
	void SetGenerateCannonLv(int lv) { mGenerateCannonLevel = lv; }
	void SetGenerateBarricadeLv(int lv) { mGenerateBarricadeLevel = lv; }

	VECTOR* GetHomeTargetPoints() { return mHomeTargetPoints; }
	VECTOR* GetFieldTargetPoints() { return mFieldTargetPoints; }
	VECTOR* GetHomeMoveTargetPoints() { return mMyTargetPoints; }
	void SetTargetPoint(const VECTOR& pos) { mHomeTargetPoint = pos; }
	int GoToTargetPoint(const VECTOR& pos);
	int GetMoveCompleteFlag() { return mMoveCompleteFlag; }

	int GetTargetPosIdx() { return mCurMyTpIdx; }
	void SetTargetPosIdx(int idx) { mCurMyTpIdx = idx; }

	void SetGenerateFlag(int flag) { mGenerateFlag = flag; }
	int GetGenerateFlag() { return mGenerateFlag; }

	int GetBattlePoints() { return mBattlePoints; }
	int GetMaxBattlePoints() { return mMaxBattlePoints; }
	void SetBattlePoints(int point) { mBattlePoints = point; }
	bool InPlayerArea(const VECTOR& pos);

	void CreateHomeArea();

	class Dore* GetDore() { return mDore; }
	struct DATA
	{
		VECTOR mFlag1Offset = VECTOR(-2.0f, 2.0f, -3.0f + 0.25f);
		VECTOR mFlag2Offset = VECTOR(2.0f, 2.0f, -3.0f + 0.25f);
		VECTOR mDoreOffset = VECTOR(0.0f, 1.8f, -1.9f);
		VECTOR mHpGaugeOffset = VECTOR(0.0f, 3.0f, 0.0f);
		float mRadius = 3.0f;
		float mHeight = 0.0f;
		int mMaxHp = 20;
		float mMaxDamageInterval = 0.09f;

	};
private:
	DATA Data;
	class PlayerFlag* mFlag1;
	class PlayerFlag* mFlag2;
	class Dore* mDore;
	VECTOR mHomeTargetPoints[4];
	VECTOR mFieldTargetPoints[4];
	VECTOR mMyTargetPoints[3];
	class UIPlayerHome* mUI;
	int mGenerateCannonLevel;
	int mGenerateBarricadeLevel;
	int mCurMyTpIdx;
	int mMoveCompleteFlag;
	VECTOR mHomeTargetPoint;

	class TreeMeshComponent*mTc;
	int mGenerateFlag;
	std::vector<Actor*>mProps;

	int mBattlePoints;
	int mMaxBattlePoints;

};

