#include "UIButton.h"
#include "Game.h"
#include "input.h"

int Button::mButtonOffImg = -1;
int Button::mButtonOnImg = -1;
VECTOR2 Button::mDimensions(0, 0);
int Button::mButtonOffImg2 = -1;
int Button::mButtonOnImg2 = -1;
VECTOR2 Button::mDimensions2(0, 0);

Button::Button(const char* name, std::function<void()> onClick,
	const VECTOR2& pos, Game* game, int imgNum, const char* text, const VECTOR2& textOffset)
	: mGame(game)
	, mName(name)
	, mPosition(pos)
	, mOnClick(onClick)
	, mImageNum(imgNum)
	, mText(text)
	, mTextOffset(textOffset)
	, mHighlighted(false)
	, mSoundFlag(false)
{
	Data = mGame->GetAllData()->buttonData;
}

void Button::SetButtonOffImg(int img)
{
	if (mButtonOffImg == -1)
	{
		mButtonOffImg = img;
		mDimensions = getTextureSize(img);
	}
	else
	{
		if (mButtonOffImg2 == -1)
		{
			mButtonOffImg2 = img;
			mDimensions2 = getTextureSize(img);
		}
	}
}

void Button::SetButtonOnImg(int img)
{
	if (mButtonOnImg == -1)
	{
		mButtonOnImg = img;
	}
	else
	{
		if (mButtonOnImg2 == -1)
		{
			mButtonOnImg2 = img;
		}
	}
}

bool Button::ContainsPoint(const VECTOR2& pt, bool flag)
{
	bool no = true;
	if (mImageNum == 1)
	{
		no =
			pt.x < (mPosition.x - mDimensions.x / 2.0f) ||
			pt.x >(mPosition.x + mDimensions.x / 2.0f) ||
			pt.y < (mPosition.y - mDimensions.y / 2.0f) ||
			pt.y >(mPosition.y + mDimensions.y / 2.0f);
	}
	else if (mImageNum == 2)
	{
		no =
			pt.x < (mPosition.x - mDimensions2.x / 2.0f) ||
			pt.x >(mPosition.x + mDimensions2.x / 2.0f) ||
			pt.y < (mPosition.y - mDimensions2.y / 2.0f) ||
			pt.y >(mPosition.y + mDimensions2.y / 2.0f);
	}

	mHighlighted = !no;

	//ボタンの外にマウスカーソルがあったら次にボタンに触れたときに音声を再生するようにする
	if (no)
	{
		mSoundFlag = false;
	}

	return !no;
}

void Button::OnClick()
{
	// アタッチされた関数ハンドラがあれば呼び出す。
	if (mOnClick)
	{
		mOnClick();
		playSound(Data.mClickSound);
	}
}

void Button::Draw()
{
	// ボタン画像表示
	int buttonImg = 0;
	bool mChangeImageFlag = false;
	for (auto button : mGame->GetUIStack().back()->GetButtons())
	{
		if (button == this)
		{
			mChangeImageFlag = true;
		}
	}
	if (mChangeImageFlag)
	{
		if (mImageNum == 1)
		{
			buttonImg = mHighlighted ? mButtonOnImg : mButtonOffImg;
		}
		else if (mImageNum == 2)
		{
			buttonImg = mHighlighted ? mButtonOnImg2 : mButtonOffImg2;
		}

		VECTOR2 mousePos = VECTOR2(mouseX, mouseY);
		if (ContainsPoint(mousePos) && GetText())
		{
			DrawGuide();
		}
	}
	else
	{
		if (mImageNum == 1)
		{
			buttonImg = mButtonOffImg;
		}
		else if (mImageNum == 2)
		{
			buttonImg = mButtonOffImg2;
		}
	}

	if (buttonImg >= 0)
	{
		image(buttonImg, mPosition.x, mPosition.y);
	}
	// ボタン文字表示
	textSize(Data.mButtonTextSize);
	VECTOR2 pos;
	pos.x = mPosition.x - mName.length() * Data.mButtonTextSize / 4.0f;//半角文字のみ対応
	pos.y = mPosition.y + Data.mButtonTextSize / 2.0f - 2.0f;
	fill(Data.mButtonTextColor);
	text(mName.c_str(), pos.x, pos.y);

}

void Button::Update()
{
	//ボタンの上にマウスカーソルがあったら一度だけ呼び出す。ボタンの外に出るとまた音を鳴らすことができるようになる。
	ContainsSound();
	mSoundFlag = true;
	setVolume(Data.mClickSound, mGame->GetEffectVolume() + Data.mClickSoundVolumeOffset);
	setVolume(Data.mContainsSound, mGame->GetEffectVolume() + Data.mContainsSoundVolumeOffset);
}

void Button::DrawGuide()
{
	float mOffsetPoM = 1.0f;
	if (mPosition.x - strlen(mText) / 4.0f * Data.mGuideTextSize + strlen(mText) / 2.0f * Data.mGuideTextSize > width)
	{
		mOffsetPoM *= -1;
	}
	VECTOR2 pos = VECTOR2(mPosition.x - strlen(mText) / 4.0f * Data.mGuideTextSize + strlen(mText) / 2.0f * Data.mGuideTextSize * mOffsetPoM + mTextOffset.x, mPosition.y + mTextOffset.y + Data.mGuideTextSize / 2.0f);

	fill(Data.mGuideTextColor);
	textSize(Data.mGuideTextSize);
	text(mText, pos.x, pos.y);
	stroke(Data.mGuideTextColor);
	strokeWeight(Data.mGuideRectSw);
	rectMode(CORNER);
	fill(Data.mGuideRectColor);
	rect(pos.x, pos.y - Data.mGuideTextSize, strlen(mText) * Data.mGuideTextSize / 2.0f, Data.mGuideTextSize);
	rectMode(CENTER);
}

void Button::ContainsSound()
{
	playSound(Data.mContainsSound);
}
