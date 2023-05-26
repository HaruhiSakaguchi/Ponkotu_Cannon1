#pragma once
#include "CharacterActor.h"
class PlayerHome :
	public CharacterActor
{
public:
	PlayerHome(class Game* game);
	~PlayerHome();
	int SetUp()override;
	void UpdateActor()override;
	void Damage(int damage = 1);

	VECTOR* GetHomeTargetPoints() { return mHomeTargetPoints; }
	VECTOR* GetFieldTargetPoints() { return mFieldTargetPoints; }


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
};

