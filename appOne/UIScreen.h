#pragma once
#include<functional>
#include"COLOR.h"
#include"VECTOR2.h"
#include <string>
#include "sound.h"

//ユーザーインターフェイス基底クラス
class UIScreen
{
public:
	UIScreen(class Game* game);
	virtual ~UIScreen();
	// 派生クラスでオーバーライド可能
	virtual void ProcessInput();
	virtual void Update() {};
	virtual void Draw();
	virtual void draw() {};
	virtual void DrawAfterButton(){};
	// UIステート。アクティブか、閉じるか
	enum class UIState
	{
		EActive,
		EClosing
	};
	void SetBackPos(const VECTOR2& pos) { mBackPos = pos; }
	const VECTOR2& GetBackPos() { return mBackPos; }
	// UIステートをClosingに設定
	void CloseMe();
	// UIステートを取得
	UIState GetState() const { return mState; }
	void SetState(UIState state) { mState = state; }
	// UIタイトルを設定
	void SetTitle(const char* text) { mTitle = text; }
	const char* GetTitle() { return mTitle.c_str(); }
	// ボタンを追加（縦に配置されていく）
	class Button *AddButton(const char* name, std::function<void()> onClick, int imgNum = 1, const char* text = 0, const VECTOR2& ButtonOffsetPos = VECTOR2(0, 0), const VECTOR2& GuideTextOffset = VECTOR2(0, 0));
	class Button *AddRectButton(const char* name, std::function<void()> onClick,const char* text = 0,const VECTOR2 dim = VECTOR2(50.0f,50.0f), const COLOR onColor = COLOR(200, 180, 116), const COLOR& offColor = COLOR(99, 163, 200));

	void Line(const VECTOR2& p1, const VECTOR2& p2);
	void LineForRect(const VECTOR2& p1, const VECTOR2& p2, const VECTOR2& p3, const VECTOR2& p4)
	{
		Line(p1, p2);
		Line(p1, p3);
		Line(p2, p4);
		Line(p3, p4);
	}
	std::vector<class Button*>& GetButtons() { return mButtons; }
	struct DATA
	{
		int mOnButton1;
		int mOffButton1;
		int mOnButton2;
		int mOffButton2;
		float mButtonPosOffsetY;
	};
private:
	DATA Data;
protected:
	class Game* mGame;

	// ステート
	UIState mState;

	//背景画像
	int mBackImg;
	VECTOR2 mBackPos;

	//タイトル文字とその属性
	std::string mTitle;
	COLOR mTitleColor;
	VECTOR2 mTitlePos;
	float mTextSize;
	//ボタン
	std::vector<class Button*> mButtons;
	VECTOR2 mButtonPos;
	COLOR mButtonTextcolor;
};

