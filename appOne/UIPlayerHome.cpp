#include "UIPlayerHome.h"
#include "PSideCharacterActor.h"
#include "Game.h"
#include "PlayerHome.h"
#include "UIGenerate.h"
#include <sstream>

UIPlayerHome::UIPlayerHome(PlayerHome* owner)
	:UIScreen(owner->GetGame())
	, mOwner(owner)
	, mGoButton(nullptr)
	, mReturnButton(nullptr)
	, mGenerate(nullptr)
	, mHpGaugeWidth(0.0f)
{
	float Width = (float)(mHeight * mOwner->GetMaxHp());
	mPos = VECTOR2(width / 2.0f, height - mOwner->GetGame()->GetAllData()->hpGaugeUIData.mHeight * 2.0f);

	PlayerHome* h = static_cast<PlayerHome*>(mOwner);

	mGoButton = AddRectButton("êiåR"
		, [this]()
		{
			PlayerHome* h = static_cast<PlayerHome*>(mOwner);
			if (h->GetTargetPosIdx() < 2 && !h->GetGenerateFlag())
			{
				h->SetTargetPoint(h->GetHomeMoveTargetPoints()[h->GetTargetPosIdx() + 1]);
				h->SetTargetPosIdx(h->GetTargetPosIdx() + 1);
			}
		}
		, nullptr
			, VECTOR2(50.0f, 100.0f)
			);
	mReturnButton = AddRectButton("ëﬁåR"
		, [this]()
		{
			PlayerHome* h = static_cast<PlayerHome*>(mOwner);
			if (h->GetTargetPosIdx() > 0 && !h->GetGenerateFlag())
			{
				h->SetTargetPoint(h->GetHomeMoveTargetPoints()[h->GetTargetPosIdx() - 1]);
				h->SetTargetPosIdx(h->GetTargetPosIdx() - 1);
			}
		}
		, nullptr
			, VECTOR2(50.0f, 100.0f)
			);

	mHomeLvUpButton = AddButton("Lv+"
		, [this]()
		{
			if (mOwner->GetLevel() < mOwner->GetMaxLevel() && static_cast<PlayerHome*>(mOwner)->GetBattlePoints() >= 300)
			{
				int curMaxHp = mOwner->GetMaxHp();
				mOwner->SetLevel(mOwner->GetLevel() + 1);
				mOwner->SetMaxHp((int)(mOwner->GetInitMaxHp() * ((mOwner->GetLevel() + mOwner->GetMaxLevel()) / 10.0f)));
				mOwner->SetHp((int)(round(mOwner->GetMaxHp() * (float)mOwner->GetHp() / (float)curMaxHp)));
				static_cast<PlayerHome*>(mOwner)->SetBattlePoints(static_cast<PlayerHome*>(mOwner)->GetBattlePoints() - 300);
			}
		}
		, 2
			);

	mGenerateCannonButton = AddRectButton("Cannon"
		, [this]()
		{
			PlayerHome* h = static_cast<PlayerHome*>(mOwner);
			if (h->GetMoveCompleteFlag() && ((int)(mGame->GetActorManager()->GetPSide().size()) - 1) <= mGame->GetActorManager()->GetPHome()->GetLevel() && !h->GetGenerateFlag() && (int)(mGame->GetActorManager()->GetPSide().size()) - 1 != mOwner->GetMaxLevel())
			{
				h->SetGenerateFlag(true);
				if (!mGenerate)
				{
					mGenerate = new UIGenerate(this, mGame, UIGenerate::Cannon);
				}
				else
				{
					mGenerate->CloseMe();
					mGenerate = new UIGenerate(this, mGame, UIGenerate::Cannon);
				}
			}
		}
		, nullptr
			, VECTOR2(100.0f, 100.0f)
			);

	mGenerateCannonButton->SetNameOffsetPos(VECTOR2(0.0f, -30.0f));


	mGenerateBarricadeButton = AddRectButton("Barricade"
		, [this]()
		{
			PlayerHome* h = static_cast<PlayerHome*>(mOwner);
			if (h->GetMoveCompleteFlag() && ((int)(mGame->GetActorManager()->GetPSide().size()) - 1) <= mGame->GetActorManager()->GetPHome()->GetLevel() && !h->GetGenerateFlag() && (int)(mGame->GetActorManager()->GetPSide().size()) - 1 != mOwner->GetMaxLevel())
			{
				h->SetGenerateFlag(true);
				//h->Open();
				if (!mGenerate)
				{
					mGenerate = new UIGenerate(this, mGame, UIGenerate::Barricade);
				}
				else
				{
					mGenerate->CloseMe();
					mGenerate = new UIGenerate(this, mGame, UIGenerate::Barricade);
				}
			}
		}
		, nullptr
			, VECTOR2(100.0f, 100.0f)
			);
	mGenerateBarricadeButton->SetNameOffsetPos(VECTOR2(0.0f, -30.0f));

	mGenerateCannonLvUpButton = AddButton("Lv+"
		, [this]()
		{
			PlayerHome* h = static_cast<PlayerHome*>(mOwner);
			if (h->GetGenerateCannonLv() < mOwner->GetLevel() && h->GetBattlePoints() >= 150)
			{
				h->SetGenerateCannonLv(h->GetGenerateCannonLv() + 1);
				h->SetBattlePoints(h->GetBattlePoints() - 150);
			}
		}
		, 2
			);

	mGenerateBarricadeLvUpButton = AddButton("Lv+"
		, [this]()
		{
			PlayerHome* h = static_cast<PlayerHome*>(mOwner);
			if (h->GetGenerateBarricadeLv() < mOwner->GetLevel() && h->GetBattlePoints() >= 100)
			{
				h->SetGenerateBarricadeLv(h->GetGenerateBarricadeLv() + 1);
				h->SetBattlePoints(h->GetBattlePoints() - 100);
			}
		}
		, 2
			);
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
	rect(mPos.x - mOwner->GetGame()->GetAllData()->hpGaugeUIData.mSw, mPos.y - mOwner->GetGame()->GetAllData()->hpGaugeUIData.mSw, mOwner->GetGame()->GetAllData()->hpGaugeUIData.mHeight * mOwner->GetInitMaxHp() - mOwner->GetGame()->GetAllData()->hpGaugeUIData.mSw * 2, mOwner->GetGame()->GetAllData()->hpGaugeUIData.mHeight + 10.0f);
	if (mOwner->GetHp() > mOwner->GetMaxHp())
	{
		rect(mPos.x + mOwner->GetGame()->GetAllData()->hpGaugeUIData.mHeight * (mOwner->GetInitMaxHp()) - mOwner->GetGame()->GetAllData()->hpGaugeUIData.mSw, mPos.y - mOwner->GetGame()->GetAllData()->hpGaugeUIData.mSw, (mOwner->GetHp() - mOwner->GetMaxHp()) * mOwner->GetGame()->GetAllData()->hpGaugeUIData.mHeight - mOwner->GetGame()->GetAllData()->hpGaugeUIData.mSw * 2.0f, mOwner->GetGame()->GetAllData()->hpGaugeUIData.mHeight + 10.0f);
	}
	fill(color);
	stroke(color);

	rect(mPos.x - 1.0f, mPos.y - 1.0f, mHpGaugeWidth, mOwner->GetGame()->GetAllData()->hpGaugeUIData.mHeight + 2.0f);

	if (mOwner->GetHp() <= mOwner->GetMaxHp())
	{
		fill(0, 0, 0);
	}
	else
	{
		fill(mGame->GetAllData()->hpGaugeUIData.mOverMaxHpColor);
	}

	textSize(mOwner->GetGame()->GetAllData()->hpGaugeUIData.mHpTextSize);
	text((let)"Lv :" + (let)mOwner->GetLevel() + " Hp :" + mOwner->GetHp() + (let)" / " + mOwner->GetMaxHp(), width / 2.0f - mOwner->GetGame()->GetAllData()->hpGaugeUIData.mHpTextSize * 7.0f, mPos.y + mOwner->GetGame()->GetAllData()->hpGaugeUIData.mHpTextSize);



}

