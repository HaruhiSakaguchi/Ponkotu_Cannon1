#pragma once
#include "Actor.h"
#include "Camera.h"
#include "Manager.h"

class CameraManager
	: public Manager
{
public:
	CameraManager(class Game* game);
	~CameraManager();
	int SetUp();
	void Update();
	Camera* GetCurCamera() { return mCurCamera; }
private:
	std::vector<Camera*>mCameras;
	VECTOR mPos;
	class Camera* mCurCamera;
private:
	//ƒJƒƒ‰Š´“x
	float mSensitivityX;
	float mSensitivityY;
public:
	void SetCameraSX(float sx) { mSensitivityX = sx; }
	void SetCameraSY(float sy) { mSensitivityY = sy; }
	float GetCameraSX() { return mSensitivityX; }
	float GetCameraSY() { return mSensitivityY; }
};

