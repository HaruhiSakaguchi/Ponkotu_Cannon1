#pragma once
#include "Actor.h"
#include "VECTOR.h"
#include "VECTOR2.h"

class Camera :
	public Actor
{
public:
	Camera(class Game* game);
	~Camera();
	void UpdateActor() override;
	const VECTOR& GetLookatPos() { return Data.mLookatPos; }
	struct DATA
	{
		float mDistanceFromLookatPos;
		float mRotSpeedX;
		float mRotSpeedY;
		VECTOR mUpVec;
		VECTOR mLookatPos;
		float mOffsetPosY;
		float mOffsetPosZ;
		float mChangePosSpeed;
		//ï‚ê≥íl
		float mCorrectPos;
		float mMaxRotX;
		float mMaxDistanceFromLookatPos;
		VECTOR2 mCenterPos;
		VECTOR2 mMousePos;
	};
protected:
	DATA Data;
};
