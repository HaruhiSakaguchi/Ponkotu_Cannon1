#include "Reconfirm.h"
#include "Game.h"
#include "TransitionFade.h"

Reconfirm::Reconfirm(class UIState* state, class Game* game, const char* name)
	:UIState(game)
	, mName(name)
	, mOwner(state)
	, mFlag(false)
{
	Data = mGame->GetAllData()->reconfirmData;

	mTitle = Data.mTitle;
	mBackPos = Data.mBackPos;
	mTitlePos = Data.mTitlePos;
	mButtonPos = Data.mButtonPos;
	mTitleColor = Data.mTitleColor;
	mBackImg = mGame->GetAllData()->mDialog2;
	
	AddButton("‚Í‚¢",
		[this]() {
			mFlag = true;
			CloseMe();
			mGame->SetContinueFlag(false);
			mOwner->CloseMe();
		}
	);

	AddButton("‚¢‚¢‚¦",
		[this]() {
			CloseMe();
		}
	);
}

Reconfirm::~Reconfirm()
{
	if (mFlag)
	{
		mGame->GetTransition()->outTrigger();
	}
}

void Reconfirm::draw()
{
	textSize(Data.mReconfirmTextSize);
	fill(Data.mReconfirmTextColor);
	text(mName.c_str(), mTitlePos.x - (mName.length() / 4.0f) * Data.mReconfirmTextSize, mTitlePos.y + Data.mReconfirmTextOffsetY);
}
