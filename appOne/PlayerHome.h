#pragma once
#include "PSideCharacterActor.h"
class PlayerHome :
	public PSideCharacterActor
{
public:
	PlayerHome(class Game* game);
	~PlayerHome();
	int SetUp()override;
	void UpdateActor()override;
	void Damage(int damage = 1);

	const int GetGenerateCannonLv() { return mGenerateCannonLevel; }
	const int GetGenerateBarricadeLv() { return mGenerateBarricadeLevel; }
	void SetGenerateCannonLv(int lv) { mGenerateCannonLevel = lv; }
	void SetGenerateBarricadeLv(int lv) { mGenerateBarricadeLevel = lv; }

	VECTOR* GetHomeTargetPoints() { return mHomeTargetPoints; }
	VECTOR* GetFieldTargetPoints() { return mFieldTargetPoints; }
	VECTOR* GetHomeMoveTargetPoints() { return mMyTargetPoints; }
	void SetTargetPoint(const VECTOR& pos) { mHomeTargetPoint = pos; }
	bool GoToTargetPoint(const VECTOR& pos);
	bool GetMoveCompleteFlag() { return mMoveCompleteFlag; }

	int GetTargetPosIdx() { return mCurMyTpIdx; }
	void SetTargetPosIdx(int idx) { mCurMyTpIdx = idx; }

	bool BeginOpen() { return mBeginOpenFlag; }
	bool BeginClose() { return mBeginCloseFlag; }
	bool GetCloseComplete() { return mCloseComplete; }
	bool GetOpenComplete() { return mOpenComplete; }
	bool OpenDore();
	bool CloseDore();

	void Open() { mBeginOpenFlag = true; }
	void Close() { mBeginCloseFlag = true; }

	void SetGenerateFlag(bool flag) { mGenerateFlag = flag; }
	bool GetGenerateFlag() { return mGenerateFlag; }

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
	bool mMoveCompleteFlag;
	VECTOR mHomeTargetPoint;
	bool mCloseComplete;
	bool mOpenComplete;
	bool mBeginCloseFlag;
	bool mBeginOpenFlag;

	bool mGenerateFlag;

};

