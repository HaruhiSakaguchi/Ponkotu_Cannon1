#include "UIPSideCharacterStatusClose.h"
#include "Game.h"
#include "graphic.h"
#include <sstream>
#include "UIPSideCharacterStatus.h"
#include "PlayerHome.h"

UIPSideCharacterStatusClose::UIPSideCharacterStatusClose(class CharacterActor* owner)
	: UIPSideCharacterStatusBase(static_cast<PSideCharacterActor*>(owner))
	, mHpGaugeWidth(0.0f)
	, mOpenButtun(nullptr)
{
	mPosition = VECTOR2(0.0f, (mGame->GetAllData()->itemCompoData.mUIOffsetPosY + 75.0f)) +
		mGame->GetAllData()->itemCompoData.mUIInitPos;

	if (mOwner->GetTag() == CharacterActor::Cannon)
	{
		mOpenButtun = AddButton("¤",
			[this]() {
				for (auto ui : mGame->GetUIManager()->GetUIPSideStatus())
				{
					if (ui->GetOwner()->GetNum() > mOwner->GetNum())
					{
						ui->SetOffset(ui->GetOffset() + VECTOR2(0.0f, mGame->GetAllData()->itemCompoData.mUIOffsetPosY + 75.0f));
					}
				}
				CloseMe();
				UIPSideCharacterStatus* ui = new UIPSideCharacterStatus(mOwner);
				ui->SetOffset(this->GetOffset());
			}
			, 2
				, nullptr
				, VECTOR2(mGame->GetAllData()->itemCompoData.mUIMinPosX - width / 2.0f + 50.0f, 215.0f)

				);

	}
	mLvUpButton = AddButton("+LV",
		[this]() {
			if (mOwner->GetLevel() < mGame->GetActorManager()->GetPHome()->GetLevel() && (100 + mOwner->GetLevel() * 50) <= mGame->GetActorManager()->GetPHome()->GetBattlePoints())
			{
				int curMaxHp = mOwner->GetMaxHp();
				mOwner->GetGame()->GetActorManager()->GetPHome()->SetBattlePoints(mOwner->GetGame()->GetActorManager()->GetPHome()->GetBattlePoints() - (100 + mOwner->GetLevel() * 50));
				mOwner->SetLevel(mOwner->GetLevel() + 1);
				mOwner->SetMaxHp((int)(mOwner->GetInitMaxHp() * ((mOwner->GetLevel() + mOwner->GetMaxLevel()) / 10.0f)));
				mOwner->SetHp((int)(round(mOwner->GetMaxHp() * (float)mOwner->GetHp() / (float)curMaxHp)));
			}
		}
		, 2
			);
}

void UIPSideCharacterStatusClose::draw()
{
	rectMode(CORNER);
	if (mOwner->GetState() == Actor::EActive)
	{
		noStroke();
		fill(mGame->GetAllData()->itemStatusData.mTriangleColor);
		rect(mPosition.x + mOffset.x, mPosition.y + mOffset.y, mGame->GetAllData()->itemStatusData.mWidth, mGame->GetAllData()->itemStatusData.mHeight / 4);

		DrawHpGauge();

		DrawNameandLevel();
	}

}

void UIPSideCharacterStatusClose::Update()
{
	if (!mOwner)
	{
		CloseMe();
	}
	else
	{
		if (mPosition.x > mGame->GetAllData()->itemCompoData.mUIMinPosX)
		{
			mPosition.x += mGame->GetAllData()->itemCompoData.mUIPosAdvSpeed;
		}

		int num = static_cast<PSideCharacterActor*>(mOwner)->GetNum();

		if (mGame->GetActorManager()->GetPHome() && num > mGame->GetActorManager()->GetPHome()->GetNum())
		{
			num--;
		}

		mPosition.y = (mGame->GetAllData()->itemCompoData.mUIOffsetPosY / 2.0f) * num + mGame->GetAllData()->itemCompoData.mUIInitPos.y;

		if (mOwner->GetState() == Actor::EActive)
		{
			float preWidth = mHpGaugeWidth;
			float wid = 100.0f * mOwner->GetHp() / mOwner->GetMaxHp();
			mHpGaugeWidth = preWidth + (wid - preWidth) * 0.05f;;

			if (mOwner->GetTag() == CharacterActor::Cannon)
			{
				mOpenButtun->SetPosition(VECTOR2(mPosition.x + 150.0f + 50.0f - 25.0f, mPosition.y + 25.0f) + mOffset);
			}

			mLvUpButton->SetPosition(VECTOR2(mPosition.x + 150.0f + 50.0f + 50.0f - 25.0f, mPosition.y + 25.0f) + mOffset);
		}
	}

	if (!mGame->GetActorManager()->GetPHome())
	{
		for (auto button : GetButtons())
		{
			button->SetState(Button::Draw_Enable);
		}
	}

}


void UIPSideCharacterStatusClose::DrawNameandLevel()
{
	textSize(15);

	std::ostringstream oss;
	oss << mOwner->GetName().c_str() << " Lv." << mOwner->GetLevel();
	fill(0, 0, 0);
	text(oss.str().c_str(), mPosition.x + mOffset.x + 15.0f, mPosition.y + mOffset.y + 15.0f);
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
	rect(mPosition.x + mOffset.x + 25.0f, mPosition.y + mOffset.y + 20.0f, 100.0f, 10.0f);
	fill(color);
	rect(mPosition.x + mOffset.x + 25.0f, mPosition.y + mOffset.y + 20.0f, mHpGaugeWidth, 10.0f);
	fill(0, 0, 0);
	textSize(15.0f);
	text("Hp :" + (let)mOwner->GetHp() + "/" + (let)mOwner->GetMaxHp(), mPosition.x + 4.5f * 15.0f / 2.0f + mOffset.x, mPosition.y + mOffset.y + 35.0f - 2.5f);
}