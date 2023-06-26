#include "CameraManager.h"
#include "Camera.h"
#include "NormalCamera.h"
#include "Game.h"
#include "PlayerHome.h"
#include "LookFieldCenterCamera.h"
#include "LookPHomeCamera.h"

CameraManager::CameraManager(Game* game)
	: Manager(game)
	, mCurCamera(nullptr)
	, mPos(0.0f,0.0f,0.0f)
	, mSensitivityX(0.0f)
	, mSensitivityY(0.0f)

{
	SetUp();
	mGame->SetCameraManager(this);
}

CameraManager::~CameraManager()
{
	while (!mCameras.empty())
	{
		mCameras.pop_back();
	}

	mGame->SetCameraManager(nullptr);
}

int CameraManager::SetUp()
{
	mSensitivityX = mGame->GetAllData()->cameraData.mRotSpeedX;
	mSensitivityY = mGame->GetAllData()->cameraData.mRotSpeedY;

	mCameras.emplace_back(new NormalCamera(mGame));
	mCurCamera = mCameras.back();
	mCameras.emplace_back(new LookFieldCenterCamera(mGame));
	mCameras.emplace_back(new LookPHomeCamera(mGame));

	return 0;
}

void CameraManager::Update()
{
	VECTOR prePos = mCurCamera->GetPosition();

	if (mGame->GetActorManager()->GetPHome())
	{
		if ((!mGame->GetActorManager()->GetPHome()->GetMoveCompleteFlag() || mGame->GetActorManager()->GetPHome()->GetDore()->GetBeginOpen()) || !mGame->GetActorManager()->GetPHome()->GetDore()->GetCloseComplete())
		{
			mCurCamera = mCameras[2];
		}
		else if (mGame->GetActorManager()->GetPHome()->GetGenerateFlag())
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

	mPos.x = prePos.x + (mPos.x - prePos.x) * mGame->GetAllData()->cameraData.mChangePosSpeed;
	mPos.y = prePos.y + (mPos.y - prePos.y) * mGame->GetAllData()->cameraData.mChangePosSpeed;
	mPos.z = prePos.z + (mPos.z - prePos.z) * mGame->GetAllData()->cameraData.mChangePosSpeed;

	MATRIX view;
	view.camera(mPos, mCurCamera->GetLookatPos(), mGame->GetAllData()->cameraData.mUpVec);
	mGame->GetRenderer()->SetView(view);

}
