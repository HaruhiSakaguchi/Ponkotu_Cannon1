#pragma once
#include "UIState.h"

class StageSelect : public UIState
{
public:
	StageSelect(class Game* game);
	struct DATA
	{
		VECTOR2 mBackPos;
		VECTOR2 mTitlePos;
		VECTOR2 mButtonPos;
		VECTOR2 mStage1GuideOffsetPos;
		VECTOR2 mStage2GuideOffsetPos;
		VECTOR2 mStage3GuideOffsetPos;
		const char*mTitle;
		COLOR mTitleColor;
		float mTextSize;
	};
private:
	DATA Data;
};
