#include "UIPSideCharacterStatusBase.h"
#include "Game.h"
UIPSideCharacterStatusBase::UIPSideCharacterStatusBase(PSideCharacterActor* owner)
	:UIScreen(owner->GetGame())
	, mOwner(owner)
	, mPosition(0.0f, 0.0f)
	, mOffset(0.0f, 0.0f)
	, mLvUpButton(nullptr)
	, mHpGaugeWidth(0.0f)
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
