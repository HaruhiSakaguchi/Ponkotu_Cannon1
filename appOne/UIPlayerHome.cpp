#include "UIPlayerHome.h"
#include "PSideCharacterActor.h"
#include "Game.h"
#include "PlayerHome.h"
#include "UIGenerate.h"
#include <sstream>
#include "UIPopUp.h"
#include "window.h"

UIPlayerHome::UIPlayerHome(PlayerHome* owner)
	:UIScreen(owner->GetGame())
	, mOwner(owner)
	, mGoButton(nullptr)
	, mReturnButton(nullptr)
	, mGenerate(nullptr)
	, mGenerateItemButton(nullptr)
	, mHpGaugeWidth(0.0f)
	, mBarrierHpGaugeWidth(0.0f)
	, mUnitMaxNum(1)
{
	float Width = (float)(mHeight * mOwner->GetMaxHp());
	mPos = VECTOR2(width / 2.0f, height - mOwner->GetGame()->GetAllData()->hpGaugeUIData.mHeight * 2.0f);

	PlayerHome* h = static_cast<PlayerHome*>(mOwner);

	mGoButton = AddRectButton("進軍"
		, [this]()
		{
			PlayerHome* h = static_cast<PlayerHome*>(mOwner);
			if (h->GetTargetPosIdx() < 2 && h->GetGenerateFlag() == 0)
			{
				h->SetTargetPoint(h->GetHomeMoveTargetPoints()[h->GetTargetPosIdx() + 1]);
				h->SetTargetPosIdx(h->GetTargetPosIdx() + 1);
			}
			else if (h->GetTargetPosIdx() == 2)
			{
				auto pop = new UIPopUp(mGame, "これ以上進めない！！", mGoButton->GetPosition(), 1, VECTOR2(0.0f, -1.0f));
				pop->SetTextSize(30);
				pop->SetTextColor(COLOR(50, 50, 255));
				pop->NoStrokeRect();
			}
			else if (h->GetGenerateFlag() == 1)
			{
				auto pop = new UIPopUp(mGame, "ユニットの追加中に移動することはできません", mGoButton->GetPosition(), 1, VECTOR2(0.0f, -1.0f));
				pop->SetTextSize(30);
				pop->SetTextColor(COLOR(50, 50, 255));
				pop->NoStrokeRect();
			}
		}
		, nullptr
			, VECTOR2(50.0f, 100.0f)
			);
	mReturnButton = AddRectButton("退軍"
		, [this]()
		{
			PlayerHome* h = static_cast<PlayerHome*>(mOwner);
			if (h->GetTargetPosIdx() > 0 && h->GetGenerateFlag() == 0)
			{
				h->SetTargetPoint(h->GetHomeMoveTargetPoints()[h->GetTargetPosIdx() - 1]);
				h->SetTargetPosIdx(h->GetTargetPosIdx() - 1);
			}
			else if (h->GetTargetPosIdx() == 0)
			{
				auto pop = new UIPopUp(mGame, "これ以上下がれない！！", mReturnButton->GetPosition(), 1, VECTOR2(0.0f, -1.0f));
				pop->SetTextSize(30);
				pop->SetTextColor(COLOR(50, 50, 255));
				pop->NoStrokeRect();
			}
			else if (h->GetGenerateFlag() == 1)
			{
				auto pop = new UIPopUp(mGame, "ユニットの追加中に移動することはできません", mReturnButton->GetPosition(), 1, VECTOR2(0.0f, -1.0f));
				pop->SetTextSize(30);
				pop->SetTextColor(COLOR(50, 50, 255));
				pop->NoStrokeRect();
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
				auto pop = new UIPopUp(mGame, "HomeのLvが上がった！！", mHomeLvUpButton->GetPosition(), 1, VECTOR2(0.0f, -1.0f));
				pop->SetTextSize(30);
				pop->SetTextColor(COLOR(255, 255, 128));
				pop->NoStrokeRect();
				if (mUnitMaxNum < mOwner->GetMaxLevel())
				{
					mUnitMaxNum++;
				}
				if (mOwner->GetLevel() == 5)
				{
					mGenerateItemButton->SetState(Button::Enable);
					auto pop = new UIPopUp(mGame, "アイテムが追加できるようになった！！", mGenerateItemButton->GetPosition(), 1, VECTOR2(0.0f, -1.0f));
					pop->SetTextSize(30);
					pop->SetTextColor(COLOR(255, 255, 128));
					pop->NoStrokeRect();
					mGame->GetActorManager()->GetStage()->AddText("アイテムの追加ができるようになりました。");
				}
			}
			else if (mOwner->GetLevel() == mOwner->GetMaxLevel())
			{
				auto pop = new UIPopUp(mGame, "HomeのLvはこれ以上上がらない！！", mHomeLvUpButton->GetPosition(), 1, VECTOR2(0.0f, -1.0f));
				pop->SetTextSize(30);
				pop->SetTextColor(COLOR(255, 255, 128));
				pop->NoStrokeRect();
			}
			else
			{
				auto pop = new UIPopUp(mGame, "ポイントが足りない", mHomeLvUpButton->GetPosition(), 1, VECTOR2(0.0f, -1.0f));
				pop->SetTextSize(30);
				pop->SetTextColor(COLOR(50, 50, 255));
				pop->NoStrokeRect();
			}
		}
		, 2
			);

	mGenerateCannonButton = AddRectButton("キャラクターを追加"
		, [this]()
		{
			PlayerHome* h = static_cast<PlayerHome*>(mOwner);
			if (h->GetMoveCompleteFlag() == 1 && ((int)(mGame->GetActorManager()->GetPSide().size()) - 1) <= mGame->GetActorManager()->GetPHome()->GetLevel() && h->GetGenerateFlag() == 0 && (int)(mGame->GetActorManager()->GetPSide().size()) - 1 != mOwner->GetMaxLevel())
			{
				h->SetGenerateFlag(1);
				if (!mGenerate)
				{
					mGenerate = new UIGenerate(this, mGame, UIGenerate::ECannon);
				}
				else
				{
					mGenerate->CloseMe();
					mGenerate = new UIGenerate(this, mGame, UIGenerate::ECannon);
				}
			}
			else if (h->GetMoveCompleteFlag() == 0)
			{
				auto pop = new UIPopUp(mGame, "移動中はユニットの追加ができません", mGenerateCannonButton->GetPosition(), 1, VECTOR2(0.0f, -1.0f));
				pop->SetTextSize(30);
				pop->SetTextColor(COLOR(50, 50, 255));
				pop->NoStrokeRect();
			}
			else if (h->GetGenerateFlag() == 1)
			{
				auto pop = new UIPopUp(mGame, "ユニットの追加中に新たにユニットを追加することはできません", mGenerateCannonButton->GetPosition(), 1, VECTOR2(0.0f, -1.0f));
				pop->SetTextSize(30);
				pop->SetTextColor(COLOR(50, 50, 255));
				pop->NoStrokeRect();
			}
			else
			{
				auto pop = new UIPopUp(mGame, "ユニットがいっぱいです", mGenerateCannonButton->GetPosition(), 1, VECTOR2(0.0f, -1.0f));
				pop->SetTextSize(30);
				pop->SetTextColor(COLOR(50, 50, 255));
				pop->NoStrokeRect();
			}
		}
		, nullptr
			, VECTOR2(100.0f, 100.0f)
			);

	mGenerateCannonButton->SetNameOffsetPos(VECTOR2(0.0f, -30.0f));

	mGenerateCannonLvUpButton = AddButton("Lv+"
		, [this]()
		{
			PlayerHome* h = static_cast<PlayerHome*>(mOwner);
			if (h->GetGenerateCannonLv() < mOwner->GetLevel() && h->GetBattlePoints() >= 150)
			{
				h->SetGenerateCannonLv(h->GetGenerateCannonLv() + 1);
				h->SetBattlePoints(h->GetBattlePoints() - 150);
				auto pop = new UIPopUp(mGame, "次に出すCannonのLvが上がります", mGenerateCannonLvUpButton->GetPosition(), 1, VECTOR2(0.0f, -1.0f));
				pop->SetTextSize(30);
				pop->SetTextColor(COLOR(255, 255, 128));
				pop->NoStrokeRect();
			}
			else if (h->GetGenerateCannonLv() == mGame->GetActorManager()->GetPHome()->GetMaxLevel())
			{
				auto pop = new UIPopUp(mGame, "Lvはこれ以上上がらない！！", mGenerateCannonLvUpButton->GetPosition(), 1, VECTOR2(0.0f, -1.0f));
				pop->SetTextSize(30);
				pop->SetTextColor(COLOR(255, 255, 128));
				pop->NoStrokeRect();
			}
			else if (h->GetGenerateCannonLv() >= mOwner->GetLevel())
			{
				auto pop = new UIPopUp(mGame, "HomeのLvを超えることはできない", mGenerateCannonLvUpButton->GetPosition(), 1, VECTOR2(0.0f, -1.0f));
				pop->SetTextSize(30);
				pop->SetTextColor(COLOR(50, 50, 255));
				pop->NoStrokeRect();
			}
			else if (h->GetBattlePoints() < 150)
			{
				auto pop = new UIPopUp(mGame, "ポイントが足りない", mGenerateCannonLvUpButton->GetPosition(), 1, VECTOR2(0.0f, -1.0f));
				pop->SetTextSize(30);
				pop->SetTextColor(COLOR(50, 50, 255));
				pop->NoStrokeRect();
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
				auto pop = new UIPopUp(mGame, "次に出すBarricadeのLvが上がります", mGenerateBarricadeLvUpButton->GetPosition(), 1, VECTOR2(0.0f, -1.0f));
				pop->SetTextSize(30);
				pop->SetTextColor(COLOR(255, 255, 128));
				pop->NoStrokeRect();
			}
			else if (h->GetGenerateBarricadeLv() == mGame->GetActorManager()->GetPHome()->GetMaxLevel())
			{
				auto pop = new UIPopUp(mGame, "Lvはこれ以上上がらない！！", mGenerateBarricadeLvUpButton->GetPosition(), 1, VECTOR2(0.0f, -1.0f));
				pop->SetTextSize(30);
				pop->SetTextColor(COLOR(255, 255, 128));
				pop->NoStrokeRect();
			}
			else if (h->GetGenerateBarricadeLv() >= mOwner->GetLevel())
			{
				auto pop = new UIPopUp(mGame, "HomeのLvを超えることはできない", mGenerateBarricadeLvUpButton->GetPosition(), 1, VECTOR2(0.0f, -1.0f));
				pop->SetTextSize(30);
				pop->SetTextColor(COLOR(50, 50, 255));
				pop->NoStrokeRect();
			}
			else if (h->GetBattlePoints() < 100)
			{
				auto pop = new UIPopUp(mGame, "ポイントが足りない", mGenerateBarricadeLvUpButton->GetPosition(), 1, VECTOR2(0.0f, -1.0f));
				pop->SetTextSize(30);
				pop->SetTextColor(COLOR(50, 50, 255));
				pop->NoStrokeRect();
			}
		}
		, 2
			);

	mGenerateItemButton = AddRectButton("アイテムを追加",
		[this]()
		{
			PlayerHome* h = static_cast<PlayerHome*>(mOwner);
			if (h->GetMoveCompleteFlag() == 1 && h->GetGenerateFlag() == 0)
			{
				h->SetGenerateFlag(1);
				if (!mGenerate)
				{
					mGenerate = new UIGenerate(this, mGame, UIGenerate::EBarrier);
				}
				else
				{
					mGenerate->CloseMe();
					mGenerate = new UIGenerate(this, mGame, UIGenerate::EBarrier);
				}
			}
			else if (h->GetMoveCompleteFlag() == 0)
			{
				auto pop = new UIPopUp(mGame, "移動中はユニットの追加ができません", mGenerateItemButton->GetPosition(), 1, VECTOR2(0.0f, -1.0f));
				pop->SetTextSize(30);
				pop->SetTextColor(COLOR(50, 50, 255));
				pop->NoStrokeRect();
			}
			else if (h->GetGenerateFlag() == 1)
			{
				auto pop = new UIPopUp(mGame, "ユニットの追加中に新たにユニットを追加することはできません", mGenerateItemButton->GetPosition(), 1, VECTOR2(0.0f, -1.0f));
				pop->SetTextSize(30);
				pop->SetTextColor(COLOR(50, 50, 255));
				pop->NoStrokeRect();
			}
		}
	);

	mGenerateItemButton->SetState(Button::Disable);
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

	rect(mPos.x - 1.0f, mPos.y - 1.0f, mHpGaugeWidth - 25.0f, mOwner->GetGame()->GetAllData()->hpGaugeUIData.mHeight + 2.0f);

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

	if (mOwner->GetBarrier())
	{
		noStroke();
		fill(50, 50, 255, 128);
		rect(mPos.x + 1000.0f - mBarrierHpGaugeWidth - 25.0f + 12.5f, mPos.y + 25.0f, mBarrierHpGaugeWidth, 25.0f);
	}

	textSize(30);
	fill(0, 0, 0);
	text((let)"ユニット数　" + (let)((int)mGame->GetActorManager()->GetPSide().size() - 1) + (let)" / "  + (let)mUnitMaxNum, 1600.0f, 50.0f);
}

