#include "SatelliteBullet.h"
#include "Game.h"
#include "Container.h"
#include "BatchMeshComponent.h"

SatelliteBullet::SatelliteBullet(Satellite* satellite, const VECTOR& pos, const VECTOR& dir)
	:SphereWeapon(satellite, pos, dir)
{
	Data = GetGame()->GetAllData()->sBulletData;
	iData = GetGame()->GetAllData()->sBulletIData;

	SetUp(Data);

	BatchMeshComponent* bc = new BatchMeshComponent(this);
	bc->SetBatch("SatelliteBulletSphere");

	if (GetGame()->GetScene() == Game::EPlay && GetGame()->GetState() == Game::EGameplay)
	{
		setVolume(iData.mLaunchSound, GetGame()->GetEffectVolume());
		playSound(iData.mLaunchSound);
	}
}

void SatelliteBullet::UpdateActor()
{
	if (GetGame()->GetCannon() && Intersect(this, GetGame()->GetCannon(), true))
	{
		SetState(Actor::EDead);
		setVolume(iData.mImpactSound, GetGame()->GetEffectVolume());
		playSound(iData.mImpactSound);
		static_cast<class Cannon*>(GetGame()->GetCannon())->Damage(GetOwner());
	}

	for (auto pSide : GetGame()->GetPSide())
	{
		if (pSide != GetGame()->GetCannon() && Intersect(this,pSide,false))
		{
			SetState(Actor::EDead);
			setVolume(iData.mImpactSound, GetGame()->GetEffectVolume());
			playSound(iData.mImpactSound);
			pSide->Damage(1);
		}
	}

	if (!GetOnMapFlag())
	{
		SetState(Actor::EDead);
	}

	SphereWeapon::UpdateActor();
}

