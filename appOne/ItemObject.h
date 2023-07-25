#pragma once
#include "CharacterActor.h"
#include <chrono>

class ItemObject : public CharacterActor
{
public:
	ItemObject(class Game* game);
	virtual ~ItemObject();
	int SetUp()override;
	void UpdateActor()override;
	virtual bool update() { return true; }
	//getter
	float GetLifeTime() { return cData.mLifeTime; }
	float GetMaxLifeTime() { return cData.mMaxLifeTime; }
	int GetNum() { return iData.mNum; }
	const char* GetName() { return iData.mName; }
	void CreateMesh(const char* normalMeshName = nullptr, const char* damageMeshName = nullptr);

public:
	//�ŗL�̃f�[�^
	struct individualDATA
	{
		COLOR mColor;
		int mSound1;
		int mSound2;
		int mNum;
		const char* mName;
	};
	//���ʂ̃f�[�^
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
	//�ŗL�̃f�[�^
	individualDATA iData;
	//���ʂ̃f�[�^
	commonDATA cData;
protected:
	class TreeMeshComponent* mTc;
	class CharacterActor* mOwner;
	std::chrono::system_clock::time_point mStart;
	float mTime;
	bool mIsSpawnParticle;
	int mBeforeTime;
	const char* mBatchName;
};