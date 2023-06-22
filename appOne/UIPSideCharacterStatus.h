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

	struct DATA
	{
		float mWidth;
		float mHeight;
		float mRadius;
		float mDiamiter;
		float mAngleOffset;
		float mAngle;//リングのアングルの一時保存用
		float mRectWidth;//バリア―のHpゲージ表示用
		float mTriangleWidth;
		float mTriangleHeight;
		float mTriangleAngle120;
		float mTriangleAngle30;
		float mChangeTimeSpeed;
		float mChangeWidthSpeed;
		float mItemNameTextSize;
		float mBarrierHpGaugeWidth;
		float mBarrierHpGaugeOffsetPosY;
		float mBarrierHpGaugeHeight;
		float mWhiteTriangleOffsetPosX;
		float mLineSw;
		COLOR mTriangleColor;
		COLOR mWhiteTriangleColor;
		COLOR mTimeGaugeColor;
		COLOR mBarrierHpGaugeColor;
		COLOR mRingOnLineColor;
		COLOR mItemNameTextColor;
	};
private:
	DATA Data;
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

