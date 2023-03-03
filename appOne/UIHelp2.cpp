#include "UIHelp2.h"
#include "UIHelp3.h"
#include "UIHelp1.h"
#include "Game.h"

UIHelp2::UIHelp2(class Game* game)
	:UIHelpBase(game)
{
	Data = mGame->GetAllData()->help2Data;

	AddButton(
		"→",
		[this]() {
			new UIHelp3(mGame);
	CloseMe();
		}
		, 2
			, "次のページへ"
			, Data.mNextButtonPos
			, Data.mNextButtonGuidePos
			);
	AddButton(
		"←",
		[this]() {
			new UIHelp1(mGame);
	CloseMe();
		}
		, 2
			, "前のページへ"
			, Data.mBeforeButtonPos
			);

	SetBackImg(Data.mImage);
}


