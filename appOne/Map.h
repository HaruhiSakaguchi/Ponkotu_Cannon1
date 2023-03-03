#pragma once
#include "Actor.h"
#include "MODEL_COLLISION.h"
#include "UIMiniMap.h"
#include "UIProgressGauge.h"
#include <chrono>
#include <string>

class Map :
	public Actor
{
public:
	Map(class Game* game);
	virtual ~Map();
	void UpdateActor()override;
	void AddCollisions(class MODEL_COLLISION* model);
	void RemoveCollisions(class MODEL_COLLISION* model);
	void Draw();
	float* GetMaxX() { return mMaxX; }
	float* GetMaxZ() { return mMaxZ; }
	float* GetMinX() { return mMinX; }
	float* GetMinZ() { return mMinZ; }
	int GetAreaNum() { return mAreaNum; }

	//マップモデルを作る
	void CreateCollisions(const char* filename);

	//getter,setter

	const std::vector<class MODEL_COLLISION*>& GetModelCollisions() { return mModelCollisions; }
	class UIMiniMap* GetMiniMap() { return mMiniMap; }

	//Mapを継承したクラスで最初に呼び出すマップデータロード用関数
	void SetCollisionText(const char* text) { Data.CollisionText = text; }

	void CreateArea(float* maxX, float* maxZ, float* minX, float* minZ, int areaNum);

	void SetClearCnt(int cnt) { Data.mClearCnt = cnt; }
	int GetClearCnt() { return Data.mClearCnt; }

	float GetStageMinX() { return Data.mStageMinX; }
	float GetStageMaxX() { return Data.mStageMaxX; }
	float GetStageMinZ() { return Data.mStageMinZ; }
	float GetStageMaxZ() { return Data.mStageMaxZ; }

	void SetCaracterCapa(int num) { Data.mStageCharacterCapa = num; }
	int GetSecond() { return mSecond; }
	int GetMunite() { return mMunite; }
	std::string& GetTimeText() { return mTimeText; }
	class UILog* GetLog();
	std::vector<std::string>& GetTexts() { return mTexts; }
public:
	void AddText(std::string text);
	void RemoveText(int num);
private:
	float* mMaxX;
	float* mMaxZ;
	float* mMinX;
	float* mMinZ;
	int mAreaNum;
protected:
	//ui
	class UIMiniMap* mMiniMap;
	class UIProgressGauge* mProgress;
	class UILog* mLog;
private:
	std::vector<class MODEL_COLLISION*> mModelCollisions;
public:
	struct DATA
	{
		int mStageNum;
		const char* CollisionText;
		float mStageNumTextSize;
		COLOR mStageNumRectColor;
		VECTOR2 mStageNumRectPos;
		COLOR mStageNumTextColor;
		float mTextNumTextSize;
		VECTOR2 mStageNumTextPos;
		//クリア条件のカウンター
		int mClearCnt;
		float mStageMaxX;
		float mStageMinX;
		float mStageMaxZ;
		float mStageMinZ;
		int mStageCharacterCapa;
	};
private:
	DATA Data;
    int mSecond;
	int mMunite;
	std::chrono::system_clock::time_point mStart;
	std::string mTimeText;
	std::vector<std::string>mTexts;
	bool mInitItemDropFlag;
	bool mClearFlag;
};

