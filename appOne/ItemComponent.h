#pragma once
#include "Actor.h"
#include "COLOR.h"
#include <string>
#include "sound.h"
#include <chrono>

class ItemComponent : public Actor
{
public:
	ItemComponent(class CharacterActor* owner);
	virtual ~ItemComponent();
	void UpdateActor()override;
	void Dead()override;
	int GetLevel() { return Data.mLevel; }
	void SetLevel(int level) { Data.mLevel = level; }
	int GetMaxLevel() { return Data.mMaxLevel; }
	float GetInterval() { return Data.mInterval; }
	float GetTime() { return Data.mTime; }
	void SetInterval(float interval) { Data.mInterval = interval; }
	void SetTime(float time) { Data.mTime = time; }
	const COLOR& GetColor() { return Data.mColor; }
	void SetColor(const COLOR& color) { Data.mColor = color; }
	std::string GetName() { return Data.mName; }
	int GetHp() { return Data.mHp; }
	void SetHp(int hp) { Data.mHp = hp; }
	void SetMaxHp(int hp) { Data.mMaxHp = hp; }
	int GetMaxHp() { return Data.mMaxHp; }
	int GetNumber() { return cData.mNumber; }
	void SetNumber(int num) { cData.mNumber = num; }
	float GetKeepTime() { return cData.mKeepTime; }
	void SetKeepTime(float time) { cData.mKeepTime = time; }
	void SetUpLifeTime(float time) { mUpTime = time; }
	void TimeReset() { mStart = std::chrono::system_clock::now(); }
	struct IDATA
	{
		int mLevel;
		int mMaxLevel;
		float mTime;
		float mInterval;
		int mHp;
		int mMaxHp;
		std::string mName;
		COLOR mColor;
	};
	struct CDATA {
		float mUIOffsetPosY;
		float mUIPosY;
		float mUIPosAdvSpeed;
		float mUIMinPosX;
		float mUIPosChangeSpeed;
		int mNumber;
		float mKeepTime;
	};
private:
	CDATA cData;
protected:
	IDATA Data;
	class CharacterActor* mOwner;
	const char* mMeshName;
	std::chrono::system_clock::time_point mStart;
	float mUpTime;
	int mNextParticleSpawnTime;
	bool mIsSpawnParticle;
	int mBeforeParticleSpawnTime;
};