#pragma once
#include "Manager.h"

class SoundVolumeManager
	:public Manager
{
public:
	SoundVolumeManager(class Game*game);
public:
	int GetEffectVolume() { return mEffectVol; }
	int GetTmpEffectVolume() { return mTmpEffectVol; }
	void SetEffectVolume(int vol) { mEffectVol = vol; }
	void SetTmpEffectVolume(int vol) { mTmpEffectVol = vol; }
	int GetVolume() { return mSetVolume; }
	int GetTmpVolume() { return mTmpVolume; }
	void SetVolume(int vol) { mSetVolume = vol; }
	void SetTmpVolume(int vol) { mTmpVolume = vol; }
	bool GetSoundFlag() { return mSoundFlag; }
	void SetSoundFlag(bool flag) { mSoundFlag = flag; }
	bool GetBgmFlag() { return mBgmFlag; }
	void SetBgmFlag(bool flag) { mBgmFlag = flag; }
private:
	int mTmpVolume;//����OFF����ON���̃{�����[�����Ƃ��Ă���
	int mSetVolume;
	int mEffectVol;
	int mTmpEffectVol;
	bool mSoundFlag;
	bool mBgmFlag;
};

