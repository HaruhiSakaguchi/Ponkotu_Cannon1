#pragma once
#include "UIScreen.h"
class UIItemStatus : public UIScreen
{
public:
	UIItemStatus(class Cannon* owner);
	void Draw()override;
	void Update()override;
	void DrawRing(const VECTOR2& pos, float radius, float sw, const COLOR& color);
	void DrawRing(const VECTOR2& pos, float radius, float sw, float angle = 1, const COLOR& color = COLOR(255, 255, 55), float angleOffset = 0);
	void DrawRingOnLine(const VECTOR2& pos, float radius, float dia, float angle, const COLOR& color);
	void DrawRightTriangle(const VECTOR2& pos, float width, float hegiht, float angle, const COLOR& color, const char* vec = "上");
	void DrawTimeGauge(class ItemComponent* item);
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
	Cannon* mOwner;
	DATA Data;
};