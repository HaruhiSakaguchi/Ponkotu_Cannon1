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
	//カメラの視点方向に三角形を作り、その中にマップの表示用ポリゴンやキャラクターの座標を入れ、表示するかどうかを判断する。
	void SetCannonKillerPos(const VECTOR&pos) { mCannonKillerPos = pos; }
	const VECTOR& GetCannonKillerPos() { return mCannonKillerPos; }
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
		//補正値
		float mCorrectPos;
		float mMaxRotX;
		float mMaxDistanceFromLookatPos;
		VECTOR2 mCenterPos;
		VECTOR2 mMousePos;
	};
private:
	DATA Data;
	//Cannonを殺したアクターのポインタ
	VECTOR mCannonKillerPos;
};
