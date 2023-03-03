#pragma once
#include "Transition.h"

class TransitionFade :
	public Transition
{
public:
	TransitionFade(class Game* game);
	void create()override;
	void inTriggerProc();
	bool inProc();
	void outTriggerProc();
	bool outProc();
private:
	COLOR mColor;
	float mTime;
};

