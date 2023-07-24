#include "UIManager.h"
#include "UIScreen.h"
#include "UIPSideCharacterStatusBase.h"
#include "Game.h"

UIManager::UIManager(class Game* game)
	: Manager(game)
	, mUpdatingUIs(false)
{
}

UIManager::~UIManager()
{
	UIClear();
}

void UIManager::Update()
{
	//ui��update

	mUpdatingUIs = true;

	for (auto ui : mUIStack)
	{
		if (ui->GetState() == UIScreen::UIState::EActive)
		{
			ui->Update();
		}
	}

	mUpdatingUIs = false;

	// �ǉ�����������Actor��mActors�ɒǉ�����
	for (auto pending : mPendingUIs)
	{
		mUIStack.emplace_back(pending);
	}

	mPendingUIs.clear();

	// UI��delete
	std::vector<UIScreen*> deadUis;
	for (auto ui : mUIStack)
	{
		if (ui->GetState() == UIScreen::UIState::EClosing)
		{
			deadUis.emplace_back(ui);
		}
	}

	for (auto ui : deadUis)
	{
		delete ui;
	}
}

void UIManager::ProcessInput()
{
	if (mGame->GetState() == Game::GameState::EGameplay)
	{
		mUpdatingUIs = true;

		for (auto ui : mUIStack)
		{
			ui->ProcessInput();
		}

		mUpdatingUIs = false;
	}
	else if (!mUIStack.empty())
	{
		mUpdatingUIs = true;

		mUIStack.back()->ProcessInput();

		mUpdatingUIs = false;
	}
}

void UIManager::PushUI(class UIScreen* uiScreen)
{
	if (mUpdatingUIs)
	{
		mPendingUIs.emplace_back(uiScreen);
	}
	else
	{
		mUIStack.emplace_back(uiScreen);
	}
}

void UIManager::PullUI(class UIScreen* uiScreen)
{
	auto iter = std::find(mUIStack.begin(), mUIStack.end(), uiScreen);
	if (iter != mUIStack.end())
	{
		std::iter_swap(iter, mUIStack.end() - 1);
		mUIStack.pop_back();
	}
}

void UIManager::AddUIPSide(UIPSideCharacterStatusBase* uiScreen)
{
	mUIStatus.emplace_back(uiScreen);
}

void UIManager::RemoveUIPSide(UIPSideCharacterStatusBase* uiScreen)
{
	auto iter = std::find(mUIStatus.begin(), mUIStatus.end(), uiScreen);
	if (iter != mUIStatus.end())
	{
		std::iter_swap(iter, mUIStatus.end() - 1);
		mUIStatus.pop_back();
	}
}

void UIManager::UIClear()
{
	while (!mUIStack.empty())
	{
		delete mUIStack.back();
	}
}