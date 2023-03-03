#pragma once
#include "UIScreen.h"
#include "Map.h"

class UIProgressGauge : public UIScreen
{
public:
	UIProgressGauge(class Map* owner, class Game* game);
	void Update()override;
	void Draw()override;
	struct DATA
	{
		VECTOR2 mPos;
		int mLimmit;
		float mGaugeWidth;
		float mGaugeHeight;
		float mSw;
		float mTextSize;
		float mChangeProgressSpeed;
		COLOR mBackColor;
		COLOR mPointColor;
		COLOR mTextColor;
		COLOR mCompleteColor;
	};
private:
	DATA Data;
	class Map* mOwner;
};