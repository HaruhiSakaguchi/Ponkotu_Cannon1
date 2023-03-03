#pragma once
#include "SphereWeapon.h"

class PlayerBullet :
	public SphereWeapon
{
public:
	PlayerBullet(class Cannon* cannon, const VECTOR& pos, const VECTOR& dir);
	void UpdateActor()override;
	void damage()override;
	struct IDATA
	{
		//サウンド本体とそれぞれの効果音の音量の調整値
		int mLaunchSound;
		int mLaunchSoundVolumeOffset;
		int mImpactSound;
		int mImpactSoundVolumeOffset;
		int mItemStockSound;
		int mItemStockSoundVolumeOffset;

	};
private:
	IDATA iData;
};

