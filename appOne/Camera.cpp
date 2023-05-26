#include "Camera.h"
#include "Game.h"
#include "Renderer.h"
#include "input.h"
#include "Cannon.h"

Camera::Camera(Game* game)
	: Actor(game)
	, mCannonKillerPos(0.0f, 0.0f, 0.0f)
{
	//if (GetGame()->GetCannon())
	//{
	//	SetPosition(GetGame()->GetCannon()->GetPosition());
	//}
	//else
	//{
	//	SetPosition(0.0f, 0.0f, 0.0f);
	//}
	//SetRotationX(3.1415926f / 3);

	//SetRotationX(3.1415926f / 2);
	//SetRotationY(-3.1415926f / 6.0f);
	SetRotationX(0.424f);
	SetRotationY(0.334f);
   // SetRotationY(3.1415926f);
	Data = GetGame()->GetAllData()->cameraData;
	//SetPosition(-10.0f, 50.0f, 0);
//	SetPosition(-10.0f, 50.0f, 20.0f);
	SetPosition(10.0f, 15.0f, 20.0f);
}

Camera::~Camera()
{
	GetGame()->SetCamera(nullptr);
}

void Camera::UpdateActor()
{
	VECTOR2 vec = VECTOR2(Data.mCenterPos.x - mouseX, Data.mCenterPos.y - mouseY);

	vec.normalize();
	float rotX = GetRotation().x;
	float rotY = GetRotation().y;


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

	//Cannonの有無によって注視点を切り替え
	//if (!GetGame()->GetCannon())
	//{
	//	//Cannonを殺したキャラクターのpositionをセット
	//	SetPosition(GetCannonKillerPos());
	//	Data.mLookatPos = GetPosition();
	//	Data.mLookatPos.z += Data.mOffsetPosZ;

	//}
	//else if (GetGame()->GetCannon())
	{
		//SetPosition(GetGame()->GetCannon()->GetPosition() + GetGame()->GetCannon()->GetCapsulOffset());
		//SetPosition(10.0f, 95.0f,(GetGame()->GetStage()->GetStageMinZ() + GetGame()->GetStage()->GetStageMaxZ()) / 2 + 10);
		//SetPosition(-10.0f, 50.0f, 20.0f);
		//SetPosition(0.0f, 95.0f, (GetGame()->GetStage()->GetStageMinZ() + GetGame()->GetStage()->GetStageMaxZ()) / 2);
		SetPosition(10.0f, 15.0f, 20.0f);
		SetRotationX(0.424f);
		SetRotationY(0.334f);
		Data.mLookatPos = GetPosition();

	}

	Data.mLookatPos.y += Data.mOffsetPosY;

	//カメラの位置を求める
	pos = Data.mLookatPos;
	pos.x += sinf(rotY) * cosf(rotX) * Data.mDistanceFromLookatPos;
	pos.y += sinf(rotX) * Data.mDistanceFromLookatPos;
	pos.z += cosf(rotY) * cosf(rotX) * Data.mDistanceFromLookatPos;

	//少し時間差でスムーズにカメラの位置を変更

	pos.x = prePos.x + (pos.x - prePos.x) * Data.mChangePosSpeed;
	pos.z = prePos.z + (pos.z - prePos.z) * Data.mChangePosSpeed;
	/*if (!GetGame()->GetCannon())
	{
		pos.y = prePos.y + (pos.y - prePos.y) * Data.mChangePosSpeed;
	}*/

	MATRIX view;
	view.camera(pos, Data.mLookatPos, Data.mUpVec);
	GetGame()->GetRenderer()->SetView(view);

	SetRotationY(rotY);
	SetRotationX(rotX);
	SetPosition(pos);

	//print(GetRotation().x);
	//print(GetRotation().y);
	//print(GetRotation().z);

	/*print("mouse(" + (let)mouseX + "," + (let)mouseY + ")");
	print("camRot(" + (let)GetRotation().x + "," + (let)GetRotation().y + "," + (let)GetRotation().z);

	print("camPos(" + (let)GetPosition().x + "," + (let)GetPosition().y + "," + (let)GetPosition().z);*/
}