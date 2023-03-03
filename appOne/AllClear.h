#pragma once
#include "UIState.h"
#include "sound.h"

class AllClear : public UIState
{
public:
	AllClear(class Game* game);
	~AllClear();
	void ChangeOption()override;
	void draw()override;
	void Update()override;
	struct DATA
	{
		int mBgm;
		int mImg;
		VECTOR2 mButtonPos;
	};
private:
	DATA Data;
};
