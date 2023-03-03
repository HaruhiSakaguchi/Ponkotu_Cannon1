#include "UIProgressGauge.h"
#include "Game.h"

UIProgressGauge::UIProgressGauge(class Map* owner, class Game* game) :
	UIScreen(game)
	, mOwner(owner)
{
	Data = mGame->GetAllData()->progressData;
	Data.mLimmit = owner->GetClearCnt();
}

void UIProgressGauge::Update()
{
	VECTOR2 prePos = Data.mPos;
	VECTOR2 pos = VECTOR2(width / 2.0f - Data.mSw * 5.0f + Data.mSw * 10.0f / Data.mLimmit * (Data.mLimmit - mOwner->GetClearCnt()), Data.mSw * 2.0f);
	Data.mPos = prePos + (pos - prePos) * Data.mChangeProgressSpeed;
}

void UIProgressGauge::Draw()
{
	rectMode(CENTER);
	noStroke();
	fill(Data.mBackColor);
	rect(width / 2.0f, Data.mSw * 2.0f, Data.mGaugeWidth, Data.mGaugeHeight);

	if (Data.mLimmit - mOwner->GetClearCnt() != 0)
	{
		rectMode(CORNER);
		fill(Data.mCompleteColor);
		rect(width / 2.0f - Data.mSw * 5.0f, Data.mPos.y - Data.mSw / 4.0f, Data.mPos.x - width / 2.0f - Data.mSw * 5.0f + Data.mSw * 10.0f, Data.mGaugeHeight);
	}

	fill(Data.mTextColor);
	textSize(Data.mTextSize);
	text((let)((int)((float)(Data.mLimmit - mOwner->GetClearCnt()) / Data.mLimmit * Data.mTextSize * 2.0f)) + (let)"%", width / 2.0f + Data.mTextSize * 5.0f + Data.mTextSize, Data.mTextSize * 2.0f + Data.mTextSize / 2.0f);

	fill(Data.mPointColor);
	strokeWeight(Data.mSw);
	stroke(Data.mPointColor);
	point(Data.mPos.x, Data.mPos.y);

	if (mOwner->GetClearCnt() != 0)
	{
		fill(Data.mTextColor);
		textSize(Data.mTextSize);
		text("‚ ‚Æ" + (let)mOwner->GetClearCnt() + "‘Ì", width / 2.0f - (Data.mTextSize * 1.5f), Data.mTextSize * 4.0f);
	}


}