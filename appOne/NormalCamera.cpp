#include "NormalCamera.h"
#include "Camera.h"
#include "Game.h"
#include "Renderer.h"
#include "input.h"
#include "Cannon.h"
#include "PlayerHome.h"
#include "TreeMeshComponent.h"


NormalCamera::NormalCamera(Game* game)
	: Camera(game)
{
	SetPosition(VECTOR(0.0f, 90.0f, -27.0f));
	Data = GetGame()->GetAllData()->cameraData;
	SetRotationX(3.1415926f / 2);
	SetPosition(VECTOR(10.0f, 15.0f, -20.0f));
}

void NormalCamera::UpdateActor()
{
	float rotX = GetRotation().x;
	float rotY = GetRotation().y;

	if (Data.mDistanceFromLookatPos < Data.mMaxDistanceFromLookatPos) { Data.mDistanceFromLookatPos = Data.mMaxDistanceFromLookatPos; }

	VECTOR pos = GetPosition();
	//現在のカメラ位置をとっておく
	VECTOR prePos = pos;
	SetPosition(15.0f, 15.0f, 25.0f);
	Data.mLookatPos = VECTOR(0.0f, -5.0f, (GetGame()->GetActorManager()->GetStage()->GetStageMinZ() + GetGame()->GetActorManager()->GetStage()->GetStageMaxZ()) / 2);
	pos = GetPosition();
	Data.mLookatPos.y += Data.mOffsetPosY;

	//カメラの位置を求める
	pos.x += sinf(rotY) * cosf(rotX) * Data.mDistanceFromLookatPos;
	pos.y += sinf(rotX) * Data.mDistanceFromLookatPos;
	pos.z += cosf(rotY) * cosf(rotX) * Data.mDistanceFromLookatPos;

	//少し時間差でスムーズにカメラの位置を変更

	SetRotationY(rotY);
	SetRotationX(rotX);
	SetPosition(pos);
}
