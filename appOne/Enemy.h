#pragma once
#include "CharacterActor.h"
class Enemy :
    public CharacterActor
{
public:
    Enemy(class Game* game);
    ~Enemy();
	//残機関係
	int GetRevivalLimmit() { return mRevivalLimmit; }
	int GetRevivalCnt() { return mRevivalCnt; }
	void SetRevivalLimmit(int limmit) { mRevivalLimmit = limmit; }
	void SetRevivalCnt(int cnt) { mRevivalCnt = cnt; }
private:
	//復活可能回数の最大値
	int mRevivalLimmit;
	//実際の復活可能回数
	int mRevivalCnt;
};

