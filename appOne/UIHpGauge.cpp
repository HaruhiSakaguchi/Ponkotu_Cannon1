#include "UIHpGauge.h"
#include "Actor.h"
#include "graphic.h"
#include "Game.h"
#include "Items.h"

UIHpGauge::UIHpGauge(Game* game, CharacterActor* owner)
	: UIScreen(game)
	, mOwner(owner)
{
	Data = mGame->GetAllData()->hpGaugeUIData;
	Data.mMaxHp = mOwner->GetHp();
	Data.mWidth = (float)(Data.mHeight * Data.mMaxHp);
	Data.mPos = VECTOR2(width / 2.0f - Data.mWidth / 2.0f, height - Data.mHeight * 2.0f);
}

void UIHpGauge::Draw()
{
	rectMode(CORNER);
	strokeWeight(Data.mSw);
	stroke(Data.mWhite);
	fill(Data.mHpWindowColor);
	rect(Data.mPos.x + Data.mHpTextSize * 2.0f, Data.mPos.y - (Data.mHeight + 10.0f), width / 2 - Data.mHpTextSize * 3.0f, (Data.mHeight + 10.0f));
	textSize(Data.mHpTextSize);
	if (mOwner->GetHp() <= Data.mMaxHp)
	{
		fill(Data.mWhite);
	}
	else
	{
		fill(Data.mOverMaxHpColor);
	}
	text((let)"Hp" + mOwner->GetHp() + (let)" / " + Data.mMaxHp, width / 2.0f - Data.mHpTextSize * 7.0f, Data.mPos.y - 10.0f);


	COLOR color;
	if (mOwner->GetHp() >= Data.mMaxHp * Data.mNormalHpRate)
	{
		color = Data.mNormalColor;
	}
	else if (mOwner->GetHp() >= Data.mMaxHp * Data.mDangerHpRate)
	{
		color = Data.mDangarColor;
	}
	else
	{
		color = Data.mDyingColor;
	}
	rectMode(CORNER);
	fill(Data.mHpWindowColor);
	strokeWeight(Data.mHpGaugeSw);
	rect(Data.mPos.x - Data.mSw, Data.mPos.y - Data.mSw, Data.mHeight * Data.mMaxHp + Data.mSw * 2, Data.mHeight + 10.0f);
	if (mOwner->GetHp() > Data.mMaxHp)
	{
		rect(Data.mPos.x + Data.mHeight * Data.mMaxHp + Data.mSw * 2.0f, Data.mPos.y - Data.mSw, (mOwner->GetHp() - Data.mMaxHp) * Data.mHeight - Data.mSw, Data.mHeight + 10.0f);
	}
	fill(color);
	stroke(color);

	rect(Data.mPos.x - 1.0f, Data.mPos.y - 1.0f, Data.mGaugeWidth, Data.mHeight + 2.0f);
}

void UIHpGauge::Update()
{
	float preWidth = Data.mGaugeWidth;
	float wid = mOwner->GetHp() * (Data.mHeight + 0.1f);
	Data.mGaugeWidth = preWidth + (wid - preWidth) * Data.mChangeHpSpeed;
}
