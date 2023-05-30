#include "UIPlayerHome.h"
#include "PSideCharacterActor.h"
#include "Game.h"
#include "PlayerHome.h"

UIPlayerHome::UIPlayerHome(PlayerHome* owner)
	:UIScreen(owner->GetGame())
	, mOwner(owner)
	, mGoButton(nullptr)
	, mReturnButton(nullptr)
{
	float Width = (float)(mHeight * mOwner->GetMaxHp());
	mPos = VECTOR2(width / 2.0f, height - mOwner->GetGame()->GetAllData()->hpGaugeUIData.mHeight * 2.0f);
	AddRectButton("iŒR"
		, [this]()
		{

		}
		,nullptr
			,VECTOR2(50.0f,100.0f)
	);


	mGoButton = mButtons[0];
}

void UIPlayerHome::draw()
{
	rectMode(CORNER);
	strokeWeight(mOwner->GetGame()->GetAllData()->hpGaugeUIData.mSw);
	stroke(mOwner->GetGame()->GetAllData()->hpGaugeUIData.mWhite);

	COLOR color;
	if (mOwner->GetHp() >= mOwner->GetMaxHp() * mGame->GetAllData()->hpGaugeUIData.mNormalHpRate)
	{
		color = mGame->GetAllData()->hpGaugeUIData.mNormalColor;
	}
	else if (mOwner->GetHp() >= mOwner->GetMaxHp() * mGame->GetAllData()->hpGaugeUIData.mDangerHpRate)
	{
		color = mGame->GetAllData()->hpGaugeUIData.mDangarColor;
	}
	else
	{
		color = mGame->GetAllData()->hpGaugeUIData.mDyingColor;
	}
	rectMode(CORNER);
	fill(mGame->GetAllData()->hpGaugeUIData.mHpWindowColor);
	strokeWeight(mGame->GetAllData()->hpGaugeUIData.mHpGaugeSw);
	rect(mPos.x - mOwner->GetGame()->GetAllData()->hpGaugeUIData.mSw, mPos.y - mOwner->GetGame()->GetAllData()->hpGaugeUIData.mSw, mOwner->GetGame()->GetAllData()->hpGaugeUIData.mHeight * mOwner->GetMaxHp() - mOwner->GetGame()->GetAllData()->hpGaugeUIData.mSw * 2, mOwner->GetGame()->GetAllData()->hpGaugeUIData.mHeight + 10.0f);
	if (mOwner->GetHp() > mOwner->GetMaxHp())
	{
		rect(mPos.x + mOwner->GetGame()->GetAllData()->hpGaugeUIData.mHeight * mOwner->GetMaxHp() + mOwner->GetGame()->GetAllData()->hpGaugeUIData.mSw * 2.0f, mPos.y - mOwner->GetGame()->GetAllData()->hpGaugeUIData.mSw, (mOwner->GetHp() - mOwner->GetMaxHp()) * mOwner->GetGame()->GetAllData()->hpGaugeUIData.mHeight - mOwner->GetGame()->GetAllData()->hpGaugeUIData.mSw, mOwner->GetGame()->GetAllData()->hpGaugeUIData.mHeight + 10.0f);
	}
	fill(color);
	stroke(color);

	rect(mPos.x - 1.0f, mPos.y - 1.0f, mHpGaugeWidth, mOwner->GetGame()->GetAllData()->hpGaugeUIData.mHeight + 2.0f);

	if (mOwner->GetHp() <= mOwner->GetMaxHp())
	{
		fill(mGame->GetAllData()->hpGaugeUIData.mWhite);
	}
	else
	{
		fill(mGame->GetAllData()->hpGaugeUIData.mOverMaxHpColor);
	}

	textSize(mOwner->GetGame()->GetAllData()->hpGaugeUIData.mHpTextSize);
	text((let)"Hp :" + mOwner->GetHp() + (let)" / " + mOwner->GetMaxHp(), width / 2.0f - mOwner->GetGame()->GetAllData()->hpGaugeUIData.mHpTextSize * 7.0f, mPos.y + mOwner->GetGame()->GetAllData()->hpGaugeUIData.mHpTextSize);
}

void UIPlayerHome::Update()
{
	float preWidth = mHpGaugeWidth;
	float wid = mOwner->GetHp() * (mOwner->GetGame()->GetAllData()->hpGaugeUIData.mHeight + 0.1f);
	mHpGaugeWidth = preWidth + (wid - preWidth) * 0.05f;
}
