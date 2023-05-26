#pragma once
#include "UIScreen.h"
#include "CharacterActor.h"

class UIPSideCharacterStatusClose
	: public UIScreen
{
public:
	UIPSideCharacterStatusClose(class CharacterActor*owner);
	void DrawHpGauge();
	void DrawNameandLevel();
	void draw()override;
	void Update()override;
private:
	CharacterActor* mOwner;
	float mHpGaugeWidth;
	VECTOR2 mPos;
	Button* mOpenButtun;
};

