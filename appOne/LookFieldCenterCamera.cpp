#include "LookFieldCenterCamera.h"
#include "Camera.h"
#include "Game.h"
#include "Renderer.h"
#include "input.h"
#include "Cannon.h"
#include "PlayerHome.h"
#include "TreeMeshComponent.h"

LookFieldCenterCamera::LookFieldCenterCamera(Game* game)
	: Camera(game)
{
	SetPosition(VECTOR(0.0f, 94.0f , -27.0f));
	Data = GetGame()->GetAllData()->cameraData;
	SetRotationX(3.1415926f / 2);
}

void LookFieldCenterCamera::UpdateActor()
{
	float rotX = GetRotation().x;
	float rotY = GetRotation().y;


	rotX = 1.57f;

	VECTOR pos = GetPosition();
	//現在のカメラ位置をとっておく
	VECTOR prePos = pos;

	SetPosition(GetGame()->GetActorManager()->GetStage()->GetCenterPos() + VECTOR(0.0f,94.0f,0.0f));

	
	Data.mLookatPos = GetPosition();
	Data.mLookatPos.y += Data.mOffsetPosY;

	//カメラの位置を求める
	pos = Data.mLookatPos;
	pos.x += sinf(rotY) * cosf(rotX) * Data.mDistanceFromLookatPos;
	pos.y += sinf(rotX) * Data.mDistanceFromLookatPos;
	pos.z += cosf(rotY) * cosf(rotX) * Data.mDistanceFromLookatPos;

	//少し時間差でスムーズにカメラの位置を変更

	SetRotationY(rotY);
	SetRotationX(rotX);
	SetPosition(pos);

	//print(GetRotation().x);
	//print(GetRotation().y);
	//print(GetRotation().z);

	/*print("mouse(" + (let)mouseX + "," + (let)mouseY + ")");
	print("camRot(" + (let)GetRotation().x + "," + (let)GetRotation().y + "," + (let)GetRotation().z);
	*/
	//print("camPos(" + (let)GetPosition().x + "," + (let)GetPosition().y + "," + (let)GetPosition().z);
	//print("LookPos(" + (let)Data.mLookatPos.x + "," + (let)Data.mLookatPos.y + "," + (let)Data.mLookatPos.z);

}