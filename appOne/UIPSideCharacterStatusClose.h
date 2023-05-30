#pragma once
#include "UIPSideCharacterStatusBase.h"
#include "CharacterActor.h"

class UIPSideCharacterStatusClose
	: public UIPSideCharacterStatusBase
{
public:
	UIPSideCharacterStatusClose(class CharacterActor*owner);
	void DrawHpGauge();
	void DrawNameandLevel();
	void draw()override;
	void Update()override;
private:
	float mHpGaugeWidth;
	Button* mOpenButtun;
	Button* mLvUpButton;

};

