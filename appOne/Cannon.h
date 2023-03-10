#pragma once
#include "CharacterActor.h"
#include "UIHpGauge.h"
#include "UIScope.h"
#include "StateComponent.h"
#include "sound.h"
#include "ItemComponent.h"

class Cannon :
	public CharacterActor
{
public:
	Cannon(class Game* game);
	~Cannon();
	int SetUp()override;
	void UpdateActor()override;
	const VECTOR& GetTargetPosition();
	void Damage(Actor* actor = nullptr);
	void FallOption()override;

	//setter,getter
	void SetBarrelDir(const VECTOR& dir) { Data.mBarrelDir = dir; }
	float GetTimer() { return Data.mTimer; }
	float GetInterval() { return Data.mInterval; }
	void SetTimer(float timer) { Data.mTimer = timer; }
	void SetInterval(float interval) { Data.mInterval = interval; }
	void SetWheelRotateZ(float rotate) { Data.mWheelAngleZ = rotate; }
	float GetWheelRotateZ() { return Data.mWheelAngleZ; }
	void SetWheelRotateX(float rotate) { Data.mWheelAngleX = rotate; }
	float GetWheelRotateX() { return Data.mWheelAngleX; }
	void DamageOption()override;
	void SetDir(const VECTOR& dir) { Data.mDir = dir; }
	void SetDamage(int damage) { Data.mDamage = damage; }
	int GetDamage() { return Data.mDamage; }
	void SetRDamage(int damage) { Data.mRDamage = damage; }
	int GetRDamage() { return Data.mRDamage; }
	int GetMaxHp() { return Data.mMaxHp; }

	class InputComponent* GetIn() { return mIn; }
	class UIScope* GetScope() { return mScope; }
	class UIHpGauge* GetHpGauge() { return mHpGauge; }
	class PowerUpCompo* GetPower() { return mPower; }
	class SpeedUpCompo* GetSpeed() { return mSpeed; }
	class BarrierCompo* GetBarrier() { return mBarrier; }
	class RapidFireCompo* GetRapid() { return mRapid; }
	void SetPower(class PowerUpCompo* power) { mPower = power; }
	void SetSpeed(class SpeedUpCompo* speed) { mSpeed = speed; }
	void SetBarrier(class BarrierCompo* barrier) { mBarrier = barrier; }
	void SetRapid(class RapidFireCompo* rapid) { mRapid = rapid; }

	void AddItemNum(int num);
	void RemoveItemNum(int num);
	void SetCnt(int cnt) { this->mCnt = cnt; }
	const int GetCnt() { return mCnt; }
	int GetJumpSound() { return Data.mJumpSound; }
	const std::vector<int>& GetItemNums() { return mItemNums; }
	int GetSlide() { return Data.mSlide; }
	int GetOutOfBullets() { return Data.mOutOfBullets; }
	int GetSlideCnt() { return mSlideCnt; }
	void SetSlideCnt(int cnt) { mSlideCnt = cnt; }

	void AddItemComponent(class ItemComponent*);
	void RemoveItemComponent(class ItemComponent*);
	std::vector<class ItemComponent*>GetItemComponents() { return mItemComponents; }
	void PlayFallSound() { playSound(Data.mFallSound); }
	void SetLaunchTime(float time) { mLaunchTime = time; }
	void StopFallSound() { stopSound(Data.mFallSound); }
	void OnMapOff() { mOnMap = false; }
	//data
	struct DATA {
		VECTOR mOffsetPos;
		VECTOR mTargetPos;
		VECTOR mBarrelDir;
		VECTOR mInitPos;
		VECTOR mTarGetPos;
		VECTOR mCapsulOffset;
		VECTOR mDir;
		float mAmbient;
		float mAdvSpeed;
		float mJumpVelocity;
		float mGravity;
		float mTimer;
		float mInterval;
		float mRange;
		float mWheelAngleZ;
		float mWheelAngleX;
		float mMaxDamageInterval;
		float mCannonCapsulHeight;
		float mCannonCapsulRadius;
		float mChangeScaleSpeed;
		int mMaxHp;
		int mJumpFlag;
		int mDamage;//?^?_??
		int mRDamage;//???_??
		int mWheelCorners;
		float mWheelFrontZOffset;
		float mWheelBackZOffset;
		float mWheelAngleOffset;
		float mLaunchTimeDecreaseAdvSpeed;
		float mLaunchBodyScale;
		float mNormalBodyScale;
		int mItemStockMaxSize;
		VECTOR mWheelOffsetPos;
		VECTOR mBodyOffsetPos;
		COLOR mDisplayColor;
		COLOR mImageColor;
		COLOR mGurdColor;
		//?????f?[?^
		int mJumpSound = 0;
		int mJumpSoundVolumeOffset;
		int mDamageSound = 0;
		int mGurdSound = 0;
		int mCyakuchi = 0;
		int mSlide = 0;
		int mFallSound = 0;
		int mFallSoundVolumeOffset;
		int mOutOfBullets = 0;
	};
private:
	//Data
	DATA Data;
	//component
	class InputComponent* mIn;
	class StateComponent* mState;

	//?A?C?e????????
	class PowerUpCompo* mPower;
	class SpeedUpCompo* mSpeed;
	class BarrierCompo* mBarrier;
	class RapidFireCompo* mRapid;

	//ui
	class UIHpGauge* mHpGauge;
	class UIScope* mScope;
	class UIItemStatus* mUIItem;

	//Hierarchy Matrix
	MATRIX Master, Body, Target;

	std::vector<int>mItemNums;
	std::vector<class ItemComponent*> mItemComponents;

	//?T?E???h???A
	bool mJumpSoundFlag;

	int mCnt;
	int mSlideCnt;
	bool mOnMap;
	float mScale;
	float mLaunchTime;

	class CannonWheelL* mWheelL;
	class CannonWheelR* mWheelR;


};