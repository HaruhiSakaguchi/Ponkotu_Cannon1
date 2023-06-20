#include "Stage2.h"
#include "Cannon.h"
#include "Camera.h"
#include "Game.h"
#include "UIProgressGauge.h"
#include "UIMiniMap.h"
#include "COLLISION_MAP.h"
#include "Satellite.h"
#include "TreeMeshComponent.h"
#include "rand.h"

Stage2::Stage2(class Game* game)
	:Map(game)
{
	SetUp();
}

int Stage2::SetUp()
{
	SetCollisionText(Maptext2);
	CreateCollisions(Maptext2);

	class TreeMeshComponent* tc = new TreeMeshComponent(this);
	tc->SetTree("Stage2");

	GetGame()->SetCollisionMap(new COLLISION_MAP(0.5f, Maptext2));

	new Cannon(GetGame());

	mMiniMap = new UIMiniMap(GetGame(), this);

	GetGame()->SetCamera(new Camera(GetGame()));

	SetCaracterCapa(5);
	SetClearCnt(30);

	mProgress = new UIProgressGauge(this, GetGame());

	while (!GetModelCollisions().empty())
	{
		delete GetModelCollisions().back();
	}

	return 0;
}
