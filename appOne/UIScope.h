#pragma once
#include "UIScreen.h"
#include "Cannon.h"

class UIScope :
	public UIScreen
{
public:
	UIScope(class Game* game, class Cannon* owner);
	void Update()override;
	void Draw()override;
	void DrawRing(const VECTOR2& pos, float radius, float sw, const COLOR& color);
	void DrawRing(const VECTOR2& pos, float radius, float sw, float angle = 1, const COLOR& color = COLOR(255, 255, 55));
	void DrawRingOnLine(const VECTOR2& pos, float radius, float dia, float angle, const COLOR& color);
	void SetPos(VECTOR2 pos) { Data.mPos = pos; }
	void SetPosX(float pos) { Data.mPos.x = pos; }
	void SetPosY(float pos) { Data.mPos.y = pos; }
	void SetOffsetPos(VECTOR2 pos) { Data.mOffsetPos = pos; }
	VECTOR2& GetPos() { return Data.mPos; }
	VECTOR2& GetOffset() { return Data.mOffsetPos; }
	struct DATA
	{
		float mGaugeRingRadius;
		float mGaugeRingWeight;
		float mCrossSw;
		float mCrossPosOffset;
		int mMaxItenNum;
		float mStockItemWeight;
		float mItemCylinderRadius;
		float mStockItemOffsetX;
		float mChangeAngleSpeed;
		float mChangePositionSpeed;
		float mMouseOffsetPosY;
		float mMouseOffsetPosX;
		float mItemNameTextSize;
		float mItemNameWindowSw;
		float mCylinderAngle;
		const char* mStrings[6];
		VECTOR2 mPos;
		VECTOR2 mOffsetPos;
		COLOR mColors[6];
		COLOR mCrossColor;
		COLOR mWhiteRingColor;
		COLOR mGaugeRingColor;
		COLOR mRingOnLineColor;
		COLOR mItemNameColor;
		COLOR mItemNameWindowColor;
		COLOR mItemNameWindowStrokeColor;
	};
private:
	DATA Data;
	class Cannon* mOwner;

};

