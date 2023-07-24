#include "DebugStage.h"
#include "Game.h"
#include "StageClear.h"
#include "Items.h"
#include "COLLISION_MAP.h"
#include "Cannon.h"
#include "TreeMeshComponent.h"
#include "rand.h"
#include "UILog.h"
#include "PlayerHome.h"
#include "EnemyHome.h"
#include "Barricade.h"
#include "CameraManager.h"
#include "PlayerFlag.h"
#include "CapsuleComponent.h"

DebugStage::DebugStage(Game* game)
	: Map(game)
{
	//SetUp();
	SetCollisionText(Maptext1);

	CreateCollisions(Maptext1);
	SetCollisionMap(new COLLISION_MAP(0.5f, Maptext1));
	
	class TreeMeshComponent* tc = new TreeMeshComponent(this);
	tc->SetTree("background");
	mMiniMap = new UIMiniMap(GetGame(), this);

	PlayerHome* ph = new PlayerHome(GetGame(),VECTOR(0.0f, 0.0f, -3.0f));
	ph->CreateHomeArea();

	EnemyHome* eh = new EnemyHome(GetGame());
	eh->SetPosition(0.0f, 0.0f, -51.0f);
	eh->SetRotationY(3.1415926f);

	eh->CreateHomeArea();

	while (!GetModelCollisions().empty())
	{
		delete GetModelCollisions().back();
	}
}

int DebugStage::SetUp()
{

	auto c = new Cannon(GetGame());
	c->SetUp();
	c->SetInitPosition(VECTOR(0.0f, 0.0f, -30.0f));
	c->SetPosition(GetGame()->GetActorManager()->GetPHome()->GetPosition());
	c->SetRotationY(3.1415926f);

	//mModelCollisionsはマップ生成したら必要ないのでプレイするときだけ全消去


	return 0;
}