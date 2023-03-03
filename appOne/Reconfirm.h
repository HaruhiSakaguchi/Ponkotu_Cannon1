#pragma once
#include "UIState.h"

class Reconfirm : public UIState
{
public:
	Reconfirm(class UIState* state, class Game* game, const char* name);
	~Reconfirm();
	void draw()override;
	struct DATA
	{
		const char *mTitle;
		float mReconfirmTextSize;
		float mReconfirmTextOffsetY;
		VECTOR2 mBackPos;
		VECTOR2 mTitlePos;
		VECTOR2 mButtonPos;
		COLOR mTitleColor;
		COLOR mReconfirmTextColor;
	};
private:
	DATA Data;
private:
	std::string mName;
	bool mFlag;
	class UIState* mOwner;
};