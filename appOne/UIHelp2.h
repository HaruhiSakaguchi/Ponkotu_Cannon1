#pragma once
#include "UIHelpBase.h"
class UIHelp2 :
	public UIHelpBase
{
public:
	UIHelp2(class Game* game);
	struct DATA
	{
		int mImage;
		VECTOR2 mNextButtonPos;
		VECTOR2 mNextButtonGuidePos;
		VECTOR2 mBeforeButtonPos;
	};
private:
	DATA Data;
};

