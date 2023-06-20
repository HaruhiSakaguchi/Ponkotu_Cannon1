#pragma once
#include "UIMainState.h"
#include <string>

class Title : public UIMainState
{
public:
	Title(class Game* game);
	~Title();
	void ChangeOption()override;
	void Update()override;
	struct DATA
	{
		int mBgm;
		int mBackImg;
		float mTextSize;
		int mBgmSoundVolumeOffset;
		const char*mTitle;
		COLOR mTitleColor;
		VECTOR2 mBackPos;
		VECTOR2 mTitlePos;
		VECTOR2 mButtonPos;
		VECTOR2 mPhaseFirstButtonOffsetPos;
		VECTOR2 mPhaseNotFirstButtonOffsetPos;
	};
private:
	DATA Data;
	bool mPlayBgmFlag = true;
	std::string mText;
	bool mChangeStateFlag;
	bool mIsChangePlay;
};

