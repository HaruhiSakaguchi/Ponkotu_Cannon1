#pragma once
#include "SphereWeapon.h"

class SatelliteBullet :
	public SphereWeapon
{
public:
	SatelliteBullet(class Satellite* satellite, const VECTOR& pos, const VECTOR& dir);
	void UpdateActor() override;
	struct IDATA
	{
		int mLaunchSound;
		int mImpactSound;
	};
private:
	IDATA iData;
	int mCnt;
	int mChangeFlame = 50;
	VECTOR mDefDir;
};

