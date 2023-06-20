#include "CameraManager.h"
#include "Camera.h"
#include "NormalCamera.h"
#include "Game.h"
#include "PlayerHome.h"
#include "LookFieldCenterCamera.h"
#include "LookPHomeCamera.h"

CameraManager::CameraManager(Game* game)
	: Actor(game)
	, mCurCamera(nullptr)
	, mPos(0.0f,0.0f,0.0f)
{
	SetUp();
	GetGame()->SetCameraManager(this);
}

CameraManager::~CameraManager()
{
	while (!mCameras.empty())
	{
		mCameras.pop_back();
	}

	GetGame()->SetCameraManager(nullptr);
}

int CameraManager::SetUp()
{
	mCameras.emplace_back(new NormalCamera(GetGame()));
	mCurCamera = mCameras.back();
	mCameras.emplace_back(new LookFieldCenterCamera(GetGame()));
	mCameras.emplace_back(new LookPHomeCamera(GetGame()));

	return 0;
}

void CameraManager::UpdateActor()
{
	VECTOR prePos = mCurCamera->GetPosition();

	if (GetGame()->GetPHome())
	{
		if ((!GetGame()->GetPHome()->GetMoveCompleteFlag() || GetGame()->GetPHome()->BeginOpen()) || !GetGame()->GetPHome()->GetCloseComplete())
		{
			mCurCamera = mCameras[2];
		}
		else if (GetGame()->GetPHome()->GetGenerateFlag())
		{
			mCurCamera = mCameras[1];
		}
		else
		{
			mCurCamera = mCameras[0];
		}
	}
	else
	{
		mCurCamera = mCameras[0];
	}

	mPos.x = prePos.x + (mPos.x - prePos.x) * GetGame()->GetAllData()->cameraData.mChangePosSpeed;
	mPos.y = prePos.y + (mPos.y - prePos.y) * GetGame()->GetAllData()->cameraData.mChangePosSpeed;
	mPos.z = prePos.z + (mPos.z - prePos.z) * GetGame()->GetAllData()->cameraData.mChangePosSpeed;

	MATRIX view;
	view.camera(mPos, mCurCamera->GetLookatPos(), GetGame()->GetAllData()->cameraData.mUpVec);
	GetGame()->GetRenderer()->SetView(view);
}
