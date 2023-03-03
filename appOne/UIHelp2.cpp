#include "UIHelp2.h"
#include "UIHelp3.h"
#include "UIHelp1.h"
#include "Game.h"

UIHelp2::UIHelp2(class Game* game)
	:UIHelpBase(game)
{
	Data = mGame->GetAllData()->help2Data;

	AddButton(
		"��",
		[this]() {
			new UIHelp3(mGame);
	CloseMe();
		}
		, 2
			, "���̃y�[�W��"
			, Data.mNextButtonPos
			, Data.mNextButtonGuidePos
			);
	AddButton(
		"��",
		[this]() {
			new UIHelp1(mGame);
	CloseMe();
		}
		, 2
			, "�O�̃y�[�W��"
			, Data.mBeforeButtonPos
			);

	SetBackImg(Data.mImage);
}


