#pragma once
#include "UIState.h"
#include "sound.h"

class GamePlay : public UIState
{
public:
	GamePlay(class Game* game);
	~GamePlay();
	void Update()override;
	struct DATA
	{
		int mBgm1SoundVolumeOffset;
		int mBgm2SoundVolumeOffset;
		int mBgm1;
		int mBgm2;
	};
private:
	DATA Data;
private:
	int mGameOverFlag;
	int mGameClearFlag;
	class Map* mMap;
	int mBgm;
	int mSoundOffset;
	bool mPlayBgmFlag;
};