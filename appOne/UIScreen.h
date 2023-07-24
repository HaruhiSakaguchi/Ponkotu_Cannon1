#pragma once
#include<functional>
#include"COLOR.h"
#include"VECTOR2.h"
#include <string>
#include "sound.h"

//���[�U�[�C���^�[�t�F�C�X���N���X
class UIScreen
{
public:
	UIScreen(class Game* game);
	virtual ~UIScreen();
	// �h���N���X�ŃI�[�o�[���C�h�\
	virtual void ProcessInput();
	virtual void Update() {};
	virtual void Draw();
	virtual void draw() {};
	virtual void DrawAfterButton(){};
	// UI�X�e�[�g�B�A�N�e�B�u���A���邩
	enum class UIState
	{
		EActive,
		EClosing
	};
	void SetBackPos(const VECTOR2& pos) { mBackPos = pos; }
	const VECTOR2& GetBackPos() { return mBackPos; }
	// UI�X�e�[�g��Closing�ɐݒ�
	void CloseMe();
	// UI�X�e�[�g���擾
	UIState GetState() const { return mState; }
	void SetState(UIState state) { mState = state; }
	// UI�^�C�g����ݒ�
	void SetTitle(const char* text) { mTitle = text; }
	const char* GetTitle() { return mTitle.c_str(); }
	// �{�^����ǉ��i�c�ɔz�u����Ă����j
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

	// �X�e�[�g
	UIState mState;

	//�w�i�摜
	int mBackImg;
	VECTOR2 mBackPos;

	//�^�C�g�������Ƃ��̑���
	std::string mTitle;
	COLOR mTitleColor;
	VECTOR2 mTitlePos;
	float mTextSize;
	//�{�^��
	std::vector<class Button*> mButtons;
	VECTOR2 mButtonPos;
	COLOR mButtonTextcolor;
};

