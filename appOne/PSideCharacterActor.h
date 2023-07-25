#pragma once
#include "CharacterActor.h"
#include "UIPSideCharacterStatusBase.h"
#include "ItemComponent.h"
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
	class PowerUpCompo* GetPower() { return mPower; }
	class SpeedUpCompo* GetSpeed() { return mSpeed; }
	class BarrierCompo* GetBarrier() { return mBarrier; }
	class RapidFireCompo* GetRapid() { return mRapid; }
	void SetPower(class PowerUpCompo* power) { mPower = power; }
	void SetSpeed(class SpeedUpCompo* speed) { mSpeed = speed; }
	void SetBarrier(class BarrierCompo* barrier) { mBarrier = barrier; }
	void SetRapid(class RapidFireCompo* rapid) { mRapid = rapid; }
protected:
	//ƒAƒCƒeƒ€‚Ì•ª—Þ
	class PowerUpCompo* mPower;
	class SpeedUpCompo* mSpeed;
	class BarrierCompo* mBarrier;
	class RapidFireCompo* mRapid;
private:
	int mNum;
	class UIPSideCharacterStatusBase* mUI;
};

