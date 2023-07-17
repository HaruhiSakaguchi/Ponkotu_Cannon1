#pragma once
#include "VECTOR.h"
#include "Map.h"
#include "UIScreen.h"

class UIMiniMap : public UIScreen
{
public:
	UIMiniMap(class Game* game, class Map* owner, bool scroll = true);
	~UIMiniMap();
	void Draw()override;
	void Update()override;
	void Create();
	void Arrow(const VECTOR2& pos = VECTOR2(0, 0), const COLOR& color = COLOR(255, 255, 255), float angle = 0);
	const VECTOR2& PosConvert2D(const VECTOR& pos);
	bool IsMiniMapCharaOnMouseCursor(const VECTOR2&pos);
	struct DATA
	{
		float mMiniHpGaugeMaxWidth;
		float mMiniHpGaugeHeight;
		float mMiniMapWindowWidth;
		float mMiniMapWindowHeight;
		int mNumModels;
		float mOffsetX;
		float mOffsetY;
		float mSw;
		float mMinPosX;
		float mMinPosY;
		float mMiniMapOffsetX;
		float mMiniMapOffsetY;
		float mMiniMapWindowLength;
		float mItemSw;
		float mWeaponSw;
		float mItemCircleRadius;
		float m3DCoordinate2DConvertRate;
		float mStage1OffsetX;
		float mStage1OffsetY;
		float mStage2OffsetX;
		float mStage2OffsetY;
		float mStage3OffsetX;
		VECTOR2 mMiniMapCornerPos;
		COLOR mItemStrokeColor;
		COLOR mArrowStrokeColor;
		COLOR mWindowColor;
		COLOR mFloorColor;

	};
private:
	DATA Data;
private:
	class Map* mMap;
	VECTOR2* mPositions;
	float* mWidths;
	float* mHeights;
	VECTOR** mAreaPoints;
	float* mMaxX;
	float* mMaxZ;
	float* mMinX;
	float* mMinZ;
	float m0toMaxYDist;
	bool mScroll;
};

