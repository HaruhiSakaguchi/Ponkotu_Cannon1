#include "UIPSideCharacterStatusClose.h"
#include "Game.h"
#include "graphic.h"
#include <sstream>
#include "UIPSideCharacterStatus.h"

UIPSideCharacterStatusClose::UIPSideCharacterStatusClose(class CharacterActor* owner)
	:UIScreen(owner->GetGame())
	,mOwner(owner)
	, mPos(0.0f, 0.0f)
	, mHpGaugeWidth(0.0f)
	,mOpenButtun(nullptr)
{
	mPos = VECTOR2(0.0f, (mGame->GetAllData()->itemCompoData.mUIOffsetPosY + 75.0f)) +
		mGame->GetAllData()->itemCompoData.mUIInitPos;

	AddButton("¤",
		[this]() {
			new UIPSideCharacterStatus(mOwner);
			CloseMe();
		}
		, 2
			, nullptr
			, VECTOR2(mGame->GetAllData()->itemCompoData.mUIMinPosX - width / 2.0f + 50.0f, 215.0f)

			);

	mOpenButtun = GetButtons()[0];
}

void UIPSideCharacterStatusClose::draw()
{
	rectMode(CORNER);
	if (mOwner->GetState() == Actor::EActive)
	{
		noStroke();
		fill(mGame->GetAllData()->itemStatusData.mTriangleColor);
		rect(mPos.x, mPos.y, mGame->GetAllData()->itemStatusData.mWidth, mGame->GetAllData()->itemStatusData.mHeight / 4);

		DrawHpGauge();

		DrawNameandLevel();
	}

}

void UIPSideCharacterStatusClose::Update()
{
	if (mOwner->GetState() == Actor::EDead)
	{
		CloseMe();
	}

	if (mPos.x > mGame->GetAllData()->itemCompoData.mUIMinPosX)
	{
		mPos.x += mGame->GetAllData()->itemCompoData.mUIPosAdvSpeed;
	}

	mPos.y = (mGame->GetAllData()->itemCompoData.mUIOffsetPosY / 2.0f) * static_cast<Cannon*>(mOwner)->GetNum() + mGame->GetAllData()->itemCompoData.mUIInitPos.y;

	if (mOwner->GetState() == Actor::EActive)
	{
		float preWidth = mHpGaugeWidth;
		float wid = 100.0f * mOwner->GetHp() / mOwner->GetMaxHp();
		mHpGaugeWidth = preWidth + (wid - preWidth) * 0.05f;;
	}

	mOpenButtun->SetPosition(VECTOR2(mPos.x + 150.0f + 50.0f,mPos.y + 25.0f));
}


void UIPSideCharacterStatusClose::DrawNameandLevel()
{
	textSize(15);

	std::ostringstream oss;
	oss << mOwner->GetName().c_str() << " Lv." << mOwner->GetLevel();
	fill(0, 0, 0);
	text(oss.str().c_str(), mPos.x + 15.0f, mPos.y + 15.0f);
}


void UIPSideCharacterStatusClose::DrawHpGauge()
{
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

	noStroke();
	rect(mPos.x + 25.0f, mPos.y + 20.0f, 100.0f, 10.0f);
	fill(color);
	rect(mPos.x + 25.0f, mPos.y + 20.0f, mHpGaugeWidth, 10.0f);
	fill(0, 0, 0);
	textSize(15.0f);
	text("Hp :" + (let)mOwner->GetHp() + "/" + (let)mOwner->GetMaxHp(), mPos.x + 4.5f * 15.0f / 2.0f, mPos.y + 35.0f - 2.5f);
}