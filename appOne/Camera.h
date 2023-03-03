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
	//�J�����̎��_�����ɎO�p�`�����A���̒��Ƀ}�b�v�̕\���p�|���S����L�����N�^�[�̍��W�����A�\�����邩�ǂ����𔻒f����B
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
		//�␳�l
		float mCorrectPos;
		float mMaxRotX;
		float mMaxDistanceFromLookatPos;
		VECTOR2 mCenterPos;
		VECTOR2 mMousePos;
	};
private:
	DATA Data;
	//Cannon���E�����A�N�^�[�̃|�C���^
	VECTOR mCannonKillerPos;
};
