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
	void LoadText(const char* fileName);
	void SaveText(const char* fileName);
private:
	std::vector<Camera*>mCameras;
	VECTOR mPos;
	class Camera* mCurCamera;
private:
	//ÉJÉÅÉâä¥ìx
	float mSensitivityX;
	float mSensitivityY;
	const char* mSaveFileName = "Assets\\option\\Camera.txt";
public:
	void SetCameraSX(float sx) { mSensitivityX = sx; }
	void SetCameraSY(float sy) { mSensitivityY = sy; }
	float GetCameraSX() { return mSensitivityX; }
	float GetCameraSY() { return mSensitivityY; }
};

