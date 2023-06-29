#pragma once
#include "CharacterActor.h"
#include <chrono>

class Item : public CharacterActor
{
public:
	Item(class Game* game);
	virtual ~Item();
	int SetUp()override;
	void UpdateActor()override;
	virtual bool update() { return true; }
	//getter
	float GetLifeTime() { return cData.mLifeTime; }
	float GetMaxLifeTime() { return cData.mMaxLifeTime; }
	int GetNum() { return iData.mNum; }
	const char* GetName() { return iData.mName; }
public:
	//固有のデータ
	struct individualDATA
	{
		COLOR mColor;
		int mSound1;
		int mSound2;
		int mNum;
		const char* mName;
	};
	//共通のデータ
	struct commonDATA
	{
		float mRadius;
		float mJumpVelocity;
		float mGravity;
		int mJumpFlag;
		float mAdvSpeed;
		int mHp;
		float mLifeTime;
		float mMaxLifeTime;
		int mDropSound;
		int mDropSoundVolumeOffset;
		VECTOR mCapsulOffset;
	};
protected:
	//固有のデータ
	individualDATA iData;
	//共通のデータ
	commonDATA cData;
protected:
	class TreeMeshComponent* mTc;
	class CharacterActor* mOwner;
	std::chrono::system_clock::time_point mStart;
	float mTime;
	bool mIsSpawnParticle;
	int mBeforeTime;
};