#include "SoundVolumeManager.h"
#include "Game.h"

SoundVolumeManager::SoundVolumeManager(class Game* game)
	: Manager(game)
	, mEffectVol(0)
	, mSetVolume(0)
	, mTmpEffectVol(0)
	, mTmpVolume(0)
	, mSoundFlag(true)
	, mBgmFlag(true)
{
	mEffectVol = mGame->GetAllData()->mInitAllSoundVolume;
	mSetVolume = mGame->GetAllData()->mInitEffectSoundVolume;
}
