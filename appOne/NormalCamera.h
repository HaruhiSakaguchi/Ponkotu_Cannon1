#pragma once
#include "Camera.h"
class NormalCamera :
    public Camera
{
public:
	NormalCamera(class Game* game);
	void UpdateActor() override;
};


