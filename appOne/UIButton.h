#pragma once
#include <functional>
#include "VECTOR2.h"
#include "sound.h"
#include "COLOR.h"
#include <string>
#include "graphic.h"

class Button
{
public:
	//このクラスを使用前に画像をセットしておく
	static void SetButtonOffImg(int img);
	static void SetButtonOnImg(int img);

	Button(const char* name,
		std::function<void()> onClick,
		const VECTOR2& pos,
		class Game* game,
		int imgNum = 1,
		const char* text = 0,
		const VECTOR2& textOffset = VECTOR2(0, 0)
	);
	~Button() {};

	void Draw();
	void Update();
	void DrawGuide();

	const VECTOR2& GetPosition() { return mPosition; }
	void SetPosition(const VECTOR2& pos) { mPosition = pos; }

	// マウスポインタがこのボタン内にあるか
	bool ContainsPoint(const VECTOR2& pt, bool flag = true);
	// クリックされたときに呼び出される関数
	void OnClick();
	// Getter
	const VECTOR2& GetDimensions() const { return mDimensions; }
	const VECTOR2& GetDimensions2() const { return mDimensions2; }
	void ContainsSound();
	bool GetSound() { return mSoundFlag; }
	void SetName(const char* name) { mName = name; }
	const char* GetText() { return mText; }
	struct DATA
	{
		int mClickSound;
		int mContainsSound;
		int mClickSoundVolumeOffset;
		int mContainsSoundVolumeOffset;
		float mButtonTextSize;
		float mGuideTextSize;
		float mGuideRectSw;
		COLOR mButtonTextColor;
		COLOR mGuideTextColor;
		COLOR mGuideRectColor;
	};
private:
	static int mButtonOnImg;
	static int mButtonOffImg;
	static int mButtonOnImg2;
	static int mButtonOffImg2;
	static VECTOR2 mDimensions;//ボタンの幅と高さ
	static VECTOR2 mDimensions2;//2つ目まで画像を登録できる
private:
	std::string mName;
	VECTOR2 mPosition;
	bool mHighlighted;
	const char* mText;
	VECTOR2 mTextOffset;
	bool mSoundFlag;
	std::function<void()> mOnClick;
	int mImageNum;
	Game* mGame;
private:
	DATA Data;
};

