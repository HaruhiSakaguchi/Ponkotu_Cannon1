#pragma once
#include "UIScreen.h"
class UIPSideCharacterStatusBase :
	public UIScreen
{
public:
	UIPSideCharacterStatusBase(class PSideCharacterActor* owner);
	virtual  ~UIPSideCharacterStatusBase();
	class PSideCharacterActor* GetOwner() { return mOwner; }
	const VECTOR2& GetPosition() { return mPosition; }
	void SetPosition(const VECTOR2& pos) { mPosition = pos; }
	const VECTOR2& GetOffset() { return mOffset; }
	void SetOffset(const VECTOR2& pos) { mOffset = pos; }
	void OtherPSideUIsTranslate(const VECTOR2&offset);
	void Update()override;
	virtual void update() {};
	void DrawHpGauge(const VECTOR2&pos);
	void DrawBarrierHpGauge(const VECTOR2&pos);
	void OwnerLvUp();
	const VECTOR2& GetNextOffset() { return mNextPSideStatusOffset; }
	void UIOpenNexrOffsetPlus();

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
protected:
	DATA Data;
protected:
	float mHpGaugeWidth;
	float mBarrierHpGaugeWidth;
	class PSideCharacterActor* mOwner;
	VECTOR2 mPosition;
	VECTOR2 mOffset;
	Button* mLvUpButton;
	VECTOR2 mNextPSideStatusOffset;
};