void UIPlayerHome::Update()
{
	float preWidth = mHpGaugeWidth;
	float wid = ((float)mOwner->GetHp() / (float)mOwner->GetMaxHp()) * (mOwner->GetInitMaxHp() * mOwner->GetGame()->GetAllData()->hpGaugeUIData.mHeight);
	mHpGaugeWidth = preWidth + (wid - preWidth) * 0.05f;

	if (mOwner->GetBarrier())
	{
		float preBWidth = mBarrierHpGaugeWidth;
		float Bwid = 500.0f * ((float)mOwner->GetBarrier()->GetHp() / (float)mOwner->GetBarrier()->GetMaxHp());
		mBarrierHpGaugeWidth = preBWidth + (Bwid - preBWidth) * 0.05f;
	}

	mGoButton->SetPosition(VECTOR2(mPos.x, mPos.y - 100.0f));
	mReturnButton->SetPosition(VECTOR2(mPos.x + 100.0f, mPos.y - 100.0f));
	mHomeLvUpButton->SetPosition(VECTOR2(mPos.x + 100.0f + 100.0f, mPos.y - 100.0f));
	mGenerateCannonButton->SetPosition(VECTOR2(mPos.x + 500.0f, mPos.y - 100.0f));
	mGenerateCannonLvUpButton->SetPosition(VECTOR2(mPos.x + 300.0f + 400.0f, mPos.y - 100.0f + 25.0f));
	//mGenerateBarricadeButton->SetPosition(VECTOR2(mPos.x + 300.0f + 200.0f, mPos.y - 100.0f));
	mGenerateBarricadeLvUpButton->SetPosition(VECTOR2(mPos.x + 300.0f + 200.0f + 250.0f, mPos.y - 100.0f + 25.0f));
	mGenerateItemButton->SetPosition(VECTOR2(mPos.x + 300.0f + 200.0f + 100.0f + 250.0f + 50.0f, mPos.y - 100.0f + 25.0f));
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
		//text(bLevel.str().c_str(), mGenerateBarricadeButton->GetPosition().x - 15.0f * 3.0f, mGenerateBarricadeButton->GetPosition().y + 30.0f);
		text((let)"ポイント　" + (let)static_cast<PlayerHome*>(mOwner)->GetBattlePoints() + " / " + (let)static_cast<PlayerHome*>(mOwner)->GetMaxBattlePoints(), 0.0f, 50.0f);
	}
}
