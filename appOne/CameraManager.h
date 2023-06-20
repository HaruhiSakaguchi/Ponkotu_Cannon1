#pragma once
#include "Actor.h"
#include "Camera.h"

class CameraManager :
	public Actor
{
public:
	CameraManager(class Game* game);
	~CameraManager();
	int SetUp()override;
	void UpdateActor()override;
	Camera* GetCurCamera() { return mCurCamera; }
private:
	std::vector<Camera*>mCameras;
	VECTOR mPos;
	class Camera* mCurCamera;
};

