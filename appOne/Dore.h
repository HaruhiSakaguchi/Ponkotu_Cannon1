#pragma once
#include "Actor.h"
class Dore : public Actor
{
public:
	Dore(class Game* game);
	int SetUp();
	void UpdateActor();
	bool OpenDore();
	bool CloseDore();
	void Open() { mBeginOpenFlag = true; }
	void Close() { mBeginCloseFlag = true; }
	void SetIsRotate(bool flag) { mIsRotate = flag; }
	bool GetCloseComplete() { return mCloseComplete; }
	bool GetOpenComplete() { return mOpenComplete; }
	bool GetBeginOpen() { return mBeginOpenFlag; }
private:
	bool mBeginOpenFlag;
	bool mBeginCloseFlag;
	bool mOpenComplete;
	bool mCloseComplete;
	bool mIsRotate;
};

