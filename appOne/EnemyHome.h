#pragma once
#include "CharacterActor.h"
class EnemyHome :
	public CharacterActor
{
public:
	EnemyHome(class Game* game);
	~EnemyHome();
	int SetUp()override;
	void UpdateActor()override;
	void Damage(int damage = 1);
	struct DATA
	{
		VECTOR mFlag1Offset = VECTOR(-2.0f, 2.0f, -3.0f + 0.25f);
		VECTOR mFlag2Offset = VECTOR(2.0f, 2.0f, -3.0f + 0.25f);
		VECTOR mDoreOffset = VECTOR(0.0f, 1.8f, -1.9f);
		VECTOR mHpGaugeOffset = VECTOR(0.0f, 3.0f, 0.0f);
		int mMaxHp = 20;
		float mRadius = 3.0f;
		float mHeight = 0.0f;
	};
private:
	DATA Data;
	class EnemyFlag* mFlag1;
	class EnemyFlag* mFlag2;
	class Dore* mDore;
};

