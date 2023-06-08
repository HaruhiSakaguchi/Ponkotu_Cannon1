#include "UIGenerate.h"
#include "PlayerHome.h"
#include "Game.h"
#include "input.h"
#include "Cannon.h"
#include "UIPlayerHome.h"
#include "Barricade.h"

UIGenerate::UIGenerate(class UIPlayerHome* owner, Game* game, GenerateActor_Id id)
	: UIScreen(game)
	, mGenePos(0.0f, 0.0f, -27.0f)
	, mMouseXPerWidth(0.0f)
	, mMouseYPerHeight(0.0f)
	, mOwner(owner)
	, mId(id)
{
	AddRectButton("キャンセル",
		[this]()
		{
			mGame->GetPHome()->SetGenerateFlag(false);
			CloseMe();
			mOwner->SetGenerate(nullptr);
			for (auto button : mOwner->GetButtons())
			{
				button->SetState(Button::Enable);
			}
			for (auto uiStatus : mGame->GetUIPSideStatus())
			{
				for (auto button : uiStatus->GetButtons())
				{
					button->SetState(Button::Enable);
				}
			}
		}
	);

	for (auto button : mOwner->GetButtons())
	{
		button->SetState(Button::Disable);
	}

	for (auto uiStatus : mGame->GetUIPSideStatus())
	{
		for (auto button : uiStatus->GetButtons())
		{
			button->SetState(Button::Draw_Enable);
		}
	}

	mOwner->SetGenerate(this);
}

void UIGenerate::draw()
{
	textSize(30);
	text("mouse(" + (let)mouseX + "," + (let)mouseY + ")", width / 2, height / 2);
	text("mouse%(" + (let)mMouseXPerWidth * 400.0f + "," + (let)mMouseYPerHeight * 400.0f + ")", width / 2, height / 2 + 50.0f);
	text("GenePos(" + (let)mGenePos.x + "," + (let)mGenePos.z + ")", width / 2, height / 2 + 100.0f);

	if (mId == GenerateActor_Id::Cannon)
	{
		text("Cannonをどこに出撃させますか？", width / 2, height / 2 + 150.0f);
	}
	else if (mId == GenerateActor_Id::Barricade)
	{
		text("Barricadeをどこに設置しますか？", width / 2, height / 2 + 150.0f);
	}
}

void UIGenerate::Update()
{

	mMouseXPerWidth = (width / 2.0f - mouseX) / -340.0f / 2;
	mMouseYPerHeight = (height / 2.0f - mouseY) / -height / 2;

	mGenePos.x = mMouseXPerWidth * 4.0f * mGame->GetStage()->GetStageMaxX();
	mGenePos.z = mMouseYPerHeight * 4.0f * -(mGame->GetStage()->GetStageMinZ() + -mGame->GetStage()->GetStageMaxZ()) / 2.0f + mGame->GetStage()->GetCenterPos().z;

	if (isTrigger(MOUSE_RBUTTON) && ((int)(mGame->GetPSide().size()) - 1) < mGame->GetPHome()->GetLevel())
	{
		if (mId != GenerateActor_Id::Empty)
		{
			CharacterActor* c = nullptr;
			if (mId == GenerateActor_Id::Cannon)
			{
				c = new class Cannon(mGame);
				c->SetUp();
				c->SetPosition(mGame->GetPHome()->GetPosition());
				c->GetGame()->GetPHome()->Open();
				c->SetLevel(c->GetGame()->GetPHome()->GetGenerateCannonLv());
			}
			else if (mId == GenerateActor_Id::Barricade)
			{
				c = new class Barricade(mGame);
				c->SetPosition(mGenePos + VECTOR(0.0f, 10.0f, 0.0f));
				mGame->GetPHome()->SetGenerateFlag(false);
				c->SetLevel(c->GetGame()->GetPHome()->GetGenerateBarricadeLv());
			}

			c->SetInitPosition(mGenePos);
			c->SetMaxHp((int)(c->GetInitMaxHp() * ((c->GetLevel() + c->GetMaxLevel()) / 10.0f)));
			c->SetHp(c->GetMaxHp());

		}

		for (auto button : mOwner->GetButtons())
		{
			button->SetState(Button::Enable);
		}
		for (auto uiStatus : mGame->GetUIPSideStatus())
		{
			for (auto button : uiStatus->GetButtons())
			{
				button->SetState(Button::Enable);
			}
		}
		CloseMe();
		mOwner->SetGenerate(nullptr);
	}

}
