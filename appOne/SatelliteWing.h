#pragma once
#include "ActorsWeapon.h"
#include "Satellite.h"

class SatelliteWing :
	public ActorsWeapon
{
public:
	SatelliteWing(class Satellite* satellite);
	~SatelliteWing();
	void UpdateActor()override;
	int SetUp()override;
	void damage()override;
	void SetAttackFlag(bool flag) { mAttackFlag = flag; }
	struct DATA
	{
		int mGurdSound;
		int mClashSound;
		int mId;
		float mGravity;
		float mWingAmbient;
		float mRadius;
		float mHeight;
		float mAdvSpeed;
		float mMaxOffsetZ;
		float mChangePosSpeed;
		float mChangeOffsetPosSpeed;
		COLOR mImageColor;
		VECTOR mWingScale1;
		VECTOR mWingScale2;
		VECTOR mS0IdW0IdOffsetPos;
		VECTOR mS0IdW1IdOffsetPos;
		VECTOR mS0IdW2IdOffsetPos;
		VECTOR mS0IdW3IdOffsetPos;
		VECTOR mS1IdOffsetPos;
		VECTOR mS0IdAttackOffsetPos;
		VECTOR mOffsetPos;
	};
private:
	DATA Data;
private:
	static int mNum;
	bool mAttackFlag;
	MATRIX Wing;
	class TreeMeshComponent* mTc;

};

