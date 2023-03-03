#pragma once
#include "UIState.h"

class Quit : public UIState
{
public:
	Quit(class Game* game);
	void ChangeOption()override;
	struct DATA
	{
		VECTOR2 mBackPos; 
		VECTOR2 mTitlePos; 
		VECTOR2 mButtonPos;
		const char*mTitle;
		COLOR mTitleColor;
		float mTextSize;
	};
private:
	DATA Data;
};
