#include "CameraManager.h"
#include "Camera.h"
#include "NormalCamera.h"
#include "Game.h"
#include "PlayerHome.h"
#include "LookFieldCenterCamera.h"
#include "LookPHomeCamera.h"
#include "FILE_BUFFER.h"
#include "NAME.h"

CameraManager::CameraManager(Game* game)
	: Manager(game)
	, mCurCamera(nullptr)
	, mPos(0.0f, 0.0f, 0.0f)
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

	SaveText(mSaveFileName);
}

int CameraManager::SetUp()
{
	mCameras.emplace_back(new NormalCamera(mGame));
	mCurCamera = mCameras.back();
	mCameras.emplace_back(new LookFieldCenterCamera(mGame));
	mCameras.emplace_back(new LookPHomeCamera(mGame));

	LoadText(mSaveFileName);

	return 0;
}

void CameraManager::Update()
{
	if (mGame->GetState() == Game::GameState::EGameplay && mGame->GetCurState()->GetState() == UIMainState::State::EGamePlay)
	{
		VECTOR prePos = mCurCamera->GetPosition();

		if (mGame->GetActorManager()->GetPHome())
		{
			if ((mGame->GetActorManager()->GetPHome()->GetMoveCompleteFlag() == 0 || mGame->GetActorManager()->GetPHome()->GetDore()->GetBeginOpen()) || !mGame->GetActorManager()->GetPHome()->GetDore()->GetCloseComplete())
			{
				mCurCamera = mCameras[2];
			}
			else if (mGame->GetActorManager()->GetPHome()->GetGenerateFlag() == 1)
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
}

void CameraManager::LoadText(const char* fileName)
{
	FILE_BUFFER fb(fileName);
	NAME name;

	fb.readString();
	while (!fb.end()) {
		if (fb == "X")
		{
			mSensitivityX = fb.readFloat();
			fb.readString();
		}
		if (fb == "Y")
		{
			mSensitivityY = fb.readFloat();
		}
		
		fb.readString();
	}
}

void CameraManager::SaveText(const char* fileName)
{
	FILE* fp = NULL;
	fopen_s(&fp, fileName, "w");
	if (fp != NULL)
	{
		fprintf_s(fp, "%s %f\n", "X ", mSensitivityX);
		fprintf_s(fp, "%s %f\n", "Y ", mSensitivityY);
	}
	fclose(fp);
}
