#include "LookPHomeCamera.h"
#include "Camera.h"
#include "Game.h"
#include "Renderer.h"
#include "input.h"
#include "Cannon.h"
#include "PlayerHome.h"
#include "TreeMeshComponent.h"

LookPHomeCamera::LookPHomeCamera(Game* game)
	: Camera(game)
{
}

void LookPHomeCamera::UpdateActor()
{
	VECTOR pos = GetPosition();

	if (GetGame()->GetActorManager()->GetPHome())
	{
		SetPosition(GetGame()->GetActorManager()->GetPHome()->GetPosition());
		Data.mLookatPos = GetGame()->GetActorManager()->GetPHome()->GetPosition();
	}
	pos = GetPosition() + VECTOR(0.0f, 5.0f, -10.0f);

	SetPosition(pos);
}