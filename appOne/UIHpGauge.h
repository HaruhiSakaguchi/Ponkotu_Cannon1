#pragma once
#include "UIScreen.h"
#include "VECTOR2.h"
#include "COLOR.h"

class UIHpGauge :
	public UIScreen
{
public:
	UIHpGauge(class Game* game, class CharacterActor* owner);
	void Draw()override;
	void Update()override;
	struct DATA
	{
		int mMaxHp;
		float mHeight;
		float mWidth;
		float mGaugeWidth;
		float mSw;
		float mHpTextSize;
		float mNormalHpRate;
		float mDangerHpRate;
		float mHpGaugeSw;
		float mChangeHpSpeed;
		VECTOR2 mPos;
		COLOR mNormalColor;
		COLOR mDyingColor;
		COLOR mDangarColor;
		COLOR mHpWindowColor;
		COLOR mWhite;
		COLOR mOverMaxHpColor;
	};
private:
	DATA Data;
	class CharacterActor* mOwner;
};

