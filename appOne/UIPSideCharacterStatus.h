#pragma once
#include "UIPSideCharacterStatusBase.h"
class UIPSideCharacterStatus :
    public UIPSideCharacterStatusBase
{
public:
    UIPSideCharacterStatus(class CharacterActor* owner);

    void draw()override;
    void Update()override;
	void DrawRing(const VECTOR2& pos, float radius, float sw, const COLOR& color);
	void DrawRing(const VECTOR2& pos, float radius, float sw, float angle = 1, const COLOR& color = COLOR(255, 255, 55), float angleOffset = 0);
	void DrawRingOnLine(const VECTOR2& pos, float radius, float dia, float angle, const COLOR& color);
	void DrawHpGauge();
	void DrawLaunchIntervalGauge();
	void DrawItemCompoLifeSpanGauge(class ItemComponent*item,const VECTOR2&pos);
	void DrawNameandLevel();
	void DrawStateCursor();
	void DrawAfterButton()override;
private:
    COLOR mNoItemColor = COLOR(128, 128, 128);
	float mCylinderAngle;
	float mBarrierHpWidth;
	Button* mStayButton;
	Button* mReturnButton;
	Button* mHomePatrollButton;
	Button* mFieldPatrollButton;
	Button* mCloseButton;
	VECTOR2 mStateCursorPos;
};

