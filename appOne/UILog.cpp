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


	for (int i = 0; i < (int)mTexts.size(); i++)
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
	if (oss.str().length() / 2.0f <= Data.mWidth / Data.mTextSize)
	{
		mTexts.emplace_back(oss.str());
	}
	else
	{
		int center = (int)(oss.str().length() / 2.0f);
		if (center % 2 == 1)
		{
			center++;
		}
		auto text0 = oss.str().substr(0, center);
		auto text1 = oss.str().substr(center, (int)oss.str().length());
		mTexts.emplace_back(text0);
		oss.str("");
		oss.clear();
		oss << "      " << text1.c_str();
		mTexts.emplace_back(oss.str());
	}

	int i = 0;
	while(mTexts.size() > (int)(Data.mHeight / Data.mTextSize))
	{
		RemoveText(i++);
	}
}

void UILog::RemoveText(int num)
{
	//num�Ԗڂ̃i���o�[����菜��
	auto iter = std::find(mTexts.begin(), mTexts.end(), mTexts[num]);
	if (iter != mTexts.end())
	{
		mTexts.erase(iter);
	}
}