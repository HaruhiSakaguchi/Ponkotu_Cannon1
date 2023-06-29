#pragma once
#include "Enemy.h"
#include "StateComponent.h"
#include "HpGaugeSpriteComponent.h"
#include "SatelliteWing.h"

class Satellite :
	public Enemy
{
public:
	Satellite(class Game* game);
	Satellite(class Game* game, const VECTOR& pos);

	~Satellite();
	int SetUp()override;
	void UpdateActor()override;
	void Damage(int damage = 1)override;
	void Dead()override;
	//getter,setter
	const VECTOR& GetTargetPosition();
	void SetInterval(float interval) { Data.mInterval = interval; }
	void SetElapsedTime(float time) { Data.mElapsedTime = time; }
	float GetElapsedTime() { return Data.mElapsedTime; }
	const VECTOR& GetBulletOffset() { return Data.mBulletOffsetPos; }
	float GetLaunchInterval() { return Data.mInterval; }
	float GetAngleForPos() { return Data.mAngleForPos; }
	float GetAnimAngle() { return Data.mAnimAngle; }
	void SetAngleForPos(float angle) { Data.mAngleForPos = angle; }
	void SetAnimAngle(float angle) { Data.mAnimAngle = angle; }
	float GetAdvSpeed() { return Data.mAdvSpeed; }
	float GetMoveRange() { return Data.mMoveRange; }
	float GetAnimSpeed() { return Data.mAnimSpeed; }
	int GetNum() { return Num; }
	StateComponent* GetStateMacine() { return mState; }
	void SetRange(float range) { mRange = range; }
	float GetRange() { return mRange; }
	void SetTargetPos(const VECTOR& pos) { mTargetPos = pos; }

	std::vector<class SatelliteWing*>& GetWings() { return mWings; }
	void AddWings(class SatelliteWing* wing);
	void RemoveWings(class SatelliteWing* wing);
	int GetId() { return Data.mId; }
	void SetTmpRotation(const VECTOR& angle) { mTmpRotation = angle; }
	void SetTmpTargetVec(const VECTOR& target) { mTargetVec = target; }

	const VECTOR& GetTmpRotation() { return mTmpRotation; }
	const VECTOR& GetType1TargetVec() { return mTargetVec; }

	class StateComponent* GetStateCompoState() { return mState; }


	//data
	struct DATA {
		VECTOR mTargetPos;
		VECTOR mBulletOffsetPos;
		float mAdvSpeed;
		float mAmbient;
		float mMoveRange;
		float mAnimSpeed;
		float mInterval;
		float mElapsedTime;
		float mRadius;
		float mHeight;
		float mAnimAngle;
		float mAngleForPos;
		float mMaxDamageInterval;
		float mGravity;
		int mBodyNumCorners;
		float mBodyFrontZ;
		float mBodyBackZ;
		float mMaxRange;
		int mMaxHp;
		int mMaxHp2;
		int mId;
		VECTOR mCapsulOffset;
		VECTOR mHpGaugeOffset;
		VECTOR mOffsetPos;
		VECTOR mTargetOffsetPos;
		VECTOR mDeadPoint;
		COLOR mImageColor;
	};
private:
	static int Num;
	bool mDeadFlag;
	//data
	DATA Data;
	//component
	class HpGaugeSpriteComponent* mHpGauge;
	class StateComponent* mState;

	float mRange;
	MATRIX Master;
	MATRIX Target;
	VECTOR mTargetVec;
	VECTOR mTmpRotation;
	VECTOR mTargetPos;
	std::vector<SatelliteWing*>mWings;

};



