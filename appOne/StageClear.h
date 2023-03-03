#pragma once
#include "UIState.h"

class StageClear : public UIState
{
public:
	StageClear(class Game* game);
	~StageClear();
	void ChangeOption()override;
	void Draw()override;
	struct DATA
	{
		int mSound;
		int mSound2;
		COLOR mTitleColor;
		float mTextSize;
		VECTOR2 mTitlePos;
		VECTOR2 mButtonPos;
		float mClearTimeTextSize;
	};
private:
	DATA Data;
	std::string mClearTimeText;
};
