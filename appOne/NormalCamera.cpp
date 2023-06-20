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
	//SetRotationY(-3.1415926f / 6.0f);
	//SetRotationX(0.424f);
	//SetRotationY(0.334f);
	// SetRotationY(3.1415926f);
	Data = GetGame()->GetAllData()->cameraData;
	SetRotationX(3.1415926f / 2);
	//SetPosition(-10.0f, 50.0f, 0);
//	SetPosition(-10.0f, 50.0f, 20.0f);
	//SetPosition(10.0f, 15.0f, 20.0f);
	SetPosition(VECTOR(10.0f, 15.0f, -20.0f));
	//ta.mLookatPos = GetPosition() + VECTOR(0.0f, -15.0f, 0.0f);
}

void NormalCamera::UpdateActor()
{
	//VECTOR2 vec = VECTOR2(Data.mCenterPos.x - mouseX, Data.mCenterPos.y - mouseY);

	//vec.normalize();
	float rotX = GetRotation().x;
	float rotY = GetRotation().y;


	//rotX = 1.57f;

	/*if (GetGame()->GetScene() == Game::EPlay)
	{
		rotX -= vec.y * GetGame()->GetCameraSY();
		rotY += vec.x * GetGame()->GetCameraSX();
	}
	else
	{
		rotX += 0;
		rotY += 0;
	}
	*/
	//rotX -= 0.17f;
	//rotY += 0.17f;
	//if (rotX > Data.mMaxRotX) { rotX = Data.mMaxRotX; }
	//if (rotX < -Data.mMaxRotX) { rotX = -Data.mMaxRotX; }
	if (Data.mDistanceFromLookatPos < Data.mMaxDistanceFromLookatPos) { Data.mDistanceFromLookatPos = Data.mMaxDistanceFromLookatPos; }

	/*if (GetGame()->GetScene() == Game::EPlay)
	{
		SetCursorPos((int)Data.mCenterPos.x, (int)Data.mCenterPos.y);
	}
	else
	{
		SetCursorPos((int)mouseX, (int)mouseY);
	}*/

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