#pragma once
#include "CharacterActor.h"
class Enemy :
    public CharacterActor
{
public:
    Enemy(class Game* game);
    ~Enemy();
	//�c�@�֌W
	int GetRevivalLimmit() { return mRevivalLimmit; }
	int GetRevivalCnt() { return mRevivalCnt; }
	void SetRevivalLimmit(int limmit) { mRevivalLimmit = limmit; }
	void SetRevivalCnt(int cnt) { mRevivalCnt = cnt; }
private:
	//�����\�񐔂̍ő�l
	int mRevivalLimmit;
	//���ۂ̕����\��
	int mRevivalCnt;
};

