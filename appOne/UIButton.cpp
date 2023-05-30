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
	else
	{
		no = pt.x < (mPosition.x - mRectButtonDim.x / 2.0f) ||
			pt.x >(mPosition.x + mRectButtonDim.x / 2.0f) ||
			pt.y < (mPosition.y - mRectButtonDim.y / 2.0f) ||
			pt.y >(mPosition.y + mRectButtonDim.y / 2.0f);
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
	/*for (auto button : mGame->GetUIStack().back()->GetButtons())
	{
		if (button == this)
		{
			mChangeImageFlag = true;
		}
	}*/
	if (!mChangeImageFlag)
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

	if (buttonImg >= 0 && mImageNum <= 2)
	{
		image(buttonImg, mPosition.x, mPosition.y);
	}

	if (mImageNum > 2)
	{
		COLOR color = mHighlighted ? mContainsColor : mNoContainsColor;
		stroke(color);
		strokeWeight(5.0f);
		fill(color);
		rect(mPosition.x, mPosition.y, mRectButtonDim.x, mRectButtonDim.y);
		fill(COLOR(128, 128, 128, 64));
		noStroke();
		rect(mPosition.x, mPosition.y + mRectButtonDim.y * 0.5f / 2.0f, mRectButtonDim.x + 5.0f, mRectButtonDim.y * 0.25f + 5.0f);
		fill(COLOR(64, 64, 64, 64));
		noStroke();
		rect(mPosition.x, mPosition.y + mRectButtonDim.y * 0.95f / 2.0f, mRectButtonDim.x + 5.0f, mRectButtonDim.y * 0.05f + 5.0f);
		fill(color);
		stroke(color);
		rect(mPosition.x, mPosition.y + mRectButtonDim.y * 0.9875f / 2.0f - (mRectButtonDim.y * 0.05f + 5.0f), mRectButtonDim.x + 5.0f, mRectButtonDim.y * 0.0125f + 5.0f);
		VECTOR2 sp = mPosition - mRectButtonDim / 2 - VECTOR2(2.0f, 2.0f);
		VECTOR2 ep = sp + VECTOR2(mRectButtonDim.x + 2.0f, 0.0f);
		fill(COLOR(64, 64, 64, 64));
		stroke(COLOR(64, 64, 64, 64));
		strokeWeight(2.0f);
		line(sp.x, sp.y, ep.x, ep.y);
	}

	// ボタン文字表示
	textSize(Data.mButtonTextSize);
	VECTOR2 pos;
	pos.x = mPosition.x + mName.length() * Data.mButtonTextSize / 4.0f;//半角文字のみ対応
	pos.y = mPosition.y + Data.mButtonTextSize / 2.0f - 2.0f;
	fill(Data.mButtonTextColor);
	char* j;
	std::string name;
	if (mImageNum > 2 && mRectButtonDim.x < mRectButtonDim.y)
	{
		pos.x = mPosition.x - Data.mButtonTextSize / 2.0f;
		pos.y = mPosition.y;
		for (int i = 0; i < (int)(mName.length() / 2); i++)
		{
		    name = mName.substr(i * 2,2);
			/*char j[7] = mName.c_str();
			char n[7];
			strncpy_s(n, sizeof(n), j + i, 1);*/
			pos.y += Data.mButtonTextSize * i;
			text(name.c_str(), pos.x, pos.y);
		}
	}
	else
	{
		text(mName.c_str(), pos.x, pos.y);
	}

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
