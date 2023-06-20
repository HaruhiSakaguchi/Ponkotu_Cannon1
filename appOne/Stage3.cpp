#include "Stage3.h"
#include "Cannon.h"
#include "Camera.h"
#include "Tama.h"
#include "Satellite.h"
#include "Game.h"
#include "UIProgressGauge.h"
#include "UIMiniMap.h"
#include "COLLISION_MAP.h"
#include "TreeMeshComponent.h"
#include "rand.h"

Stage3::Stage3(class Game* game)
	:Map(game)
{
	SetUp();
}

int Stage3::SetUp()
{
	SetCollisionText(Maptext3);
	CreateCollisions(Maptext3);

	//GetGame()->SetCollisionMap(new COLLISION_MAP(1.0f, Maptext3));

	class TreeMeshComponent* tc = new TreeMeshComponent(this);
	tc->SetTree("Stage3");

	new Cannon(GetGame());

	mMiniMap = new UIMiniMap(GetGame(), this);

	/*GetGame()->SetCamera(new Camera(GetGame()));*/

	SetPosition(VECTOR(0, 0, 0));

	SetClearCnt(40);
	SetCaracterCapa(10);

	mProgress = new UIProgressGauge(this, GetGame());

	while (!GetModelCollisions().empty())
	{
		delete GetModelCollisions().back();
	}


	return 0;
}

