#include "Stage1.h"
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

Stage1::Stage1(Game* game)
	: Map(game)
{
	SetUp();
	PlayerHome*ph = new PlayerHome(GetGame());
	ph->SetPosition(0.0f, 0.0f, -3.0f);
	EnemyHome*eh = new EnemyHome(GetGame());
	eh->SetPosition(0.0f, 0.0f, -51.0f);
	eh->SetRotationY(3.1415926f);
}

int Stage1::SetUp()
{
	SetCollisionText(Maptext1);

	CreateCollisions(Maptext1);
	//GetGame()->SetCollisionMap(new COLLISION_MAP(0.5f, Maptext1));

	class TreeMeshComponent* tc = new TreeMeshComponent(this);
	tc->SetTree("background");
	auto c = new Cannon(GetGame());
	c->SetPosition(VECTOR(0.0f, 0.0f, 0.0f));
	mMiniMap = new UIMiniMap(GetGame(), this);

	/*GetGame()->SetCamera(new Camera(GetGame()));*/

	class Barricade*b = new Barricade(GetGame());
	b->SetPosition(0, 5.0f, -5.0f);
	b = new Barricade(GetGame());
	b->SetPosition(-15.0f, 5.0f, -5.0f);

	//mProgress = new UIProgressGauge(this, GetGame());

	//mModelCollisionsはマップ生成したら必要ないのでプレイするときだけ全消去

	while (!GetModelCollisions().empty())
	{
		delete GetModelCollisions().back();
	}

	return 0;
}
