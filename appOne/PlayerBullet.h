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
		//�T�E���h�{�̂Ƃ��ꂼ��̌��ʉ��̉��ʂ̒����l
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

