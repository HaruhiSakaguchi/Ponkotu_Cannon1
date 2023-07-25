#pragma once
#include "ItemObject.h"
#include "ItemEffect.h"
#include "sound.h"

class Recovery : public ItemObject
{
public:
	Recovery(class Game* game);
	bool update()override;
private:
	int mRecoverySoundVolumeOffset = -450;
};

class RecoveryEffect : public ItemEffect
{
public:
	RecoveryEffect(class PSideCharacterActor* owner);
private:
	int mRecoveryHp = 3;
};

class SpeedUp : public ItemObject
{
public:
	SpeedUp(class Game* game);
	bool update()override;
private:
	int mSpeedUpSoundVolumeOffset = -800;
};

class SpeedUpEffect : public ItemEffect
{
public:
	SpeedUpEffect(class PSideCharacterActor* owner);
	~SpeedUpEffect();
	void UpdateActor()override;
	void Dead()override;
private:
	float mSpeedUpRate = 0.1f;
};

class PowerUp : public ItemObject
{
public:
	PowerUp(class Game* game);
	bool update()override;
private:
	int mPowerUpSoundVolumeOffset = -450;
};

class PowerUpEffect : public ItemEffect
{
public:
	PowerUpEffect(class PSideCharacterActor* owner);
	~PowerUpEffect();
	void UpdateActor()override;
	void Dead()override;
};

class RapidFire : public ItemObject
{
public:
	RapidFire(class Game* game);
	bool update()override;
private:
	int mRapidFireSoundVolumeOffset = -450;
};

class RapidFireEffect : public ItemEffect
{
public:
	RapidFireEffect(class PSideCharacterActor* owner);
	~RapidFireEffect();
	void UpdateActor()override;
	void Dead()override;
private:
	float mLaunchIntervalDecreaseRate = 0.1f;
};

class Barrier : public ItemObject
{
public:
	Barrier(class Game* game);
	bool update()override;
private:
	int mBarrierSoundVolumeOffset = -750;
};

class BarrierEffect : public ItemEffect
{
public:
	BarrierEffect(class PSideCharacterActor* owner);
	~BarrierEffect();
	void UpdateActor()override;
	void Dead()override;
};