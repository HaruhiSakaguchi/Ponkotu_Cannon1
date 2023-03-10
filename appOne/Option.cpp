#include "Option.h"
#include "Game.h"

Option::Option(class Game* game)
	:UIState(game)
	, mVolume(0)
	, mEffectVolume(0)
{
	Data = mGame->GetAllData()->optionData;
	mTitle = Data.mTitle;
	mTitlePos = Data.mTitlePos;
	mTitleColor = Data.mTitleColor;
	mBackImg = mGame->GetAllData()->mDialog1;
	mBackPos = Data.mBackPos;
	mButtonPos = Data.mButtonPos;
	mVolume = mGame->GetVolume();
	mEffectVolume = mGame->GetEffectVolume();

	AddButton("+",
		[this]()
		{
			if (mGame->GetSoundFlag())
			{
				//0から-4500までで音量調節
				//最小値を下回るともっと小さい値を入れて完全に音が聞こえないようにする
				if (mVolume <= Data.mMinVolume)
				{
					mVolume = Data.mMinVolume * 2;
				}
				if (mVolume == Data.mMinVolume * 2)
				{
					mVolume = Data.mMinVolume;
				}
				mVolume += Data.mOneVolume;
				if (mVolume > 0)
				{
					mVolume = 0;
				}
				mGame->SetVolume(mVolume);
			}
		}
		,
			2,
			"BGMの音量を上げます"
			, Data.mBgmVolumeUpButtonOffsetPos
			, Data.mBgmVolumeUpButtonGuideOffsetPos
			);


	AddButton("-",
		[this]()
		{
			if (mGame->GetSoundFlag())
			{
				mVolume -= Data.mOneVolume;
				if (mVolume <= Data.mMinVolume)
				{
					mVolume = Data.mMinVolume * 2;
				}
				mGame->SetVolume(mVolume);
			}
		},
		2,
			"BGMの音量を下げます"
			, Data.mBgmVolumeDownButtonOffsetPos
			, Data.mBgmVolumeDownButtonGuideOffsetPos
			);

	AddButton("+",
		[this]()
		{
			if (mGame->GetSoundFlag())
			{
				//0から-4500までで音量調節
				if (mEffectVolume <= Data.mMinVolume)
				{
					mEffectVolume = Data.mMinVolume * 2;
				}
				if (mEffectVolume == Data.mMinVolume * 2)
				{
					mEffectVolume = Data.mMinVolume;
				}
				mEffectVolume += Data.mOneVolume;
				if (mEffectVolume > 0)
				{
					mEffectVolume = 0;
				}
				mGame->SetEffectVolume(mEffectVolume);
			}
		}
		,
			2,
			"効果音の音量を上げます"
			, Data.mEffectVolumeUpButtonOffsetPos
			, Data.mEffectVolumeUpButtonGuideOffsetPos
			);


	AddButton("-",
		[this]()
		{
			if (mGame->GetSoundFlag())
			{
				mEffectVolume -= Data.mOneVolume;
				if (mEffectVolume <= Data.mMinVolume)
				{
					mEffectVolume = Data.mMinVolume * 2;
				}
				mGame->SetEffectVolume(mEffectVolume);
			}
		},
		2,
			"効果音の音量を下げます"
			, Data.mEffectVolumeDownButtonOffsetPos
			, Data.mEffectVolumeDownButtonGuideOffsetPos
			);

	AddButton("SOUND ON/OFF",
		[this]()
		{
			if (mGame->GetSoundFlag())
			{
				if (mVolume <= Data.mMinVolume * 2)
				{
					mVolume = Data.mMinVolume / 2;
					mEffectVolume = Data.mMinVolume / 2;
					mGame->SetVolume(mVolume);
					mGame->SetEffectVolume(mEffectVolume);
				}
				mGame->SetTmpVolume(mGame->GetVolume());
				mGame->SetTmpEffectVolume(mGame->GetEffectVolume());
				mVolume = Data.mMinVolume * 2;
				mEffectVolume = Data.mMinVolume * 2;
				mGame->SetVolume(mVolume);
				mGame->SetEffectVolume(mEffectVolume);
				mGame->SetSoundFlag(false);
			}
			else
			{
				mVolume = mGame->GetTmpVolume();
				mEffectVolume = mGame->GetTmpEffectVolume();
				mGame->SetVolume(mVolume);
				mGame->SetEffectVolume(mEffectVolume);
				mGame->SetSoundFlag(true);
			}

		},
		1,
			"すべての音声のON/OFFを切り替えます"
			, Data.mSoundOnOffButtonOfsetPos
			, Data.mSoundOnOffButtonGuideOfsetPos
			);

	AddButton("BGM ON/OFF",
		[this]()
		{
			if (mGame->GetBgmFlag())
			{
				mGame->SetBgmFlag(false);
			}
			else
			{
				mGame->SetBgmFlag(true);
			}
		},
		1,
			"BGMのON/OFFを切り替えます"
			, Data.mSoundOnOffButtonOfsetPos
			, Data.mSoundOnOffButtonGuideOfsetPos
			);

	AddButton("+",
		[this]()
		{
			if (mGame->GetCameraSX() < Data.mMaxCameraS)
			{
				float cameraX = mGame->GetCameraSX();
				cameraX += Data.mOneCameraS;
				mGame->SetCameraSX(cameraX);
			}
		}
		,
			2,
			"カメラY軸の感度を上げます"
			, Data.mCameraYUpButtonOffsetPos
			, Data.mCameraYUpButtonGuideOffsetPos

			);


	AddButton("-",
		[this]()
		{
			if (mGame->GetCameraSX() > Data.mOneCameraS)
			{
				float cameraX = mGame->GetCameraSX();
				cameraX -= Data.mOneCameraS;
				mGame->SetCameraSX(cameraX);
			}
		},
		2,
			"カメラY軸の感度を下げます"
			, Data.mCameraYDownButtonOffsetPos
			, Data.mCameraYDownButtonGuideOffsetPos

			);

	AddButton("+",
		[this]()
		{
			if (mGame->GetCameraSY() < Data.mMaxCameraS)
			{
				float cameraY = mGame->GetCameraSY();
				cameraY += Data.mOneCameraS;
				mGame->SetCameraSY(cameraY);
			}
		}
		,
			2,
			"カメラX軸の感度を上げます"
			, Data.mCameraXUpButtonOffsetPos
			, Data.mCameraXUpButtonGuideOffsetPos

			);


	AddButton("-",
		[this]()
		{
			if (mGame->GetCameraSY() > Data.mOneCameraS)
			{
				float cameraY = mGame->GetCameraSY();
				cameraY -= Data.mOneCameraS;
				mGame->SetCameraSY(cameraY);
			}
		},
		2,
			"カメラX軸の感度を下げます"
			, Data.mCameraXDownButtonOffsetPos
			, Data.mCameraXDownButtonGuideOffsetPos

			);

	AddButton("閉じる",
		[this]()
		{
			CloseMe();
		},
		1,
			"オプションを閉じます"
			, Data.mCloseButtonOffsetPos
			);
}


