#pragma once
#include "UIState.h"
#include "sound.h"

class Pause :
	public UIState
{
public:
	Pause(class Game* game);
	~Pause();
	void ProcessInput() override;
	void Draw() override;
	void ChangeOption()override;
	struct DATA
	{
		int mSound;
		const char*mTitle;
		int mPauseSoundVolumeOffset;
		VECTOR2 mTitlePos;
		VECTOR2 mButtonPos;
		VECTOR2 mHelpButtonPosOffset;
	};
private:
	DATA Data;
	bool mQuitFlag;
};
