#include "Camera.h"
#include "Game.h"
#include "Renderer.h"
#include "input.h"
#include "Cannon.h"

Camera::Camera(Game* game)
	: Actor(game)
	, mCannonKillerPos(0.0f, 0.0f, 0.0f)
{
	if (GetGame()->GetCannon())
	{
		SetPosition(GetGame()->GetCannon()->GetPosition());
	}
	else
	{
		SetPosition(0.0f, 0.0f, 0.0f);
	}

	Data = GetGame()->GetAllData()->cameraData;
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


	if (GetGame()->GetScene() == Game::EPlay)
	{
		rotX -= vec.y * GetGame()->GetCameraSY();
		rotY += vec.x * GetGame()->GetCameraSX();
	}
	else
	{
		rotX += 0;
		rotY += 0;
	}

	if (rotX > Data.mMaxRotX) { rotX = Data.mMaxRotX; }
	if (rotX < -Data.mMaxRotX) { rotX = -Data.mMaxRotX; }
	if (Data.mDistanceFromLookatPos < Data.mMaxDistanceFromLookatPos) { Data.mDistanceFromLookatPos = Data.mMaxDistanceFromLookatPos; }

	if (GetGame()->GetScene() == Game::EPlay)
	{
		SetCursorPos((int)Data.mCenterPos.x, (int)Data.mCenterPos.y);
	}
	else
	{
		SetCursorPos((int)mouseX, (int)mouseY);
	}

	VECTOR pos = GetPosition();
	//���݂̃J�����ʒu���Ƃ��Ă���
	VECTOR prePos = pos;

	//Cannon�̗L���ɂ���Ē����_��؂�ւ�
	if (!GetGame()->GetCannon())
	{
		//Cannon���E�����L�����N�^�[��position���Z�b�g
		SetPosition(GetCannonKillerPos());
		Data.mLookatPos = GetPosition();
		Data.mLookatPos.z += Data.mOffsetPosZ;

	}
	else if (GetGame()->GetCannon())
	{
		SetPosition(GetGame()->GetCannon()->GetPosition() + GetGame()->GetCannon()->GetCapsulOffset());
		Data.mLookatPos = GetPosition();
	}

	Data.mLookatPos.y += Data.mOffsetPosY;

	//�J�����̈ʒu�����߂�
	pos = Data.mLookatPos;
	pos.x += sinf(rotY) * cosf(rotX) * Data.mDistanceFromLookatPos;
	pos.y += sinf(rotX) * Data.mDistanceFromLookatPos;
	pos.z += cosf(rotY) * cosf(rotX) * Data.mDistanceFromLookatPos;

	//�������ԍ��ŃX���[�Y�ɃJ�����̈ʒu��ύX

	pos.x = prePos.x + (pos.x - prePos.x) * Data.mChangePosSpeed;
	pos.z = prePos.z + (pos.z - prePos.z) * Data.mChangePosSpeed;
	if (!GetGame()->GetCannon())
	{
		pos.y = prePos.y + (pos.y - prePos.y) * Data.mChangePosSpeed;
	}

	MATRIX view;
	view.camera(pos, Data.mLookatPos, Data.mUpVec);
	GetGame()->GetRenderer()->SetView(view);

	SetRotationY(rotY);
	SetRotationX(rotX);
	SetPosition(pos);

}