#pragma once
#include "UIMainState.h"

class GameOver : public UIMainState
{
public:
	GameOver(class Game* game);
	void Update()override;
	void ChangeOption()override;
	struct DATA
	{
		const char*mTitle;
		int mSound;
		float mTitleTextSize;
		float mTitlePosAdvSpeed;
		float mMaxOffsetPosY;
		VECTOR2 mTitlePos;
		VECTOR2 mButtonPos;
		VECTOR2 mRetryButtonPosOffset;
		COLOR mTitleColor;
	};
private:
	DATA Data;
private:
	float mTime;
	bool mIsRetryFlag;
	bool mIsChangeTitleFlag;
};
