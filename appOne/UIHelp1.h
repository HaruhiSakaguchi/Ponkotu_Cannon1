#pragma once
#include "UIHelpBase.h"

class UIHelp1 :
	public UIHelpBase
{
public:
	UIHelp1(class Game* game);
	struct DATA
	{
		int mImage;
		VECTOR2 mNextButtonPos;
		VECTOR2 mNextButtonGuidePos;
	};
private:
	DATA Data;
};