void UIPlayerHome::Update()
{
	float preWidth = mHpGaugeWidth;
	float wid = ((float)mOwner->GetHp() / (float)mOwner->GetMaxHp()) * (mOwner->GetInitMaxHp() * mOwner->GetGame()->GetAllData()->hpGaugeUIData.mHeight + 0.1f);
	mHpGaugeWidth = preWidth + (wid - preWidth) * 0.05f;
	mGoButton->SetPosition(VECTOR2(mPos.x, mPos.y - 100.0f));
	mReturnButton->SetPosition(VECTOR2(mPos.x + 100.0f, mPos.y - 100.0f));
	mHomeLvUpButton->SetPosition(VECTOR2(mPos.x + 100.0f + 100.0f, mPos.y - 100.0f));
	mGenerateCannonButton->SetPosition(VECTOR2(mPos.x + 300.0f, mPos.y - 100.0f));
	mGenerateCannonLvUpButton->SetPosition(VECTOR2(mPos.x + 300.0f + 100.0f, mPos.y - 100.0f + 25.0f));
	mGenerateBarricadeButton->SetPosition(VECTOR2(mPos.x + 300.0f + 200.0f, mPos.y - 100.0f));
	mGenerateBarricadeLvUpButton->SetPosition(VECTOR2(mPos.x + 300.0f + 200.0f + 100.0f, mPos.y - 100.0f + 25.0f));
	
}

void UIPlayerHome::DrawAfterButton()
{
	if (!mGenerate)
	{
		std::ostringstream cLevel;
		std::ostringstream bLevel;
		textSize(30);
		cLevel << "Lv : " << static_cast<PlayerHome*>(mOwner)->GetGenerateCannonLv();
		bLevel << "Lv : " << static_cast<PlayerHome*>(mOwner)->GetGenerateBarricadeLv();

		text(cLevel.str().c_str(), mGenerateCannonButton->GetPosition().x - 15.0f * 3.0f, mGenerateCannonButton->GetPosition().y + 30.0f);
		text(bLevel.str().c_str(), mGenerateBarricadeButton->GetPosition().x - 15.0f * 3.0f, mGenerateBarricadeButton->GetPosition().y + 30.0f);
		text((let)static_cast<PlayerHome*>(mOwner)->GetBattlePoints() + " / " + (let)static_cast<PlayerHome*>(mOwner)->GetMaxBattlePoints(), 0.0f, 50.0f);
	}
}
