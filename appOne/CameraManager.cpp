#include "CameraManager.h"
#include "Camera.h"
#include "NormalCamera.h"
#include "Game.h"
#include "PlayerHome.h"
#include "LookFieldCenterCamera.h"
#include "LookPHomeCamera.h"

CameraManager::CameraManager(Game* game)
	:Actor(game)
{
	SetUp();
}

CameraManager::~CameraManager()
{
	while (!mCameras.empty())
	{
		mCameras.pop_back();
	}
}

int CameraManager::SetUp()
{
	mCameras.emplace_back(new NormalCamera(GetGame()));
	GetGame()->SetCamera(mCameras.back());
	mCameras.emplace_back(new LookFieldCenterCamera(GetGame()));
	mCameras.emplace_back(new LookPHomeCamera(GetGame()));

	return 0;
}

void CameraManager::UpdateActor()
{
	VECTOR prePos = GetGame()->GetCamera()->GetPosition();

	if (GetGame()->GetPHome())
	{
		if ((!GetGame()->GetPHome()->GetMoveCompleteFlag() || GetGame()->GetPHome()->BeginOpen()) || !GetGame()->GetPHome()->GetCloseComplete())
		{
			GetGame()->SetCamera(mCameras[2]);
		}
		else if (GetGame()->GetPHome()->GetGenerateFlag())
		{
			GetGame()->SetCamera(mCameras[1]);
		}
		else
		{
			GetGame()->SetCamera(mCameras[0]);
		}
	}
	else
	{
		GetGame()->SetCamera(mCameras[0]);
	}

	mPos.x = prePos.x + (mPos.x - prePos.x) * GetGame()->GetAllData()->cameraData.mChangePosSpeed;
	mPos.y = prePos.y + (mPos.y - prePos.y) * GetGame()->GetAllData()->cameraData.mChangePosSpeed;
	mPos.z = prePos.z + (mPos.z - prePos.z) * GetGame()->GetAllData()->cameraData.mChangePosSpeed;

	MATRIX view;
	view.camera(mPos, static_cast<Camera*>(GetGame()->GetCamera())->GetLookatPos(), GetGame()->GetAllData()->cameraData.mUpVec);
	GetGame()->GetRenderer()->SetView(view);
}
