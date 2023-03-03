#pragma once
#include "CharacterActor.h"
#include "MoveComponent.h"
#include "Collision_Capsul.h"

class ActorsWeapon :
	public CharacterActor
{
public:
	ActorsWeapon(class CharacterActor* owner, const VECTOR& pos, const VECTOR& dir);
	virtual ~ActorsWeapon()override;
	virtual void damage();
	//getter,setter
	CharacterActor* GetOwner() { return mOwner; }
	const VECTOR& GetDir() { return Data.mDir; }
	void SetDir(const VECTOR& dir) { Data.mDir = dir; }
	void SetDamage(int damage) { Data.mDamage = damage; }
	//mMoveFlag‚ªfalse‚¾‚ÆMoveComponent‚ğg‚í‚È‚¢
	void UnMoveComponent() { mMoveFlag = false; }
public:
	//data
	struct DATA {
		float mRadius;
		float mHeight;
		float mLength;
		float mAdvSpeed;
		float mLifeTime;
		float mLifeTimeDecreaseSpeed;
		float mTime;
		float mMaxLifeTime;
		float mAmbient;
		int mJumpFlag;
		float mGravity;
		float mJumpVelocity;
		float mSize;
		int mDamage;
		VECTOR mDir;
		VECTOR mOffsetPos;
		COLOR mImageColor;
	};
public:
	//setup‚ğŒÄ‚Ño‚·‚Æ‚«‚Édata‚ğ“n‚·
	virtual int SetUp(const DATA& data);
protected:
	//data
	DATA Data;
	//component
	class MoveComponent* mMove;
	//matrix
	MATRIX Master;
	CharacterActor* mOwner;
	VECTOR mTmpDir;
	VECTOR mTmpPos;
	//MoveComponent‚ğg‚¤‚©‚Ç‚¤‚©
	bool mMoveFlag;
};

