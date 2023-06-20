#include "Transition.h"

Transition::Transition(class Game* game)
	: mGame(game)
	, State(STATE::OUT_END)
{
}

void Transition::inTrigger()
{
	if (State == STATE::OUT_END) 
	{
		State = STATE::IN_TRIGGER;
	}
}

void Transition::outTrigger()
{
	if (State == STATE::IN_END) 
	{
		State = STATE::OUT_TRIGGER;
	}
}

void Transition::Draw() {
	if (State == STATE::IN_END ||State == STATE::OUT_END) 
	{
		return;
	}
	switch (State) 
	{
	case STATE::IN_TRIGGER:
		inTriggerProc();
		State = STATE::IN_NOW;
		break;
	case STATE::IN_NOW:
		if (inProc())
		{
			State = STATE::IN_END;
		}
		break;
	case STATE::OUT_TRIGGER:
		outTriggerProc();
		State = STATE::OUT_NOW;
		break;
	case STATE::OUT_NOW:
		if (outProc())
		{
			State = STATE::OUT_END;
		}
		break;
	case STATE::IN_END:
		break;
	case STATE::OUT_END:
		break;
	}
}

//inÇ‹ÇΩÇÕoutÇ™èIóπÇµÇΩÇ©Ç«Ç§Ç©
bool Transition::inEndFlag()
{
	bool flag = State == STATE::IN_END;
	return flag;
}

bool Transition::outEndFlag()
{
	bool flag = State == STATE::OUT_END;
	return flag;
}
