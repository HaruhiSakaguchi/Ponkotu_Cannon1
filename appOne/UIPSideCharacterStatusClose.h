#pragma once
#include "UIPSideCharacterStatusBase.h"
#include "CharacterActor.h"

class UIPSideCharacterStatusClose
	: public UIPSideCharacterStatusBase
{
public:
	UIPSideCharacterStatusClose(class CharacterActor*owner);
	void DrawNameandLevel();
	void draw()override;
	void update()override;
private:
	Button* mOpenButtun;
};

