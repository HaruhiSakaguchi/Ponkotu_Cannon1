#include "UIGenerate.h"
#include "PlayerHome.h"
#include "Game.h"
#include "input.h"
#include "Cannon.h"
#include "UIPlayerHome.h"
#include "Barricade.h"
#include "TreeMeshComponent.h"
#include "CannonWheelL.h"
#include "CannonWheelR.h"
#include "UIPopUp.h"


UIGenerate::UIGenerate(class UIPlayerHome* owner, Game* game, GenerateActor_Id id)
	: UIScreen(game)
	, mGenePos(0.0f, 0.0f, -27.0f)
	, mMouseXPerWidth(0.0f)
	, mMouseYPerHeight(0.0f)
	, mOwner(owner)
	, mId(id)
	, mGenerateUsingPoints(0)
	, mGenerateActor(nullptr)
	, mChangeButton(nullptr)
	, mCancellButton(nullptr)
{
	mCancellButton = AddRectButton("�L�����Z��",
		[this]()
		{
			mGame->GetActorManager()->GetPHome()->SetGenerateFlag(false);
			CloseMe();
			mOwner->SetGenerate(nullptr);
			for (auto button : mOwner->GetButtons())
			{
				button->SetState(Button::Enable);
			}
			for (auto uiStatus : mGame->GetUIManager()->GetUIPSideStatus())
			{
				for (auto button : uiStatus->GetButtons())
				{
					button->SetState(Button::Enable);
				}
			}
		}
	);

	mCancellButton->SetPosition(mCancellButton->GetPosition() + VECTOR2(300.0f, 0.0f));

	const char* text = nullptr;

	if (mId == GenerateActor_Id::ECannon)
	{
		text = "Barricade�ɐ؂�ւ�";
	}
	else if (mId == GenerateActor_Id::EBarricade)
	{
		text = "Cannon�ɐ؂�ւ�";
	}

	mChangeButton =AddRectButton(text,
		[this]()
		{
			if (mId == GenerateActor_Id::ECannon)
			{
				auto ui = new UIGenerate(mOwner, mGame,UIGenerate::GenerateActor_Id::EBarricade);
				CloseMe();
				mOwner->SetGenerate(ui);
			}
			else if (mId == GenerateActor_Id::EBarricade)
			{
				auto ui = new UIGenerate(mOwner, mGame, UIGenerate::GenerateActor_Id::ECannon);
				CloseMe();
				mOwner->SetGenerate(ui);

			}
		}
	);

	mChangeButton->SetPosition(mChangeButton->GetPosition() + VECTOR2(300.0f, 100.0f));

	for (auto button : mOwner->GetButtons())
	{
		button->SetState(Button::Disable);
	}

	for (auto uiStatus : mGame->GetUIManager()->GetUIPSideStatus())
	{
		for (auto button : uiStatus->GetButtons())
		{
			button->SetState(Button::Draw_Enable);
		}
	}

	mGenerateActor = new CharacterActor(mGame);
	auto tree = new TreeMeshComponent(mGenerateActor);
	if (mId == GenerateActor_Id::ECannon)
	{
		tree->SetTree("CannonBarrel");
		new CannonWheelL(static_cast<class Cannon*>(mGenerateActor));
		new CannonWheelR(static_cast<class Cannon*>(mGenerateActor));
		mGenerateActor->SetRotationY(3.1415926f);
	}
	else if (mId == GenerateActor_Id::EBarricade)
	{
		tree->SetTree("Barricade");
	}
	else
	{
		mGenerateActor->SetState(CharacterActor::EDead);
	}

	mOwner->SetGenerate(this);
}

UIGenerate::~UIGenerate()
{
	mGenerateActor->SetState(CharacterActor::EDead);
}

void UIGenerate::draw()
{
	textSize(30);
	fill(0, 0, 0);
	text((let)mGame->GetActorManager()->GetPHome()->GetBattlePoints() + " / " + (let)mGame->GetActorManager()->GetPHome()->GetMaxBattlePoints(), width / 2, height / 2 + 150.0f);
	if (mId == GenerateActor_Id::ECannon)
	{
		text("Cannon���ǂ��ɏo�������܂����H " + (let)mGenerateUsingPoints + "�|�C���g����", width / 2, height / 2 + 200.0f);
	}
	else if (mId == GenerateActor_Id::EBarricade)
	{
		text("Barricade���ǂ��ɐݒu���܂����H " + (let)mGenerateUsingPoints + "�|�C���g����", width / 2, height / 2 + 200.0f);
	}
}