void Option::draw()
{

	int bVol = 0;
	int eVol = 0;
	COLOR color = Data.mOnColor;
	if (mGame->GetSoundFlag())
	{
		bVol = mGame->GetVolume() / Data.mOneVolume + 10;
		if (mGame->GetVolume() == Data.mMinVolume * 2)
		{
			bVol = 0;
		}
		eVol = mGame->GetEffectVolume() / Data.mOneVolume + 10;
		if (mGame->GetEffectVolume() == Data.mMinVolume * 2)
		{
			eVol = 0;
		}
	}
	else
	{
		bVol = mGame->GetTmpVolume() / Data.mOneVolume + 10;
		if (mGame->GetTmpVolume() == Data.mMinVolume * 2)
		{
			bVol = 0;
		}
		eVol = mGame->GetTmpEffectVolume() / Data.mOneVolume + 10;
		if (mGame->GetTmpEffectVolume() == Data.mMinVolume * 2)
		{
			eVol = 0;
		}
		color = Data.mOffColor;
	}

	float bOffset = 0;
	float eOffset = 0;
	//Volumeの値が2桁なら値を表示する場所をずらす
	if (bVol < 10)
	{
		bOffset = -Data.mTextSize / 4.0f;
	}
	else
	{
		bOffset = -Data.mTextSize / 2.0f;
	}

	if (eVol < 10)
	{
		eOffset = -Data.mTextSize / 4.0f;
	}
	else
	{
		eOffset = -Data.mTextSize / 2.0f;
	}

	textSize(Data.mTextSize);
	fill(color);
	text("volume", Data.mVolumeTextOffsetPos.x, Data.mVolumeTextOffsetPos.y);
	text("BGM", Data.mBgmTextOffsetPos.x, Data.mBgmTextOffsetPos.y);
	text("効果音", Data.mEffectTextOffsetPos.x, Data.mEffectTextOffsetPos.y);
	text(bVol, bOffset + Data.mBVolTextOffsetPos.x, Data.mBVolTextOffsetPos.y);
	text(eVol, eOffset + Data.mEVolTextOffsetPos.x, Data.mEVolTextOffsetPos.y);

	const char* onOff;
	if (mGame->GetSoundFlag())
	{
		onOff = "SOUND : ON";
	}
	else
	{
		onOff = "SOUND : OFF";
	}

	text(onOff, Data.mSoundOnOffTextOffsetPos.x, Data.mSoundOnOffTextOffsetPos.y);

	if (mGame->GetBgmFlag())
	{
		onOff = "BGM : ON";
	}
	else
	{
		onOff = "BGM : OFF";
	}

	text(onOff, Data.mBgmOnOffTextOffsetPos.x, Data.mBgmOnOffTextOffsetPos.y);

	fill(Data.mOnColor);
	text("カメラ感度", Data.mCameraSTextOffsetPos.x, Data.mCameraSTextOffsetPos.y);
	text("X", Data.mCameraSXTextOffsetPos.x, Data.mCameraSXTextOffsetPos.y);
	text("Y", Data.mCameraSYTextOffsetPos.x, Data.mCameraSYTextOffsetPos.y);

	//100倍して小数点以下切り捨て
	int sx = (int)roundf(mGame->GetCameraSX() * 100.0f);
	int sy = (int)roundf(mGame->GetCameraSY() * 100.0f);

	float offsetXX = 0.0f;
	float offsetYX = 0.0f;
	if (sx == 10)
	{
		offsetXX = -Data.mTextSize / 4.0f;
	}
	if (sy == 10)
	{
		offsetYX = -Data.mTextSize / 4.0f;
	}

	text(sx,Data.mCameraSXNumTextOffsetPos.x + offsetXX, Data.mCameraSXNumTextOffsetPos.y);
	text(sy,Data.mCameraSYNumTextOffsetPos.x + offsetYX, Data.mCameraSYNumTextOffsetPos.y);

}