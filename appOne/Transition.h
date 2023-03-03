#pragma once
#include "Game.h"

class Transition
{
public:
	Transition(class Game* game);
	virtual ~Transition() {};
	virtual void create() {};
	void inTrigger();
	void outTrigger();
	void draw();
	virtual void inTriggerProc() {};
	virtual bool inProc() { return true; }
	virtual void outTriggerProc() {};
	virtual bool outProc() { return true; }
	bool inEndFlag();
	bool outEndFlag();
	Game* GetGame() { return mGame; }
private:
	enum class STATE {
		IN_TRIGGER, IN_NOW, IN_END,
		OUT_TRIGGER, OUT_NOW, OUT_END
	};
	STATE State;
private:
	class Game* mGame;
};