void UIGenerate::Update()
{

	mMouseXPerWidth = (width / 2.0f - mouseX) / -340.0f / 2;
	mMouseYPerHeight = (height / 2.0f - mouseY) / -height / 2;

	mGenePos.x = mMouseXPerWidth * 4.0f * mGame->GetActorManager()->GetStage()->GetStageMaxX();
	mGenePos.z = mMouseYPerHeight * 4.0f * -(mGame->GetActorManager()->GetStage()->GetStageMinZ() + -mGame->GetActorManager()->GetStage()->GetStageMaxZ()) / 2.0f + mGame->GetActorManager()->GetStage()->GetCenterPos().z;

	VECTOR2 mousePos = VECTOR2(mouseX, mouseY);

	if (!mGame->GetActorManager()->GetPHome())
	{
		CloseMe();
	}
	else
	{
		if (mId != GenerateActor_Id::EEmpty)
		{
			if (mId == GenerateActor_Id::ECannon)
			{
				mGenerateUsingPoints = mGame->GetActorManager()->GetPHome()->GetGenerateCannonLv() * 100 + 300;
			}
			else if (mId == GenerateActor_Id::EBarricade)
			{
				mGenerateUsingPoints = mGame->GetActorManager()->GetPHome()->GetGenerateBarricadeLv() * 50 + 100;
			}
		}

		if (mGenerateActor)
		{
			mGenerateActor->SetPosition(mGenePos);
		}

		if (isTrigger(MOUSE_LBUTTON) && ((int)(mGame->GetActorManager()->GetPSide().size()) - 1) <= mGame->GetActorManager()->GetPHome()->GetLevel() && !mCancellButton->ContainsPoint(mousePos) && !mChangeButton->ContainsPoint(mousePos))
		{
			if (mGenerateUsingPoints <= mGame->GetActorManager()->GetPHome()->GetBattlePoints())
			{
				if (mId != GenerateActor_Id::EEmpty)
				{
					CharacterActor* c = nullptr;
					if (mId == GenerateActor_Id::ECannon)
					{
						c = new class Cannon(mGame);
						c->SetUp();
						c->SetPosition(mGame->GetActorManager()->GetPHome()->GetPosition());
						c->GetGame()->GetActorManager()->GetPHome()->GetDore()->Open();
						c->SetLevel(c->GetGame()->GetActorManager()->GetPHome()->GetGenerateCannonLv());
					}
					else if (mId == GenerateActor_Id::EBarricade)
					{
						c = new class Barricade(mGame);
						c->SetPosition(mGenePos + VECTOR(0.0f, 10.0f, 0.0f));
						mGame->GetActorManager()->GetPHome()->SetGenerateFlag(false);
						c->SetLevel(c->GetGame()->GetActorManager()->GetPHome()->GetGenerateBarricadeLv());
					}

					c->SetInitPosition(mGenePos);
					c->SetMaxHp((int)(c->GetInitMaxHp() * ((c->GetLevel() + c->GetMaxLevel()) / 10.0f)));
					c->SetHp(c->GetMaxHp());
					mGame->GetActorManager()->GetPHome()->SetBattlePoints(mGame->GetActorManager()->GetPHome()->GetBattlePoints() - mGenerateUsingPoints);

				}

				for (auto button : mOwner->GetButtons())
				{
					button->SetState(Button::Enable);
				}
				for (auto uiStatus : mGame->GetUIManager()->GetUIPSideStatus())
				{
					for (auto button : uiStatus->GetButtons())
					{
						button->SetState(Button::Enable);
					}
				}
				CloseMe();
				mOwner->SetGenerate(nullptr);
				auto pop = new UIPopUp(mGame, "Generate�I�I", mousePos, 1, VECTOR2(0.0f, -1.0f));
				pop->SetTextSize(30);
				pop->SetTextColor(COLOR(255, 255, 128));
				pop->NoStrokeRect();
			}
			else
			{
				auto pop = new UIPopUp(mGame, "�|�C���g������Ȃ�", mousePos, 1, VECTOR2(0.0f, -1.0f));
				pop->SetTextSize(30);
				pop->SetTextColor(COLOR(50, 50, 255));
				pop->NoStrokeRect();
			}
		}
	}

}