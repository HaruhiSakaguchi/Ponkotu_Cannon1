#include "StageSelect.h"
#include "window.h"
#include "Reconfirm.h"
#include "Game.h"

StageSelect::StageSelect(class Game* game)
	:UIState(game)
{
	Data = mGame->GetAllData()->stageSelectData;
	//各パーツ位置調整

	mTitle = Data.mTitle;
	mTitleColor = Data.mTitleColor;
	mTextSize = Data.mTextSize;
	mBackPos = Data.mBackPos;
	mTitlePos = Data.mTitlePos;
	mButtonPos = Data.mButtonPos;

	mBackImg = mGame->GetAllData()->mDialog2;

	mGame->SetScene(Game::ETitle);

	AddButton("ステージ1",
		[this]() {
			mGame->SetPhase(Game::FIRST);
	        new Reconfirm(this, mGame, "『ステージ１』から始めますか？");
		}
		, 1
			, "『たま』が登場するフラットなステージです"
			, VECTOR2(0, 0)
			, Data.mStage1GuideOffsetPos
			);

	AddButton("ステージ2",
		[this]() {
			mGame->SetPhase(Game::SECOND);
			new Reconfirm(this, mGame, "『ステージ２』から始めますか？");
		}
		, 1
			, "『サテライト』が登場するフラットなステージです"
			, VECTOR2(0, 0)
			, Data.mStage2GuideOffsetPos
			);

	AddButton("ステージ3",
		[this]() {
			mGame->SetPhase(Game::THIRD);
			new Reconfirm(this, mGame, "『ステージ３』から始めますか？");
		}
		, 1
			, "すべての敵が登場する、高低差のあるステージです"
			, VECTOR2(0, 0)
			, Data.mStage3GuideOffsetPos
			);

	AddButton("戻る",
		[this]() {
			CloseMe();
		}
	);
}


