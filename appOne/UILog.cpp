#include "UILog.h"
#include "graphic.h"
#include <sstream>
#include "Game.h"

UILog::UILog(class Game* game)
	:UIScreen(game)
{
	Data = mGame->GetAllData()->logData;
}

UILog::~UILog()
{
	while (!mTexts.empty())
	{
		mTexts.pop_back();
	}
}

void UILog::Draw()
{
	rectMode(CORNER);
	fill(Data.mLogWindowColor);
	noStroke();
	rect(Data.mPos.x, Data.mPos.y, Data.mWidth, Data.mHeight);

	
	for (int i = 0;i < (int)mTexts.size();i++)
	{
		VECTOR2 pos = VECTOR2(Data.mPos.x, Data.mPos.y + Data.mTextSize + Data.mTextSize * i);
		textSize(Data.mTextSize);
		fill(Data.mLogTextColor);
		text(mTexts[i].c_str(), pos.x, pos.y);
	}
}

void UILog::AddText(std::string string)
{
	std::ostringstream oss;
	oss << mGame->GetActorManager()->GetStage()->GetTimeText().c_str() << " " << string.c_str();
	mTexts.emplace_back(oss.str());
	if (mTexts.size() > (int)(Data.mHeight / Data.mTextSize))
	{
		RemoveText(0);
	}
}

void UILog::RemoveText(int num)
{
	//num”Ô–Ú‚Ìƒiƒ“ƒo[‚ğæ‚èœ‚­
	auto iter = std::find(mTexts.begin(), mTexts.end(), mTexts[num]);
	if (iter != mTexts.end())
	{
		mTexts.erase(iter);
	}
}