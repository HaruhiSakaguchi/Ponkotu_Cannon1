#include "UIProgressGauge.h"
#include "Game.h"

UIProgressGauge::UIProgressGauge(class Map* owner, class Game* game) :
	UIScreen(game)
	, mOwner(owner)
{
	Data = mGame->GetAllData()->progressData;
}

void UIProgressGauge::Update()
{
	VECTOR2 prePos = Data.mPos;
	VECTOR2 pos = VECTOR2(width / 2.0f - Data.mSw * 5.0f + Data.mSw * 10.0f / Data.mLimmit * (Data.mLimmit), Data.mSw * 2.0f);
	Data.mPos = prePos + (pos - prePos) * Data.mChangeProgressSpeed;
}

void UIProgressGauge::Draw()
{
	rectMode(CENTER);
	noStroke();
	fill(Data.mBackColor);
	rect(width / 2.0f, Data.mSw * 2.0f, Data.mGaugeWidth, Data.mGaugeHeight);

	fill(Data.mTextColor);
	textSize(Data.mTextSize);

	fill(Data.mPointColor);
	strokeWeight(Data.mSw);
	stroke(Data.mPointColor);
	point(Data.mPos.x, Data.mPos.y);


}