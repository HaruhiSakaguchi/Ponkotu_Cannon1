#include "DebugStage.h"
#include "Game.h"
#include "StageClear.h"
//#include "UIProgressGauge.h"
#include "Items.h"
#include "COLLISION_MAP.h"
#include "Cannon.h"
#include "TreeMeshComponent.h"
#include "rand.h"
#include "UILog.h"
#include "PlayerHome.h"
#include "EnemyHome.h"
#include "Barricade.h"

DebugStage::DebugStage(Game* game)
	: Map(game)
{
	SetUp();
	PlayerHome* ph = new PlayerHome(GetGame());
	ph->SetPosition(0.0f, 0.0f, -3.0f);
	EnemyHome* eh = new EnemyHome(GetGame());
	eh->SetPosition(0.0f, 0.0f, -51.0f);
	eh->SetRotationY(3.1415926f);
}

int DebugStage::SetUp()
{
	SetCollisionText(Maptext1);

	CreateCollisions(Maptext1);
	GetGame()->SetCollisionMap(new COLLISION_MAP(0.5f, Maptext1));

	class TreeMeshComponent* tc = new TreeMeshComponent(this);
	tc->SetTree("background");
	GetGame()->SetCannon(new Cannon(GetGame()));
	GetGame()->GetCannon()->SetUp();
	GetGame()->GetCannon()->SetPosition(VECTOR(0.0f, 0.0f, -30.0f));
	GetGame()->GetCannon()->SetRotationY(3.1415926f);

	class Cannon* c = new Cannon(GetGame());
	c->SetUp();
	c->SetPosition(VECTOR(5.0f, 0.0f, -30.0f));
	c->SetRotationY(3.1415926f);
	c = new Cannon(GetGame());
	c->SetUp();
	c->SetPosition(VECTOR(-5.0f, 0.0f, -30.0f));
	c->SetRotationY(3.1415926f);

	mMiniMap = new UIMiniMap(GetGame(), this);

	GetGame()->SetCamera(new Camera(GetGame()));

	SetClearCnt(30);
	SetCaracterCapa(5);


	class Barricade* b = new Barricade(GetGame());
	b->SetPosition(0, 5.0f, -35.0f);
	b = new Barricade(GetGame());
	b->SetPosition(5.0f, 5.0f, -35.0f);
	b = new Barricade(GetGame());
	b->SetPosition(-5.0f, 5.0f, -35.0f);

	//mProgress = new UIProgressGauge(this, GetGame());

	//mModelCollisionsはマップ生成したら必要ないのでプレイするときだけ全消去

	while (!GetModelCollisions().empty())
	{
		delete GetModelCollisions().back();
	}

	return 0;
}