#pragma once
#include "UIState.h"
class Option : public UIState
{
public:
	Option(class Game* game);
	void draw()override;
	struct DATA
	{
		const char*mTitle;
		int mMinVolume;
		int mOneVolume;//ボリューム1段階分の数値
		float mOneCameraS;
		float mMaxCameraS;
		float mTextSize;
		VECTOR2 mTitlePos;
		COLOR mTitleColor;
		VECTOR2 mBackPos;
		VECTOR2 mButtonPos;
		VECTOR2 mBgmVolumeUpButtonOffsetPos;
		VECTOR2 mBgmVolumeUpButtonGuideOffsetPos;
		VECTOR2 mBgmVolumeDownButtonOffsetPos;
		VECTOR2 mBgmVolumeDownButtonGuideOffsetPos;
		VECTOR2 mEffectVolumeUpButtonOffsetPos;
		VECTOR2 mEffectVolumeUpButtonGuideOffsetPos;
		VECTOR2 mEffectVolumeDownButtonOffsetPos;
		VECTOR2 mEffectVolumeDownButtonGuideOffsetPos;
		VECTOR2 mSoundOnOffButtonOfsetPos;
		VECTOR2 mSoundOnOffButtonGuideOfsetPos;
		VECTOR2 mCameraXUpButtonOffsetPos;
		VECTOR2 mCameraXUpButtonGuideOffsetPos;
		VECTOR2 mCameraXDownButtonOffsetPos;
		VECTOR2 mCameraXDownButtonGuideOffsetPos;
		VECTOR2 mCameraYUpButtonOffsetPos;
		VECTOR2 mCameraYUpButtonGuideOffsetPos;
		VECTOR2 mCameraYDownButtonOffsetPos;
		VECTOR2 mCameraYDownButtonGuideOffsetPos;
		VECTOR2 mCloseButtonOffsetPos;
		VECTOR2 mVolumeTextOffsetPos;
		VECTOR2 mBgmTextOffsetPos;
		VECTOR2 mEffectTextOffsetPos;
		VECTOR2 mEVolTextOffsetPos;
		VECTOR2 mBVolTextOffsetPos;
		VECTOR2 mSoundOnOffTextOffsetPos;
		VECTOR2 mBgmOnOffTextOffsetPos;
		VECTOR2 mCameraSTextOffsetPos;
		VECTOR2 mCameraSXTextOffsetPos;
		VECTOR2 mCameraSYTextOffsetPos;
		VECTOR2 mCameraSXNumTextOffsetPos;
		VECTOR2 mCameraSYNumTextOffsetPos;
		COLOR mOnColor;
		COLOR mOffColor;
	};
private:
	DATA Data;
private:
	int mVolume;
	int mEffectVolume;
};