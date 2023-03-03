#include "UIHelp3.h"
#include "UIHelp2.h"
#include "Game.h"

UIHelp3::UIHelp3(Game* game)
	:UIHelpBase(game)
{
	Data = mGame->GetAllData()->help3Data;
	AddButton(
		"←",
		[this]() {
			new UIHelp2(mGame);
	CloseMe();
		}
		, 2
			, "前のページへ"
			, Data.mBoforeButtonPos
			);

	SetBackImg(Data.mImage);

}