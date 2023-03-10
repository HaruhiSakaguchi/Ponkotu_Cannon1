#include "Stage1.h"
#include "Game.h"
#include "StageClear.h"
#include "UIProgressGauge.h"
#include "Items.h"
#include "COLLISION_MAP.h"
#include "Cannon.h"
#include "TreeMeshComponent.h"
#include "rand.h"
#include "UILog.h"
#include "PlayerHome.h"
#include "EnemyHome.h"

Stage1::Stage1(Game* game)
	: Map(game)
{
	SetUp();
	new PlayerHome(GetGame());
	EnemyHome*eh = new EnemyHome(GetGame());
	eh->SetPosition(-17.5f, 0.0f, 0.0f);
}

int Stage1::SetUp()
{
	SetCollisionText(Maptext1);

	CreateCollisions(Maptext1);
	GetGame()->SetCollisionMap(new COLLISION_MAP(0.5f, Maptext1));

	class TreeMeshComponent* tc = new TreeMeshComponent(this);
	tc->SetTree("Stage1");
	GetGame()->SetCannon(new Cannon(GetGame()));
	
	mMiniMap = new UIMiniMap(GetGame(), this, false);

	GetGame()->SetCamera(new Camera(GetGame()));

	SetClearCnt(30);
	SetCaracterCapa(5);

	mProgress = new UIProgressGauge(this, GetGame());

	//mModelCollisionsはマップ生成したら必要ないのでプレイするときだけ全消去

	while (!GetModelCollisions().empty())
	{
		delete GetModelCollisions().back();
	}

	return 0;
}
