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
	mGame->GetUIManager()->AddUIPSide(this);
	mOwner->SetUI(this);
}

UIPSideCharacterStatusBase::~UIPSideCharacterStatusBase()
{
	mGame->GetUIManager()->RemoveUIPSide(this);

}
