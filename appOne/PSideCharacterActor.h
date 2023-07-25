#pragma once
#include "CharacterActor.h"
#include "UIPSideCharacterStatusBase.h"
#include "Items.h"

class PSideCharacterActor :
	public CharacterActor
{
public:
	PSideCharacterActor(class Game* game);
	~PSideCharacterActor();
	virtual void Dead()override;
	void SetUI(UIPSideCharacterStatusBase* ui) { mUI = ui; }

	void SetNum(int num) { mNum = num; }
	int GetNum() { return mNum; }
	class PowerUpEffect* GetPower() { return mPower; }
	class SpeedUpEffect* GetSpeed() { return mSpeed; }
	class BarrierEffect* GetBarrier() { return mBarrier; }
	class RapidFireEffect* GetRapid() { return mRapid; }
	void SetPower(class PowerUpEffect* power) { mPower = power; }
	void SetSpeed(class SpeedUpEffect* speed) { mSpeed = speed; }
	void SetBarrier(class BarrierEffect* barrier) { mBarrier = barrier; }
	void SetRapid(class RapidFireEffect* rapid) { mRapid = rapid; }
protected:
	//ƒAƒCƒeƒ€‚Ì•ª—Þ
	class PowerUpEffect* mPower;
	class SpeedUpEffect* mSpeed;
	class BarrierEffect* mBarrier;
	class RapidFireEffect* mRapid;
private:
	int mNum;
	class UIPSideCharacterStatusBase* mUI;
};

