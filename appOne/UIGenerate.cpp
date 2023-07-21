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
#include "Items.h"
#include "EnemyHome.h"
#include "Map.h"
#include <sstream>

int UIGenerate::mGenerateActorLevel = 0;

UIGenerate::UIGenerate(class UIPlayerHome* owner, Game* game, GenerateActor_Id id)
	: UIScreen(game)
	, mGenePos(0.0f, 0.0f, -27.0f)
	, mMouseXPerWidth(0.0f)
	, mMouseYPerHeight(0.0f)
	, mOwner(owner)
	, mId(id)
	, mGenerateUsingPoints(0)
	, mItemGeneratePoint(150)
	, mGenerateActor(nullptr)
	, mChangeButton(nullptr)
	, mCancellButton(nullptr)
	, mGenerateActorLvPlusButton(nullptr)
	, mGenerateActorLvMinusButton(nullptr)
	, mLvMaxButton(nullptr)
	, mLvResetButton(nullptr)
	, mLvTUButtonPosOffset(80.0f, 0.0f)
{
	mCancellButton = AddRectButton("キャンセル",
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
		text = "Barricadeに切り替え";
	}
	else if (mId == GenerateActor_Id::EBarricade)
	{
		text = "Cannonに切り替え";
	}

	if ((int)mId < 2)
	{
		mChangeButton = AddRectButton(text,
			[this]()
			{
				if (mId == GenerateActor_Id::ECannon)
				{
					auto ui = new UIGenerate(mOwner, mGame, UIGenerate::GenerateActor_Id::EBarricade);
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
	}

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
	mMesh = new TreeMeshComponent(mGenerateActor);

	auto tree = static_cast<class TreeMeshComponent*>(mMesh);

	if (mId == GenerateActor_Id::ECannon)
	{
		tree->SetTree("CannonBarrel");
		new CannonWheelL(static_cast<class Cannon*>(mGenerateActor));
		new CannonWheelR(static_cast<class Cannon*>(mGenerateActor));
		mGenerateActor->SetRotationY(3.1415926f);
		CreateChangeItemGenerateButtons();

		CreateGenerateActorLvUpandDownButtons();

	}
	else if (mId == GenerateActor_Id::EBarricade)
	{
		tree->SetTree("Barricade");
		CreateChangeItemGenerateButtons();
		CreateGenerateActorLvUpandDownButtons();
	}
	else if (mId == GenerateActor_Id::EBarrier)
	{
		CreateItemButtons();

		tree->SetTree("Barrier");
		auto ChangeItemButton = AddRectButton("キャラクターに切り替え",
			[this]()
			{
				auto ui = new UIGenerate(mOwner, mGame, UIGenerate::GenerateActor_Id::ECannon);
				CloseMe();
				mOwner->SetGenerate(ui);
			});
		ChangeItemButton->SetPosition(mCancellButton->GetPosition() + VECTOR2(50.0f, 169.0f));
	}
	else if (mId == GenerateActor_Id::EPower)
	{
		CreateItemButtons();

		tree->SetTree("Power");
		auto ChangeItemButton = AddRectButton("キャラクターに切り替え",
			[this]()
			{
				auto ui = new UIGenerate(mOwner, mGame, UIGenerate::GenerateActor_Id::ECannon);
				CloseMe();
				mOwner->SetGenerate(ui);
			});
		ChangeItemButton->SetPosition(mCancellButton->GetPosition() + VECTOR2(50.0f, 169.0f));

	}
	else if (mId == GenerateActor_Id::ESpeed)
	{
		CreateItemButtons();

		tree->SetTree("Speed");
		auto ChangeItemButton = AddRectButton("キャラクターに切り替え",
			[this]()
			{
				auto ui = new UIGenerate(mOwner, mGame, UIGenerate::GenerateActor_Id::ECannon);
				CloseMe();
				mOwner->SetGenerate(ui);
			});
		ChangeItemButton->SetPosition(mCancellButton->GetPosition() + VECTOR2(50.0f, 169.0f));

	}
	else if (mId == GenerateActor_Id::ERapid)
	{
		CreateItemButtons();

		tree->SetTree("Rapid");
		auto ChangeItemButton = AddRectButton("キャラクターに切り替え",
			[this]()
			{
				auto ui = new UIGenerate(mOwner, mGame, UIGenerate::GenerateActor_Id::ECannon);
				CloseMe();
				mOwner->SetGenerate(ui);
			});
		ChangeItemButton->SetPosition(mCancellButton->GetPosition() + VECTOR2(50.0f, 169.0f));

	}
	else if (mId == GenerateActor_Id::ERecover)
	{
		CreateItemButtons();
		tree->SetTree("Recover");
		auto ChangeItemButton = AddRectButton("キャラクターに切り替え",
			[this]()
			{
				auto ui = new UIGenerate(mOwner, mGame, UIGenerate::GenerateActor_Id::ECannon);
				CloseMe();
				mOwner->SetGenerate(ui);
			});
		ChangeItemButton->SetPosition(mCancellButton->GetPosition() + VECTOR2(50.0f, 169.0f));
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
		text("Cannonをどこに出撃させますか？ " + (let)mGenerateUsingPoints + "ポイント消費", width / 2, height / 2 + 200.0f);
	}
	else if (mId == GenerateActor_Id::EBarricade)
	{
		text("Barricadeをどこに設置しますか？ " + (let)mGenerateUsingPoints + "ポイント消費", width / 2, height / 2 + 200.0f);
	}
	else if (mId == GenerateActor_Id::EBarrier)
	{
		text("BarrierItemをどこに設置しますか？ " + (let)mGenerateUsingPoints + "ポイント消費", width / 2, height / 2 + 200.0f);
	}
	else if (mId == GenerateActor_Id::EPower)
	{
		text("PowerItemをどこに設置しますか？ " + (let)mGenerateUsingPoints + "ポイント消費", width / 2, height / 2 + 200.0f);
	}
	else if (mId == GenerateActor_Id::ESpeed)
	{
		text("SpeedItemをどこに設置しますか？ " + (let)mGenerateUsingPoints + "ポイント消費", width / 2, height / 2 + 200.0f);
	}
	else if (mId == GenerateActor_Id::ERapid)
	{
		text("RapidItemをどこに設置しますか？ " + (let)mGenerateUsingPoints + "ポイント消費", width / 2, height / 2 + 200.0f);
	}
	else if (mId == GenerateActor_Id::ERecover)
	{
		text("RecoverItemをどこに設置しますか？ " + (let)mGenerateUsingPoints + "ポイント消費", width / 2, height / 2 + 200.0f);
	}

	if (mGenerateActorLvPlusButton && mGenerateActorLvMinusButton)
	{
		text(mGenerateActorLevel, mGenerateActorLvPlusButton->GetPosition().x + (mGenerateActorLvMinusButton->GetPosition().x - mGenerateActorLvPlusButton->GetPosition().x) / 2.0f - 7.5f, mGenerateActorLvPlusButton->GetPosition().y + 15.0f);
	}
}

void UIGenerate::Update()
{
	mMouseXPerWidth = (width / 2.0f - mouseX) / -340.0f / 2;
	mMouseYPerHeight = (height / 2.0f - mouseY) / -height / 2;

	mGenePos.x = mMouseXPerWidth * 4.0f * mGame->GetActorManager()->GetStage()->GetStageMaxX();
	mGenePos.z = mMouseYPerHeight * 4.0f * -((mGame->GetActorManager()->GetStage()->GetStageMinZ() + 3.0f) + -(mGame->GetActorManager()->GetStage()->GetStageMaxZ() - 3.0f)) / 2.0f + mGame->GetActorManager()->GetStage()->GetCenterPos().z;

	VECTOR2 mousePos = VECTOR2(mouseX, mouseY);

	bool inMapArea = true;

	if (width / 2.0f - 170.0f <= mousePos.x && mousePos.x <= width / 2.0f + 170.0f)
	{
		mMesh->SetDrawFlag(true);
		mGenerateActor->SetDamageInterval(0.0f);
	}
	else
	{
		mMesh->SetDrawFlag(false);
		mGenerateActor->SetDamageInterval(1.0f);
		inMapArea = false;
	}

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
				mGenerateUsingPoints = mGenerateActorLevel * 100 + 300;
			}
			else if (mId == GenerateActor_Id::EBarricade)
			{
				mGenerateUsingPoints = mGenerateActorLevel * 50 + 100;
			}
			else if (mId != GenerateActor_Id::EEmpty)
			{
				mGenerateUsingPoints = mItemGeneratePoint;
			}
		}

		if (mGenerateActor)
		{
			mGenerateActor->SetPosition(mGenePos);
		}

		bool contains = false;

		for (auto button : GetButtons())
		{
			if (button->ContainsPoint(mousePos))
			{
				contains = true;
				break;
			}
		}

		if (isTrigger(MOUSE_LBUTTON) && !contains)
		{
			if (inMapArea)
			{

				if (!mGame->GetActorManager()->GetEHome() || (mGame->GetActorManager()->GetEHome() && !mGame->GetActorManager()->GetEHome()->InEnemyArea(mGenePos)))
				{
					if (mGenerateUsingPoints <= mGame->GetActorManager()->GetPHome()->GetBattlePoints())
					{
						if (mId != GenerateActor_Id::EEmpty)
						{
							CharacterActor* c = nullptr;
							if (mId == GenerateActor_Id::ECannon && ((int)(mGame->GetActorManager()->GetPSide().size()) - 1) <= mGame->GetActorManager()->GetPHome()->GetLevel())
							{
								c = new class Cannon(mGame);
								c->SetUp();
								c->SetPosition(mGame->GetActorManager()->GetPHome()->GetPosition());
								c->GetGame()->GetActorManager()->GetPHome()->GetDore()->Open();
								c->SetLevel(mGenerateActorLevel);
							}
							else if (mId == GenerateActor_Id::EBarricade && ((int)(mGame->GetActorManager()->GetPSide().size()) - 1) <= mGame->GetActorManager()->GetPHome()->GetLevel())
							{
								c = new class Barricade(mGame);
								c->SetPosition(mGenePos + VECTOR(0.0f, 10.0f, 0.0f));
								mGame->GetActorManager()->GetPHome()->SetGenerateFlag(false);
								c->SetLevel(mGenerateActorLevel);
							}
							else if (mId == GenerateActor_Id::EBarrier)
							{
								c = new class Barrier(mGame);
								c->SetPosition(mGenePos + VECTOR(0.0f, 10.0f, 0.0f));
								mGame->GetActorManager()->GetPHome()->SetGenerateFlag(false);
							}
							else if (mId == GenerateActor_Id::EPower)
							{
								c = new class PowerUp(mGame);
								c->SetPosition(mGenePos + VECTOR(0.0f, 10.0f, 0.0f));
								mGame->GetActorManager()->GetPHome()->SetGenerateFlag(false);
							}
							else if (mId == GenerateActor_Id::ESpeed)
							{
								c = new class SpeedUp(mGame);
								c->SetPosition(mGenePos + VECTOR(0.0f, 10.0f, 0.0f));
								mGame->GetActorManager()->GetPHome()->SetGenerateFlag(false);
							}
							else if (mId == GenerateActor_Id::ERapid)
							{
								c = new class RapidFire(mGame);
								c->SetPosition(mGenePos + VECTOR(0.0f, 10.0f, 0.0f));
								mGame->GetActorManager()->GetPHome()->SetGenerateFlag(false);
							}
							else if (mId == GenerateActor_Id::ERecover)
							{
								c = new class Recovery(mGame);
								c->SetPosition(mGenePos + VECTOR(0.0f, 10.0f, 0.0f));
								mGame->GetActorManager()->GetPHome()->SetGenerateFlag(false);
							}


							c->SetInitPosition(mGenePos);
							c->SetMaxHp((int)(c->GetInitMaxHp() * ((c->GetLevel() + c->GetMaxLevel()) / 10.0f)));
							c->SetHp(c->GetMaxHp());
							mGame->GetActorManager()->GetPHome()->SetBattlePoints(mGame->GetActorManager()->GetPHome()->GetBattlePoints() - mGenerateUsingPoints);

							std::ostringstream oss;
							const char* text = nullptr;
							if (mId <= 1)
							{
								text = "が出撃。";
							}
							else
							{
								text = "を設置。";
							}

							oss << c->GetName().c_str() << text;
							mGame->GetActorManager()->GetStage()->GetLog()->AddText(oss.str());

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
						auto pop = new UIPopUp(mGame, "Generate！！", mousePos, 1, VECTOR2(0.0f, -1.0f));
						pop->SetTextSize(30);
						pop->SetTextColor(COLOR(255, 255, 128));
						pop->NoStrokeRect();
					}
					else
					{
						auto pop = new UIPopUp(mGame, "ポイントが足りない", mousePos, 1, VECTOR2(0.0f, -1.0f));
						pop->SetTextSize(30);
						pop->SetTextColor(COLOR(50, 50, 255));
						pop->NoStrokeRect();
					}
				}
				else
				{
					auto pop = new UIPopUp(mGame, "敵のエリアには出すことができない", mousePos, 1, VECTOR2(0.0f, -1.0f));
					pop->SetTextSize(30);
					pop->SetTextColor(COLOR(50, 50, 255));
					pop->NoStrokeRect();
				}
			}
			else
			{
				auto pop = new UIPopUp(mGame, "マップの外に出すことができない", mousePos, 1, VECTOR2(0.0f, -1.0f));
				pop->SetTextSize(30);
				pop->SetTextColor(COLOR(50, 50, 255));
				pop->NoStrokeRect();
			}
		}
	}
}

void UIGenerate::CreateItemButtons()
{
	VECTOR2 pos = mCancellButton->GetPosition() + VECTOR2(50.0f, 169.0f);
	int buttonNum = 0;
	for (int i = 2; i < 7; i++)
	{
		buttonNum = i;

		if (mId == static_cast<GenerateActor_Id>(i))
		{
			continue;
		}

		auto button = AddRectButton(mItemNames[i - 2],
			[this, buttonNum]()
			{
				auto ui = new UIGenerate(mOwner, mGame, (static_cast<GenerateActor_Id>(buttonNum)));
				CloseMe();
				mOwner->SetGenerate(ui);
			});

		pos.y += 69.0f;
		button->SetPosition(pos);
	}
}

void UIGenerate::CreateGenerateActorLvUpandDownButtons()
{
	mGenerateActorLvPlusButton = AddRectButton("Lv+",
		[this]()
		{
			if (mGenerateActorLevel < mGame->GetActorManager()->GetPHome()->GetLevel())
			{
				mGenerateActorLevel++;
			}
			else
			{
				auto pop = new UIPopUp(mGame, "PlayerHomeのレベルを超えることはできません", mGenerateActorLvPlusButton->GetPosition(), 1, VECTOR2(0.0f, -1.0f));
				pop->SetTextSize(30);
				pop->SetTextColor(COLOR(50, 50, 255));
				pop->NoStrokeRect();
			}
		}
	);

	mGenerateActorLvPlusButton->SetPosition(mChangeButton->GetPosition() + VECTOR2(-25.0f, 69.0f * 2.0f));

	mLvMaxButton = AddRectButton("Max",
		[this]()
		{
			mGenerateActorLevel = mGame->GetActorManager()->GetPHome()->GetLevel();
		}
	);

	mLvMaxButton->SetPosition(mGenerateActorLvPlusButton->GetPosition() - mLvTUButtonPosOffset);

	mGenerateActorLvMinusButton = AddRectButton("Lv-",
		[this]()
		{
			if (mGenerateActorLevel > 0)
			{
				mGenerateActorLevel--;
			}
			else
			{
				auto pop = new UIPopUp(mGame, "レベルをこれ以上下げることはできません", mGenerateActorLvMinusButton->GetPosition(), 1, VECTOR2(0.0f, -1.0f));
				pop->SetTextSize(30);
				pop->SetTextColor(COLOR(50, 50, 255));
				pop->NoStrokeRect();
			}
		}
	);

	mGenerateActorLvMinusButton->SetPosition(mChangeButton->GetPosition() + VECTOR2(-25.0f + 69.0f * 2.0f, 69.0f * 2.0f));


	mLvResetButton = AddRectButton("Reset",
		[this]()
		{
			mGenerateActorLevel = 0;
		}
	);

	mLvResetButton->SetPosition(mGenerateActorLvMinusButton->GetPosition() + mLvTUButtonPosOffset);
}

void UIGenerate::CreateChangeItemGenerateButtons()
{
	if (mGame->GetActorManager()->GetPHome()->GetLevel() >= 5)
	{
		auto ChangeItemButton = AddRectButton("アイテムに切り替え",
			[this]()
			{
				auto ui = new UIGenerate(mOwner, mGame, UIGenerate::GenerateActor_Id::EBarrier);
				CloseMe();
				mOwner->SetGenerate(ui);
			});
		ChangeItemButton->SetPosition(mChangeButton->GetPosition() + VECTOR2(-25.0f, 69.0f));
	}
}
