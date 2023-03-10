#pragma once
#include "Item.h"
#include "ItemComponent.h"
#include "sound.h"

class Recovery : public Item
{
public:
	Recovery(class Game* game);
	bool update()override;
private:
	int mRecoverySoundVolumeOffset = -450;
};

class RecoveryCompo : public ItemComponent
{
public:
	RecoveryCompo(class Cannon* owner);
private:
	int mRecoveryHp = 3;
};

class SpeedUp : public Item
{
public:
	SpeedUp(class Game* game);
	bool update()override;
private:
	int mSpeedUpSoundVolumeOffset = -800;
};

class SpeedUpCompo : public ItemComponent
{
public:
	SpeedUpCompo(class Cannon* owner);
	~SpeedUpCompo();
	void Update()override;
private:
	float mSpeedUpRate = 0.1f;
};

class PowerUp : public Item
{
public:
	PowerUp(class Game* game);
	bool update()override;
private:
	int mPowerUpSoundVolumeOffset = -450;
};

class PowerUpCompo : public ItemComponent
{
public:
	PowerUpCompo(class Cannon* owner);
	~PowerUpCompo();
	void Update()override;
};

class RapidFire : public Item
{
public:
	RapidFire(class Game* game);
	bool update()override;
private:
	int mRapidFireSoundVolumeOffset = -450;
};

class RapidFireCompo : public ItemComponent
{
public:
	RapidFireCompo(class Cannon* owner);
	~RapidFireCompo();
	void Update()override;
private:
	float mLaunchIntervalDecreaseRate = 0.1f;
};

class Barrier : public Item
{
public:
	Barrier(class Game* game);
	bool update()override;
private:
	int mBarrierSoundVolumeOffset = -750;
};

class BarrierCompo : public ItemComponent
{
public:
	BarrierCompo(class Cannon* owner);
	~BarrierCompo();
	void Update()override;
};