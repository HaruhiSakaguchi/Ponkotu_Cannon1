#include "UIScreen.h"
#include "graphic.h"
#include "input.h"
#include "Game.h"
#include "UIButton.h"
#include "Container.h"
#include "TransitionFade.h"

UIScreen::UIScreen(Game* game)
	: mGame(game)
	, mState(EActive)
	, mBackImg(-1)
	, mTextSize(40.0f)
	, mTitlePos(width / 2.0f, 100.0f)
	, mButtonPos(width / 2.0f, 170.0f)
	, mBackPos(0.0f,0.0f)
	, mTitleColor(0,0,0)
	, mButtonTextcolor(0,0,0)
{
	Data = mGame->GetAllData()->uiScreenData;
	//GameのUIStackに追加
	mGame->GetUIManager()->PushUI(this);
	//ボタン画像
	Button::SetButtonOnImg(Data.mOnButton1);
	Button::SetButtonOffImg(Data.mOffButton1);
	Button::SetButtonOnImg(Data.mOnButton2);
	Button::SetButtonOffImg(Data.mOffButton2);
}

UIScreen::~UIScreen()
{
	for (auto b : mButtons)
	{
		delete b;
	}
	mButtons.clear();
	mGame->GetUIManager()->PullUI(this);
}

void UIScreen::ProcessInput()
{
	// ボタンがあるなら
	if (!mButtons.empty())
	{
		// マウスの位置を取得
		VECTOR2 mousePos = VECTOR2(mouseX, mouseY);
		// 全ボタンのチェック
		for (auto b : mButtons)
		{
			// ロールオーバーしていたら
			if (b->ContainsPoint(mousePos))
			{
				if (mGame->GetTransition()->inEndFlag())
				{
					//サウンドフラッグが立っていなかったら音を鳴らす。鳴らすとサウンドフラッグが立つ。
					if (!b->GetSound())
					{
						b->Update();
					}
					if (isTrigger(MOUSE_LBUTTON))
					{
						b->OnClick();
					}
				}
			}
		}
	}
}

void UIScreen::Draw()
{
	rectMode(CENTER);
	// 背景画像表示（もしあったら）
	if (mBackImg >= 0)
	{
		image(mBackImg, mBackPos.x, mBackPos.y);
	}

	draw();

	rectMode(CENTER);

	// タイトル表示（もしあったら）
	if (mTitle.c_str())
	{
		textSize(mTextSize);
		fill(mTitleColor);
		text(mTitle.c_str(),
			mTitlePos.x - mTitle.length() * mTextSize / 4.0f,//半角文字のみ対応
			mTitlePos.y + mTextSize / 2.0f
		);
	}

	// ボタン表示（もしあったら）
	for (auto b : mButtons)
	{
		b->Draw();
	}

	DrawAfterButton();

}

void UIScreen::CloseMe()
{
	mState = EClosing;
}

//ボタンは縦に並んで配置される
Button* UIScreen::AddButton(const char* name, std::function<void()> onClick, int imgNum, const char* text, const VECTOR2& offsetPos, const VECTOR2& textOffset)
{
	Button* b = new Button(name, onClick, mButtonPos + offsetPos, mGame, imgNum, text, textOffset);
	mButtons.emplace_back(b);

	// 次に追加されるボタンの位置を計算しておく（画像の高さ＋間隔）
	if (imgNum == 1)
	{
		mButtonPos.y += b->GetDimensions().y + Data.mButtonPosOffsetY;
	}
	else if (imgNum == 2)
	{
		mButtonPos.y += b->GetDimensions2().y + Data.mButtonPosOffsetY;
	}
	return b;
}

Button* UIScreen::AddRectButton(const char* name, std::function<void()> onClick, const char* text, const VECTOR2 dim, const COLOR onColor, const COLOR& offColor)
{
	Button* b = new Button(name, onClick, mButtonPos, mGame, 3, text);
	b->SetRectButtonDim(dim);
	b->SetContainsRectButtonCOLOR(onColor);
	b->SetNoContainsRectButtonCOLOR(offColor);
	mButtons.emplace_back(b);
	mButtonPos.x += dim.x + 5.0f;
	return b;
}

void UIScreen::Line(const VECTOR2& p1, const VECTOR2& p2) { line(p1.x, p1.y, p2.x, p2.y); }
