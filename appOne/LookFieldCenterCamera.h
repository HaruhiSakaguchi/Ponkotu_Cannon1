#pragma once
#include "Camera.h"
class LookFieldCenterCamera :
    public Camera
{
public:
	LookFieldCenterCamera(class Game* game);
	void UpdateActor() override;
};

