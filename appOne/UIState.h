#pragma once
#include "UIScreen.h"

//UI���p�������X�e�[�g�J�ڐ�p�N���X

class UIState : public UIScreen
{
public:
	UIState(class Game* game);
	virtual int SetUp() { return 1; }
	virtual void Update()override;
	void ChangeState();
	virtual void ChangeOption() {};//�X�e�[�g�J�ڂ̍ۂɒǉ���Update�̒��ŌĂяo���B
};