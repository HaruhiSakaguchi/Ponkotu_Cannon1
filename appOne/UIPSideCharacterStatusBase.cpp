#include "UIPSideCharacterStatusBase.h"
#include "Game.h"
#include "PlayerHome.h"
#include "UIPopUp.h"

UIPSideCharacterStatusBase::UIPSideCharacterStatusBase(PSideCharacterActor* owner)
	: UIScreen(owner->GetGame())
	, mOwner(owner)
	, mPosition(0.0f, 0.0f)
	, mOffset(0.0f, 0.0f)
	, mLvUpButton(nullptr)
	, mHpGaugeWidth(0.0f)
	, mBarrierHpGaugeWidth(0.0f)
	, mNextPSideStatusOffset(0.0f)
{
	Data = mGame->GetAllData()->psData;

	mGame->GetUIManager()->AddUIPSide(this);
	mOwner->SetUI(this);
}

UIPSideCharacterStatusBase::~UIPSideCharacterStatusBase()
{
	mGame->GetUIManager()->RemoveUIPSide(this);
}

void UIPSideCharacterStatusBase::OtherPSideUIsTranslate(const VECTOR2& offset)
{
	for (auto ui : mGame->GetUIManager()->GetUIPSideStatus())
	{
		//自分よりナンバーの大きい（下にある）PsideStatusUIの位置を自分が縮んだり消えたりした分だけ詰める
		if (ui->GetOwner()->GetNum() > mOwner->GetNum())
		{
			ui->SetOffset(ui->GetOffset() + offset);
		}
	}
}

void UIPSideCharacterStatusBase::Update()
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

		int num = mOwner->GetNum();

		if (mGame->GetActorManager()->GetPHome() && num > mGame->GetActorManager()->GetPHome()->GetNum())
		{
			num--;
		}

		mPosition.y = (mGame->GetAllData()->itemCompoData.mUIOffsetPosY / 2.0f) * num + mGame->GetAllData()->itemCompoData.mUIInitPos.y;

		float preWidth = mHpGaugeWidth;
		float wid = 100.0f * mOwner->GetHp() / mOwner->GetMaxHp();
		mHpGaugeWidth = preWidth + (wid - preWidth) * 0.05f;

		if (mOwner->GetTag() == CharacterActor::Cannon)
		{
			auto c = static_cast<class Cannon*>(mOwner);
			if (c->GetBarrier())
			{
				float preBWidth = mBarrierHpGaugeWidth;
				float Bwid = 50.0f * c->GetBarrier()->GetHp() / c->GetBarrier()->GetMaxHp();
				mBarrierHpGaugeWidth = preBWidth + (Bwid - preBWidth) * 0.05f;
			}
		}

		update();
	}

	if (!mGame->GetActorManager()->GetPHome())
	{
		for (auto button : GetButtons())
		{
			button->SetState(Button::Draw_Enable);
		}
	}

}

void UIPSideCharacterStatusBase::DrawHpGauge(const VECTOR2& pos)
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
	rect(pos.x + 25.0f, pos.y + 20.0f, 100.0f, 10.0f);
	fill(color);
	rect(pos.x + 25.0f, pos.y + 20.0f, mHpGaugeWidth, 10.0f);
	fill(0, 0, 0);
	textSize(15.0f);
	text("Hp :" + (let)mOwner->GetHp() + "/" + (let)mOwner->GetMaxHp(), pos.x + 4.5f * 15.0f / 2.0f, pos.y + 20.0f + 15.0f - 2.5f);

	if (mOwner->GetTag() == CharacterActor::Cannon && static_cast<class Cannon*>(mOwner)->GetBarrier())
	{
		DrawBarrierHpGauge(pos);
	}
}

void UIPSideCharacterStatusBase::DrawBarrierHpGauge(const VECTOR2& pos)
{
	noStroke();
	fill(COLOR(0, 0, 255));
	rect(pos.x + 25.0f - mBarrierHpGaugeWidth + 100.0f, pos.y + 50.0f / 2.0f, mBarrierHpGaugeWidth, 5.0f);
}

void UIPSideCharacterStatusBase::OwnerLvUp()
{
	if (mOwner->GetLevel() < mGame->GetActorManager()->GetPHome()->GetLevel() && (100 + mOwner->GetLevel() * 50) <= mGame->GetActorManager()->GetPHome()->GetBattlePoints())
	{
		int curMaxHp = mOwner->GetMaxHp();
		mOwner->GetGame()->GetActorManager()->GetPHome()->SetBattlePoints(mOwner->GetGame()->GetActorManager()->GetPHome()->GetBattlePoints() - (100 + mOwner->GetLevel() * 50));
		mOwner->SetLevel(mOwner->GetLevel() + 1);
		mOwner->SetMaxHp((int)(mOwner->GetInitMaxHp() * ((mOwner->GetLevel() + mOwner->GetMaxLevel()) / 10.0f)));
		mOwner->SetHp((int)(round(mOwner->GetMaxHp() * (float)mOwner->GetHp() / (float)curMaxHp)));
		std::ostringstream oss;
		oss << mOwner->GetName() << "のLvが上がった！！";

		auto pop = new UIPopUp(mGame, oss.str().c_str(), mLvUpButton->GetPosition(), 1, VECTOR2(0.0f, -1.0f));
		pop->SetTextSize(30);
		pop->SetTextColor(COLOR(255, 255, 128));
		pop->NoStrokeRect();
	}
	else if (mOwner->GetLevel() == mGame->GetActorManager()->GetPHome()->GetMaxLevel())
	{
		auto pop = new UIPopUp(mGame, "Lvはこれ以上上がらない！！", mLvUpButton->GetPosition(), 1, VECTOR2(0.0f, -1.0f));
		pop->SetTextSize(30);
		pop->SetTextColor(COLOR(255, 255, 128));
		pop->NoStrokeRect();
	}
	else if (mOwner->GetLevel() >= mGame->GetActorManager()->GetPHome()->GetLevel())
	{
		auto pop = new UIPopUp(mGame, "HomeのLvを超えることはできない", mLvUpButton->GetPosition(), 1, VECTOR2(0.0f, -1.0f));
		pop->SetTextSize(30);
		pop->SetTextColor(COLOR(50, 50, 255));
		pop->NoStrokeRect();
	}
	else if((100 + mOwner->GetLevel() * 50) > mGame->GetActorManager()->GetPHome()->GetBattlePoints())
	{
		auto pop = new UIPopUp(mGame, "ポイントが足りない", mLvUpButton->GetPosition(), 1, VECTOR2(0.0f, -1.0f));
		pop->SetTextSize(30);
		pop->SetTextColor(COLOR(50, 50, 255));
		pop->NoStrokeRect();
	}

}

void UIPSideCharacterStatusBase::UIOpenNexrOffsetPlus()
{
	if (!mGame->GetUIManager()->GetUIPSideStatus().empty())
	{
		int num = -1;
		UIPSideCharacterStatusBase* uiPsideBack = nullptr;
		VECTOR2 offset;
		for (auto ui : mGame->GetUIManager()->GetUIPSideStatus())
		{
			if (ui->GetOwner()->GetNum() > num)
			{
				num = ui->GetOwner()->GetNum();
				uiPsideBack = ui;
			}
			offset += uiPsideBack->GetNextOffset();
		}

		mOffset += uiPsideBack->GetOffset() + offset;
	}
}

