#pragma once
#include "UIHelpBase.h"

class UIHelp3 :
	public UIHelpBase
{
public:
	UIHelp3(class Game* game);
	struct DATA
	{
		int mImage;
		VECTOR2 mBoforeButtonPos;
	};
private:
	DATA Data;
};

