#include "UIHelp1.h"
#include "UIHelp2.h"
#include "Game.h"

UIHelp1::UIHelp1(class Game* game)
	:UIHelpBase(game)
{
	Data = mGame->GetAllData()->help1Data;

	AddButton(
		"¨",
		[this]() {
			new UIHelp2(mGame);
	CloseMe();
		}
		, 2
			, "Ÿ‚Ìƒy[ƒW‚Ö"
			, Data.mNextButtonPos
			, Data.mNextButtonGuidePos
			);

	SetBackImg(Data.mImage);

}
