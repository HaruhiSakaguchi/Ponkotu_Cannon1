#include "Container.h"
#include "graphic.h"

void SetAllData(Container& ad)
{
	//manager
	{
		ad.mWidth = 1920.0f;
		ad.mHeight = 1080.0f;
		ad.mFullScreenFlag = 1;
		ad.mInitAllSoundVolume = -450;
		ad.mInitEffectSoundVolume = -1300;
		ad.mBackGroundColor = COLOR(64, 128, 255);
		ad.mNormalDisplayColor = COLOR(0, 0, 0, 0);
		ad.mPauseDisplayColor = COLOR(0, 0, 0, 128);
	}
	//renderer
	{
		ad.rendererData.mAmbient = 0.8f;
		ad.rendererData.mPerspectiveAngle = 0.6f;
		ad.rendererData.mPerspectiveAspect = 1.7777777f;
		ad.rendererData.mPerspectiveNear = 0.1f;
		ad.rendererData.mPerspectiveFar = 100.0f;
		ad.rendererData.mCamPos = VECTOR(0.0f, 1.0f, 0.0f);
		ad.rendererData.mLookat = VECTOR(0.0f, 2.0f, 0.0f);
		ad.rendererData.mUp = VECTOR(0.0f, 1.0f, 0.0f);
		ad.rendererData.mLightPos = VECTOR(0.0f, 1.0f, 0.1f);
		ad.rendererData.mDiffuse = COLOR(1, 1, 1, 1);
	}
	//uiScreen
	{
		ad.uiScreenData.mButtonPosOffsetY = 20.0f;
	}
	//button
	{
		ad.buttonData.mButtonTextSize = 30.0f;
		ad.buttonData.mGuideTextSize = 30.0f;
		ad.buttonData.mClickSoundVolumeOffset = -450;
		ad.buttonData.mContainsSoundVolumeOffset = -100;
		ad.buttonData.mGuideRectSw = 1.0f;
		ad.buttonData.mButtonTextColor = COLOR(0, 0, 0);
		ad.buttonData.mGuideTextColor = COLOR(0, 0, 0);
		ad.buttonData.mGuideRectColor = COLOR(0, 0, 0, 0);
	}
	//title
	{
		ad.titleData.mTitle = "Ponkotu Cannon";
		ad.titleData.mTextSize = 200.0f;
		ad.titleData.mBgmSoundVolumeOffset = -450;
		ad.titleData.mBackPos = VECTOR2(ad.mWidth / 2, ad.mHeight / 2);
		ad.titleData.mTitlePos = VECTOR2(ad.mWidth / 2, ad.mHeight / 2 - 100.0f);
		ad.titleData.mButtonPos = VECTOR2(ad.mWidth / 2, ad.titleData.mTitlePos.y + 200.0f);
		ad.titleData.mPhaseFirstButtonOffsetPos = VECTOR2(ad.mWidth / 2.0f - 50.0f, -ad.mHeight + 200.0f);
		ad.titleData.mPhaseNotFirstButtonOffsetPos = VECTOR2(ad.mWidth / 2.0f - 50.0f, -ad.mHeight + 200.0f - 69.0f);
		ad.titleData.mTitleColor = COLOR(128, 128, 50);
	}
	//quit
	{
		ad.quitData.mTitle = "ゲームを終了しますか？";
		ad.quitData.mTextSize = 40.0f;
		ad.quitData.mBackPos = VECTOR2(ad.mWidth / 2, 400.0f);
		ad.quitData.mTitlePos = VECTOR2(ad.mWidth / 2, ad.quitData.mBackPos.y - 70.0f);
		ad.quitData.mButtonPos = VECTOR2(ad.mWidth / 2, ad.quitData.mTitlePos.y + 140.0f);
		ad.quitData.mTitleColor = COLOR(0, 0, 0);
	}
	//option
	{
		ad.optionData.mTitle = "オプション";
		ad.optionData.mMinVolume = -4500;
		ad.optionData.mOneVolume = 450;
		ad.optionData.mOneCameraS = 0.01f;
		ad.optionData.mMaxCameraS = 0.09f;
		ad.optionData.mTextSize = 30.0f;
		ad.optionData.mTitlePos = VECTOR2(ad.mWidth / 2, 300.0f);
		ad.optionData.mBackPos = VECTOR2(ad.optionData.mTitlePos.x, ad.optionData.mTitlePos.y + 200.0f);
		ad.optionData.mButtonPos = VECTOR2(ad.optionData.mBackPos.x, ad.optionData.mBackPos.y - 100.0f);
		ad.optionData.mBgmVolumeUpButtonOffsetPos = VECTOR2(-100.0f, 0.0f);
		ad.optionData.mBgmVolumeUpButtonGuideOffsetPos = VECTOR2(-600 + 30 / 4 + 30 / 2 + 69 * 0.75f - 69.0f / 2 - 3.525f);
		ad.optionData.mBgmVolumeDownButtonOffsetPos = VECTOR2(-200.0f, -69.0f);
		ad.optionData.mBgmVolumeDownButtonGuideOffsetPos = VECTOR2(-500 + 30 / 4 + 30 / 2 + 69 * 0.75f - 69.0f / 2 - 3.525f);
		ad.optionData.mEffectVolumeUpButtonOffsetPos = VECTOR2(200.0f, -138.0f);
		ad.optionData.mEffectVolumeUpButtonGuideOffsetPos = VECTOR2(-150.0f + 69.0f / 4.0f, 0);
		ad.optionData.mEffectVolumeDownButtonOffsetPos = VECTOR2(100.0f, -207.0f);
		ad.optionData.mEffectVolumeDownButtonGuideOffsetPos = VECTOR2(-50.0f + 69.0f / 4.0f, 0);
		ad.optionData.mSoundOnOffButtonOfsetPos = VECTOR2(0.0f, -207.0f);
		ad.optionData.mSoundOnOffButtonGuideOfsetPos = VECTOR2(100.0f, 0);
		ad.optionData.mCameraYUpButtonOffsetPos = VECTOR2(200.0f, -138.0f);
		ad.optionData.mCameraYUpButtonGuideOffsetPos = VECTOR2(-175 + 30 / 4 + 30 / 2 + 3.525f, 0);
		ad.optionData.mCameraXUpButtonOffsetPos = VECTOR2(-100.0f, -69.0f * 4);
		ad.optionData.mCameraXUpButtonGuideOffsetPos = VECTOR2(-750 + 30 / 4 + 30 / 2 + 69 * 0.75f - 15 + 3.75f - 69.0f / 4.0f, 0);
		ad.optionData.mCameraYDownButtonOffsetPos = VECTOR2(100.0f, -207.0f);
		ad.optionData.mCameraYDownButtonGuideOffsetPos = VECTOR2(-75 + 30 / 4 + 30 / 2 + 3.525f, 0);
		ad.optionData.mCameraXDownButtonOffsetPos = VECTOR2(-200, -69.0f * 5);
		ad.optionData.mCameraXDownButtonGuideOffsetPos = VECTOR2(-650 + 30 / 4 + 30 / 2 + 69 * 0.75f - 15 + 3.75f - 69.0f / 4.0f, 0);
		ad.optionData.mCloseButtonOffsetPos = VECTOR2(0, -69.0f * 5);
		ad.optionData.mVolumeTextOffsetPos = VECTOR2(ad.mWidth / 2.0f - 45.0f, ad.mHeight / 2.0f - 185.0f);
		ad.optionData.mBgmTextOffsetPos = VECTOR2(ad.mWidth / 2.0f - 173.5f, ad.mHeight / 2.0f - 175.0f);
		ad.optionData.mEffectTextOffsetPos = VECTOR2(ad.mWidth / 2.0f + 104.0f, ad.mHeight / 2.0f - 175.0f);
		ad.optionData.mBVolTextOffsetPos = VECTOR2(ad.mWidth / 2.0f + -151.0f, ad.mHeight / 2.0f - 125.0f);
		ad.optionData.mEVolTextOffsetPos = VECTOR2(ad.mWidth / 2.0f + +149.0f, ad.mHeight / 2.0f - 125.0f);
		ad.optionData.mSoundOnOffTextOffsetPos = VECTOR2(ad.mWidth / 2.0f + 150.0f, ad.mHeight / 2.0f - 59.0f);
		ad.optionData.mBgmOnOffTextOffsetPos = VECTOR2(ad.mWidth / 2.0f + 150.0f, ad.mHeight / 2.0f + 10.0f);
		ad.optionData.mCameraSTextOffsetPos = VECTOR2(ad.mWidth / 2.0f - 75.0f, ad.mHeight / 2.0f + 79.0f);
		ad.optionData.mCameraSXTextOffsetPos = VECTOR2(ad.mWidth / 2.0f - 158.5f, ad.mHeight / 2.0f + 99.0f);
		ad.optionData.mCameraSYTextOffsetPos = VECTOR2(ad.mWidth / 2.0f + 141.5f, ad.mHeight / 2.0f + 99.0f);
		ad.optionData.mCameraSXNumTextOffsetPos = VECTOR2(ad.mWidth / 2.0f + 141.5f, ad.mHeight / 2.0f + 148.0f);
		ad.optionData.mCameraSYNumTextOffsetPos = VECTOR2(ad.mWidth / 2.0f - 158.5f, ad.mHeight / 2.0f + 148.0f);
		ad.optionData.mTitleColor = COLOR(0, 0, 0);
		ad.optionData.mOnColor = COLOR(0, 0, 0);
		ad.optionData.mOffColor = COLOR(128, 128, 128);
	}
	//recconfirm
	{
		ad.reconfirmData.mTitle = "確認";
		ad.reconfirmData.mReconfirmTextSize = 30.0f;
		ad.reconfirmData.mReconfirmTextOffsetY = 100.0f;
		ad.reconfirmData.mBackPos = VECTOR2(ad.mWidth / 2, 350.0f);
		ad.reconfirmData.mTitlePos = VECTOR2(ad.mWidth / 2, ad.reconfirmData.mBackPos.y - 150.0f);
		ad.reconfirmData.mButtonPos = VECTOR2(ad.mWidth / 2, ad.reconfirmData.mTitlePos.y + 190.0f);
		ad.reconfirmData.mReconfirmTextColor = COLOR(0, 0, 0);
		ad.reconfirmData.mTitleColor = COLOR(0, 0, 0);
	}
	//stageSelect
	{
		ad.stageSelectData.mTitle = "ステージセレクト";
		ad.stageSelectData.mTextSize = 40.0f;
		ad.stageSelectData.mBackPos = VECTOR2(ad.mWidth / 2, 400.0f);
		ad.stageSelectData.mTitlePos = VECTOR2(ad.mWidth / 2, ad.stageSelectData.mBackPos.y - 150.0f);
		ad.stageSelectData.mButtonPos = VECTOR2(ad.mWidth / 2, ad.stageSelectData.mTitlePos.y + 100.0f);
		ad.stageSelectData.mStage1GuideOffsetPos = VECTOR2(-150.0f, 0.0f);
		ad.stageSelectData.mStage2GuideOffsetPos = VECTOR2(-200.0f, 0.0f);
		ad.stageSelectData.mStage3GuideOffsetPos = VECTOR2(-200.0f, 0.0f);
		ad.stageSelectData.mTitleColor = COLOR(0, 0, 0);

	}
	//help
	{
		ad.help1Data.mNextButtonPos = VECTOR2(ad.mWidth / 2.0f - 100.0f, ad.mHeight / 2.0f - 200.0f);
		ad.help1Data.mNextButtonGuidePos = VECTOR2(-360.0f, 0.0f);
		ad.help2Data.mNextButtonPos = VECTOR2(ad.mWidth / 2.0f - 100.0f, ad.mHeight / 2.0f - 200.0f);
		ad.help2Data.mNextButtonGuidePos = VECTOR2(-360.0f, 0.0f);
		ad.help2Data.mBeforeButtonPos = VECTOR2(-ad.mWidth / 2.0f + 100.0f, ad.mHeight / 2.0f - 200.0f - 69.0f);
		ad.help3Data.mBoforeButtonPos = VECTOR2(-ad.mWidth / 2.0f + 100.0f, ad.mHeight / 2.0f - 200.0f);
	}
	//pause
	{
		ad.pauseData.mTitle = "Pause";
		ad.pauseData.mPauseSoundVolumeOffset = 450;
		ad.pauseData.mTitlePos = VECTOR2(ad.mWidth / 2, ad.mHeight / 2);
		ad.pauseData.mButtonPos = VECTOR2(ad.pauseData.mTitlePos.x, ad.pauseData.mTitlePos.y + 200.0f);
		ad.pauseData.mHelpButtonPosOffset = VECTOR2(ad.mWidth / 2.0f - 50.0f, -ad.mHeight + 100.0f);
	}
	//stageClear
	{
		ad.stageClearData.mTextSize = 300.0f;
		ad.stageClearData.mTitlePos.set(ad.mWidth / 2, ad.mHeight / 2 - 100.0f);
		ad.stageClearData.mButtonPos.set(ad.mWidth / 2, ad.stageClearData.mTitlePos.y + 200.0f);
		ad.stageClearData.mClearTimeTextSize = 50.0f;
		ad.stageClearData.mTitleColor = COLOR(128, 50, 50);
	}
	//gamePlay
	{
		ad.playData.mBgm1SoundVolumeOffset = -450;
		ad.playData.mBgm2SoundVolumeOffset = -1000;
	}
	//gameOver
	{
		ad.overData.mTitle = "GameOver";
		ad.overData.mTitleTextSize = 300.0f;
		ad.overData.mTitlePosAdvSpeed = 0.04f;
		ad.overData.mMaxOffsetPosY = -150.0f;
		ad.overData.mTitlePos = VECTOR2(ad.mWidth / 2.0f, ad.mHeight / 2.0f - 100.0f);
		ad.overData.mButtonPos = VECTOR2(ad.mWidth / 2.0f, ad.overData.mTitlePos.y + 200.0f);
		ad.overData.mRetryButtonPosOffset = VECTOR2(-100.0f, 0.0f);
		ad.overData.mTitleColor = COLOR(128, 50, 50);
	}
	//allData
	{
		ad.allClearData.mButtonPos.set(ad.mWidth / 2, ad.mHeight / 2 - 100.0f + 300.0f);
	}
	//uiHpGauge
	{
		ad.hpGaugeUIData.mMaxHp = 0;
		ad.hpGaugeUIData.mHeight = 50.0f;
		ad.hpGaugeUIData.mWidth = 0.0f;
		ad.hpGaugeUIData.mSw = 5.0f;
		ad.hpGaugeUIData.mGaugeWidth = 0.0f;
		ad.hpGaugeUIData.mHpTextSize = ad.hpGaugeUIData.mHeight;
		ad.hpGaugeUIData.mNormalHpRate = 0.66f;
		ad.hpGaugeUIData.mDangerHpRate = 0.33f;
		ad.hpGaugeUIData.mHpGaugeSw = 7.5f;
		ad.hpGaugeUIData.mChangeHpSpeed = 0.05f;
		ad.hpGaugeUIData.mPos = VECTOR2(0.0f, 0.0f);
		ad.hpGaugeUIData.mNormalColor = COLOR(0, 255, 0);
		ad.hpGaugeUIData.mDyingColor = COLOR(255, 0, 0);
		ad.hpGaugeUIData.mDangarColor = COLOR(255, 255, 0);
		ad.hpGaugeUIData.mHpWindowColor = COLOR(200, 200, 200);
		ad.hpGaugeUIData.mWhite = COLOR(255, 255, 255);
		ad.hpGaugeUIData.mOverMaxHpColor = COLOR(50, 255, 255);
	}
	//uiProgress
	{
		ad.progressData.mLimmit = 0;
		ad.progressData.mChangeProgressSpeed = 0.05f;
		ad.progressData.mGaugeWidth = 500.0f;
		ad.progressData.mGaugeHeight = 25.0f;
		ad.progressData.mSw = 50.0f;
		ad.progressData.mTextSize = 50.0f;
		ad.progressData.mPos = VECTOR2(0.0f, 0.0f);
		ad.progressData.mBackColor = COLOR(255, 255, 255);
		ad.progressData.mPointColor = COLOR(0, 255, 0);
		ad.progressData.mTextColor = COLOR(255, 0, 0);
		ad.progressData.mCompleteColor = COLOR(0, 255, 0, 128);
	}
	//uiMiniMap
	{
		ad.miniMapData.mMiniMapWindowWidth = 0.0f;
		ad.miniMapData.mMiniMapWindowHeight = 0.0f;
		ad.miniMapData.mNumModels = 0;
		ad.miniMapData.mOffsetX = 350.0f;
		ad.miniMapData.mOffsetY = 525.0f;
		ad.miniMapData.mSw = 10.0f;
		ad.miniMapData.mMinPosX = 0.0f;
		ad.miniMapData.mMinPosY = 0.0f;
		ad.miniMapData.mMiniMapOffsetX = 0.0f;
		ad.miniMapData.mMiniMapOffsetY = 0.0f;
		ad.miniMapData.mMiniMapWindowLength = 538.0f;
		ad.miniMapData.mItemSw = 1.0f;
		ad.miniMapData.mWeaponSw = 5.0f;
		ad.miniMapData.mItemCircleRadius = 10.0f;
		ad.miniMapData.m3DCoordinate2DConvertRate = 7.0f;
		ad.miniMapData.mStage1OffsetX = 288.0f;
		ad.miniMapData.mStage1OffsetY = 450.0f;
		ad.miniMapData.mStage2OffsetX = 293.5f;
		ad.miniMapData.mStage2OffsetY = 440.0f;
		ad.miniMapData.mStage3OffsetX = 293.5f;
		ad.miniMapData.mMiniHpGaugeMaxWidth = 30.0f;
		ad.miniMapData.mMiniHpGaugeHeight = 5.0f;
		ad.miniMapData.mMiniMapCornerPos = VECTOR2(108.0f, 68.0f);
		ad.miniMapData.mItemStrokeColor = COLOR(255, 255, 255);
		ad.miniMapData.mArrowStrokeColor = COLOR(255, 255, 255);
		ad.miniMapData.mWindowColor = COLOR(128, 128, 128, 128);
		ad.miniMapData.mFloorColor = COLOR(40, 150, 128, 128);
	}
	//scope
	{
		ad.scopeData.mCrossSw = 5.0f;
		ad.scopeData.mCrossPosOffset = 10.0f;
		ad.scopeData.mGaugeRingRadius = 40.0f;
		ad.scopeData.mGaugeRingWeight = 10.0f;
		ad.scopeData.mStockItemWeight = 10.0f;
		ad.scopeData.mItemCylinderRadius = 75.0f;
		ad.scopeData.mMaxItenNum = 6;
		ad.scopeData.mItemNameTextSize = 30.0f;
		ad.scopeData.mItemNameWindowSw = 5.0f;
		ad.scopeData.mCylinderAngle = 0.0f;
		ad.scopeData.mStockItemOffsetX = 100.0f;
		ad.scopeData.mChangeAngleSpeed = 0.5f;
		ad.scopeData.mChangePositionSpeed = 0.05f;
		ad.scopeData.mMouseOffsetPosY = 150.0f;
		ad.scopeData.mMouseOffsetPosX = 100.0f;
		ad.scopeData.mStrings[0] = "砲弾";
		ad.scopeData.mStrings[1] = "回復";
		ad.scopeData.mStrings[2] = "パワー";
		ad.scopeData.mStrings[3] = "スピード";
		ad.scopeData.mStrings[4] = "発射間隔";
		ad.scopeData.mStrings[5] = "バリア―";
		ad.scopeData.mPos = VECTOR2(0.0f, 0.0f);
		ad.scopeData.mOffsetPos = VECTOR2(0.0f, 0.0f);
		ad.scopeData.mWhiteRingColor = COLOR(255, 255, 255);
		ad.scopeData.mGaugeRingColor = COLOR(255, 0, 0, 128);
		ad.scopeData.mColors[0] = COLOR(0, 0, 0);
		ad.scopeData.mColors[1] = COLOR(0, 255, 0);
		ad.scopeData.mColors[2] = COLOR(255, 255, 0);
		ad.scopeData.mColors[3] = COLOR(0, 0, 255);
		ad.scopeData.mColors[4] = COLOR(255, 0, 0);
		ad.scopeData.mColors[5] = COLOR(255, 0, 255);
		ad.scopeData.mCrossColor = COLOR(255, 0, 0);
		ad.scopeData.mRingOnLineColor = COLOR(0, 0, 0);
		ad.scopeData.mItemNameColor = COLOR(255, 255, 255);
		ad.scopeData.mItemNameWindowColor = COLOR(0, 0, 0, 0);
		ad.scopeData.mItemNameWindowStrokeColor = COLOR(0, 0, 0);
	}
	//itemStatus
	{
		ad.itemStatusData.mWidth = 260.0f;
		ad.itemStatusData.mHeight = 150.0f;
		ad.itemStatusData.mRadius = 50.0f;
		ad.itemStatusData.mDiamiter = 10.0f;
		ad.itemStatusData.mAngleOffset = 90.0f;
		ad.itemStatusData.mAngle = 0.0f;
		ad.itemStatusData.mRectWidth = 0.0f;
		ad.itemStatusData.mTriangleWidth = 50.0f;
		ad.itemStatusData.mTriangleHeight = 150.0f;
		ad.itemStatusData.mTriangleAngle120 = 120.0f;
		ad.itemStatusData.mTriangleAngle30 = 30.0f;
		ad.itemStatusData.mChangeTimeSpeed = 0.04f;
		ad.itemStatusData.mChangeWidthSpeed = 0.04f;
		ad.itemStatusData.mItemNameTextSize = 30.0f;
		ad.itemStatusData.mBarrierHpGaugeWidth = 100.0f;
		ad.itemStatusData.mBarrierHpGaugeOffsetPosY = 130.0f;
		ad.itemStatusData.mBarrierHpGaugeHeight = 10.0f;
		ad.itemStatusData.mWhiteTriangleOffsetPosX = 10.0f;
		ad.itemStatusData.mLineSw = 1.0f;
		ad.itemStatusData.mTriangleColor = COLOR(128, 255, 255, 128);
		ad.itemStatusData.mWhiteTriangleColor = COLOR(255, 255, 255);
		ad.itemStatusData.mTimeGaugeColor = COLOR(255, 0, 0, 128);
		ad.itemStatusData.mBarrierHpGaugeColor = COLOR(50, 50, 255);
		ad.itemStatusData.mRingOnLineColor = COLOR(0, 0, 0);
		ad.itemStatusData.mItemNameTextColor = COLOR(0, 0, 0);

	}
	//UIpSideCharaStatus
	{
		ad.psData.mWidth = 260.0f;
		ad.psData.mHeight = 150.0f;
		ad.psData.mRadius = 50.0f;
		ad.psData.mDiamiter = 10.0f;
		ad.psData.mAngleOffset = 90.0f;
		ad.psData.mAngle = 0.0f;
		ad.psData.mRectWidth = 0.0f;
		ad.psData.mTriangleWidth = 50.0f;
		ad.psData.mTriangleHeight = 150.0f;
		ad.psData.mTriangleAngle120 = 120.0f;
		ad.psData.mTriangleAngle30 = 30.0f;
		ad.psData.mChangeTimeSpeed = 0.04f;
		ad.psData.mChangeWidthSpeed = 0.04f;
		ad.psData.mItemNameTextSize = 30.0f;
		ad.psData.mBarrierHpGaugeWidth = 100.0f;
		ad.psData.mBarrierHpGaugeOffsetPosY = 130.0f;
		ad.psData.mBarrierHpGaugeHeight = 10.0f;
		ad.psData.mWhiteTriangleOffsetPosX = 10.0f;
		ad.psData.mLineSw = 1.0f;
		ad.psData.mTriangleColor = COLOR(128, 255, 255, 128);
		ad.psData.mWhiteTriangleColor = COLOR(255, 255, 255);
		ad.psData.mTimeGaugeColor = COLOR(255, 0, 0, 128);
		ad.psData.mBarrierHpGaugeColor = COLOR(50, 50, 255);
		ad.psData.mRingOnLineColor = COLOR(0, 0, 0);
		ad.psData.mItemNameTextColor = COLOR(0, 0, 0);

	}
	//camera
	{
		ad.cameraData.mDistanceFromLookatPos = 1.0f;
		ad.cameraData.mMaxDistanceFromLookatPos = 1.5f;
		ad.cameraData.mRotSpeedX = 0.04f;
		ad.cameraData.mRotSpeedY = 0.02f;
		ad.cameraData.mMaxRotX = 0.7f;
		ad.cameraData.mLookatPos = 0.0f;
		ad.cameraData.mOffsetPosY = 1.250f;
		ad.cameraData.mOffsetPosZ = 10.0f;
		ad.cameraData.mChangePosSpeed = 0.5f;
		ad.cameraData.mCorrectPos = 1.0f;
		ad.cameraData.mCenterPos = VECTOR2(ad.mWidth / 2, ad.mHeight / 2);
		ad.cameraData.mUpVec = VECTOR(0.0f, 1.0f, 0.0f);
	}
	//cannon
	{
		ad.cannonData.mAdvSpeed = 0.05f * 10.0f;
		ad.cannonData.mAmbient = 0.3f;
		ad.cannonData.mGravity = -24.0f;
		ad.cannonData.mJumpVelocity = 0.0f;
		ad.cannonData.mInterval = 0.5f;
		ad.cannonData.mJumpFlag = 1;
		ad.cannonData.mTimer = 0.0f;
		ad.cannonData.mRange = 20.0f;
		ad.cannonData.mMaxHp = 7;
		ad.cannonData.mDamage = 1;//与ダメ
		ad.cannonData.mRDamage = 1;//被ダメ
		ad.cannonData.mMaxDamageInterval = 0.16f;
		ad.cannonData.mCannonCapsulHeight = 0.50f;
		ad.cannonData.mCannonCapsulRadius = 0.75f;
		ad.cannonData.mWheelCorners = 36;
		ad.cannonData.mWheelFrontZOffset = 0.0f;
		ad.cannonData.mWheelBackZOffset = -0.2f;
		ad.cannonData.mWheelAngleOffset = 1.57f;
		ad.cannonData.mLaunchTimeDecreaseAdvSpeed = 300.0f;
		ad.cannonData.mLaunchBodyScale = 1.5f;
		ad.cannonData.mNormalBodyScale = 1.0f;
		ad.cannonData.mJumpSoundVolumeOffset = -800;
		ad.cannonData.mFallSoundVolumeOffset = -100;
		ad.cannonData.mItemStockMaxSize = 6;
		ad.cannonData.mWheelAngleZ = 0.0f;
		ad.cannonData.mWheelAngleX = 0.0f;
		ad.cannonData.mChangeScaleSpeed = 0.4f;
		ad.cannonData.mWheelOffsetPos = VECTOR(-0.5f, 0.0f, 0.0f);
		ad.cannonData.mBarrelDir = VECTOR(0.0f, 0.0f, -1.0f);
		ad.cannonData.mInitPos = VECTOR(0.0f, 10.0f, 1.0f);
		ad.cannonData.mCapsulOffset = VECTOR(0, 0.5f, 0);
		ad.cannonData.mBodyOffsetPos = VECTOR(0.0f, 0.3f, 0.0f);
		ad.cannonData.mImageColor = COLOR(0, 0, 255);
		ad.cannonData.mDisplayColor = COLOR(0, 0, 0, 0);
		ad.cannonData.mGurdColor = COLOR(128, 128, 255, 128);
	}
	//itemすべてに共通するデータ
	{
		ad.itemCommonData.mJumpVelocity = 0.0f;
		ad.itemCommonData.mGravity = -5.0f;
		ad.itemCommonData.mJumpFlag = 1;
		ad.itemCommonData.mAdvSpeed = 0.5f;
		ad.itemCommonData.mLifeTime = 0.0f;
		ad.itemCommonData.mMaxLifeTime = 30.0f;
		ad.itemCommonData.mRadius = 0.1f;
		ad.itemCommonData.mDropSoundVolumeOffset = -450;
		ad.itemCommonData.mCapsulOffset = VECTOR(0.0f, 1.0f, 0.0f);
	}
	//recover
	{
		ad.recoverData.mNum = 1;
		ad.recoverData.mColor = COLOR(0, 255, 0);
		ad.recoverData.mName = "Recovery";
	}
	//power
	{
		ad.powerData.mNum = 2;
		ad.powerData.mColor = COLOR(255, 255, 0);
		ad.powerData.mName = "PowerUp";

	}
	//speed
	{
		ad.speedData.mNum = 3;
		ad.speedData.mColor = COLOR(0, 0, 255);
		ad.speedData.mName = "SpeedUp";

	}
	//rapid
	{
		ad.rapidData.mNum = 4;
		ad.rapidData.mColor = COLOR(255, 0, 0);
		ad.rapidData.mName = "RapidFire";
	}
	//barrier
	{
		ad.barrierData.mNum = 5;
		ad.barrierData.mColor = COLOR(255, 0, 255);
		ad.barrierData.mName = "Barrier";
	}
	//itemComponentに共通するデータ
	{
		ad.itemCompoData.mUIPosY = 0.0f;
		ad.itemCompoData.mUIOffsetPosY = 150.0f;
		ad.itemCompoData.mUIPosAdvSpeed = -100.0f;
		ad.itemCompoData.mUIMinPosX = 1600.0f;
		ad.itemCompoData.mUIPosChangeSpeed = 0.5f;
		ad.itemCompoData.mNumber = 0;
		ad.itemCompoData.mKeepTime = 0.0f;
	}
	//recoverCompo
	{
		ad.recoverCompoData.mName = "回復";
		ad.recoverCompoData.mLevel = 1;
		ad.recoverCompoData.mMaxLevel = 1;
		ad.recoverCompoData.mTime = 0.0f;
		ad.recoverCompoData.mInterval = 0.0f;
		ad.recoverCompoData.mMaxHp = 0;
		ad.recoverCompoData.mColor = COLOR(0, 255, 0);
	}
	//powerCompo
	{
		ad.powerCompoData.mName = "威力増加";
		ad.powerCompoData.mLevel = 1;
		ad.powerCompoData.mMaxLevel = 4;
		ad.powerCompoData.mTime = 0.0f;
		ad.powerCompoData.mInterval = 100.0f;
		ad.powerCompoData.mMaxHp = 1;
		ad.powerCompoData.mColor = COLOR(255, 255, 0);
	}
	//speedCompo
	{
		ad.speedCompoData.mName = "移動速度増加";
		ad.speedCompoData.mLevel = 1;
		ad.speedCompoData.mMaxLevel = 5;
		ad.speedCompoData.mTime = 0.0f;
		ad.speedCompoData.mInterval = 100.0f;
		ad.speedCompoData.mMaxHp = 1;
		ad.speedCompoData.mColor = COLOR(0, 0, 255);
	}
	//rapidCompo
	{
		ad.rapidCompoData.mName = "発射間隔短縮";
		ad.rapidCompoData.mLevel = 1;
		ad.rapidCompoData.mMaxLevel = 3;
		ad.rapidCompoData.mTime = 0.0f;
		ad.rapidCompoData.mInterval = 100.0f;
		ad.rapidCompoData.mMaxHp = 1;
		ad.rapidCompoData.mColor = COLOR(255, 0, 0);
	}
	//barrierCompo
	{
		ad.barrierCompoData.mName = "ダメージ無効";
		ad.barrierCompoData.mLevel = 1;
		ad.barrierCompoData.mMaxLevel = 5;
		ad.barrierCompoData.mTime = 0.0f;
		ad.barrierCompoData.mInterval = 100.0f;
		ad.barrierCompoData.mMaxHp = 3;
		ad.barrierCompoData.mColor = COLOR(255, 0, 255);
	}
	//playerBullet
	{
		ad.pBulletData.mRadius = 0.2f;
		ad.pBulletData.mLength = 1.1f;
		ad.pBulletData.mHeight = 0.0f;
		ad.pBulletData.mAdvSpeed = 15.5f;
		ad.pBulletData.mMaxLifeTime = 10.0f;
		ad.pBulletData.mLifeTimeDecreaseSpeed = 5.0f;
		ad.pBulletData.mAmbient = 0.5f;
		ad.pBulletData.mJumpFlag = 0;
		ad.pBulletData.mGravity = 0.0f;
		ad.pBulletData.mJumpVelocity = 0.0f;
		ad.pBulletData.mSize = ad.pBulletData.mRadius * 2;
		ad.pBulletData.mDamage = 1;
		ad.pBulletIData.mLaunchSoundVolumeOffset = -100;
		ad.pBulletIData.mImpactSoundVolumeOffset = 0;
		ad.pBulletIData.mItemStockSoundVolumeOffset = 0;
		ad.pBulletData.mDir = VECTOR(0.0f, 0.0f, 0.0f);
		ad.pBulletData.mImageColor = COLOR(0, 0, 255);
	}
	//enemy
	{
		ad.tamaData.mOffsetPosY = 0.5f;
		ad.tamaData.mAnimSpeed = 0.017f;
		ad.tamaData.mJumpVelocity = 1.0f;
		ad.tamaData.mGravity = -19.6f;
		ad.tamaData.mJumpFlag = 1;
		ad.tamaData.mAdvSpeed = 0.5f;
		ad.tamaData.mAmbient = 0.4f;
		ad.tamaData.mMaxHp = 7;
		ad.tamaData.mAnimAngle = 0.0f;
		ad.tamaData.mWeaponCnt = 0;
		ad.tamaData.mElapsedTime = 0.0f;
		ad.tamaData.mInterval = 80.0f;
		ad.tamaData.mRadius = 0.4f;
		ad.tamaData.mHeight = 0.0f;
		ad.tamaData.mMaxDamageInterval = 0.016f * 3;
		ad.tamaData.mBlackEyeScale = 0.01f;
		ad.tamaData.mHeadScale = ad.tamaData.mRadius * 2;
		ad.tamaData.mMaxOffsetZ = 0.5f;
		ad.tamaData.mMaxScale = ad.tamaData.mMaxOffsetZ * 2;
		ad.tamaData.mChangeScaleSpeed = 0.4f;
		ad.tamaData.mOffsetPos = VECTOR(0.0f, ad.tamaData.mRadius, 0.0f);
		ad.tamaData.mBlackEyeOffsetPos = VECTOR(0.0f, 0.0f, 0.375f);
		ad.tamaData.mCapsulOffset = VECTOR(0.0f, 1.0f, 0.0f);
		ad.tamaData.mHpGaugeOffset = VECTOR(0.0f, 0.25f, 0.0f);
		ad.tamaData.mImageColor = COLOR(255.0f, 0.0f, 0.0f);
	}
	//enemyWeapon
	{
		ad.tWeaponData.mAmbient = 1.0f;
		ad.tWeaponData.mHeight = 0.0f;
		ad.tWeaponData.mRadius = 0.2f;
		ad.tWeaponData.mAdvSpeed = 10.0f;
		ad.tWeaponData.mLifeTime = 0.0f;
		ad.tWeaponData.mMaxLifeTime = 10.0f;
		ad.tWeaponData.mLifeTimeDecreaseSpeed = 5.0f;
		ad.tWeaponData.mSize = ad.tamaData.mRadius;
		ad.tWeaponData.mAmbient = 1.0f;
		ad.tWeaponData.mJumpFlag = 0;
		ad.tWeaponData.mGravity = 0.0f;
		ad.tWeaponData.mJumpVelocity = 0.0f;
		ad.tWeaponIData.mTime = 1.0f;
		ad.tWeaponData.mOffsetPos = VECTOR(0.0f, 0.0f, 0.0f);
		ad.tWeaponData.mImageColor = COLOR(255, 0, 0);
	}
	//satellite
	{
		ad.satelliteData.mElapsedTime = 0.0f;
		ad.satelliteData.mHeight = 0.75f;
		ad.satelliteData.mRadius = 0.6f;
		ad.satelliteData.mInterval = 0.250f;
		ad.satelliteData.mAnimAngle = 0.0f;
		ad.satelliteData.mMoveRange = 2;
		ad.satelliteData.mAdvSpeed = 0.01f;
		ad.satelliteData.mAmbient = 0.4f;
		ad.satelliteData.mAnimSpeed = 0.017f;
		ad.satelliteData.mMaxHp = 5;
		ad.satelliteData.mMaxHp2 = 8;
		ad.satelliteData.mAngleForPos = 0.05f;
		ad.satelliteData.mMaxDamageInterval = 0.016f * 3.0f;
		ad.satelliteData.mGravity = -4.9f;
		ad.satelliteData.mBodyNumCorners = 36;
		ad.satelliteData.mBodyFrontZ = 0.0f;
		ad.satelliteData.mBodyBackZ = -1.5f;
		ad.satelliteData.mMaxRange = 10.0f;
		ad.satelliteData.mBulletOffsetPos = VECTOR(0.0f, -0.75f, 0.0f);
		ad.satelliteData.mCapsulOffset = VECTOR(0.0f, 0.5f, 0.0f);
		ad.satelliteData.mHpGaugeOffset = VECTOR(0.0f, -2.0f, 0.0f);
		ad.satelliteData.mOffsetPos = VECTOR(0.0f, 0.0f, 0.75f);
		ad.satelliteData.mTargetOffsetPos = VECTOR(-7.0f, 7.0f, 7.0f);
		ad.satelliteData.mTargetPos = VECTOR(0.0f, 0.0f, 0.0f);
		ad.satelliteData.mDeadPoint = VECTOR(0.0f, 0.0f, 0.0f);
		ad.satelliteData.mImageColor = COLOR(255, 0, 0);
	}
	//satelliteBullet
	{
		ad.sBulletData.mAdvSpeed = 10.0f;
		ad.sBulletData.mHeight = 0.0f;
		ad.sBulletData.mRadius = 0.05f;
		ad.sBulletData.mLifeTime = 0.0f;
		ad.sBulletData.mMaxLifeTime = 10.0f;
		ad.sBulletData.mLifeTimeDecreaseSpeed = 5.0f;
		ad.sBulletData.mAmbient = 1.0f;
		ad.sBulletData.mJumpFlag = 0;
		ad.sBulletData.mGravity = 0.0f;
		ad.sBulletData.mJumpVelocity = 0.0f;
		ad.sBulletData.mSize = ad.sBulletData.mRadius * 2;
		ad.sBulletData.mDir = VECTOR(0, 0, 0);
		ad.sBulletData.mImageColor = COLOR(255, 0, 0);
	}
	//satelliteWing
	{
		ad.sWingData.mGravity = -9.8f;
		ad.sWingData.mWingAmbient = 0.8f;
		ad.sWingData.mRadius = 0.5f;
		ad.sWingData.mHeight = 0.0f;
		ad.sWingData.mAdvSpeed = 0.5f;
		ad.sWingData.mMaxOffsetZ = 5.0f;
		ad.sWingData.mId = 0;
		ad.sWingData.mChangePosSpeed = 0.05f;
		ad.sWingData.mChangeOffsetPosSpeed = 0.4f;
		ad.sWingData.mWingScale1 = VECTOR(0.7f, 1.1f, 1.0f);
		ad.sWingData.mWingScale2 = VECTOR(1.1f, 1.1f, 1.0f);
		ad.sWingData.mS0IdW0IdOffsetPos = VECTOR(0.0f, 0.0f, 0.8f);
		ad.sWingData.mS0IdW1IdOffsetPos = VECTOR(0.85f, 0.0f, 0.0f);
		ad.sWingData.mS0IdW2IdOffsetPos = VECTOR(-0.85f, 0.0f, 0.0f);
		ad.sWingData.mS0IdW3IdOffsetPos = VECTOR(0.0f, 0.0f, -0.8f);
		ad.sWingData.mS1IdOffsetPos = VECTOR(0.0f, 0.750f, -1.0f);
		ad.sWingData.mS0IdAttackOffsetPos = VECTOR(0.0f, 0.0f, -1.0f);
		ad.sWingData.mOffsetPos = VECTOR(0.0f, 0.0f, 0.0f);
		ad.sWingData.mImageColor = COLOR(255, 0, 0);
	}
	//map
	{
		ad.mapData.CollisionText = nullptr;
		ad.mapData.mStageMaxX = 0;
		ad.mapData.mStageMinX = 0;
		ad.mapData.mStageMaxZ = 0;
		ad.mapData.mStageMinZ = 0;
	}
	{
		ad.logData.mLogWindowColor = COLOR(128, 128, 128, 128);
		ad.logData.mLogTextColor = COLOR(255, 255, 255);
		ad.logData.mPos = VECTOR2(1920 - (400), 1080 - (200 + 50));
		ad.logData.mWidth = 380.0f;
		ad.logData.mHeight = 200.0f;
		ad.logData.mTextSize = 20.0f;
	}
}
void loadGraphic(Container& ad)
{
	loadingSound(ad);
	loadingImage(ad);
}

void loadingSound(struct Container& ad)
{
	//button
	{
		ad.buttonData.mClickSound = loadSound("Assets\\sound\\Button\\Click.wav");
		ad.buttonData.mContainsSound = loadSound("Assets\\sound\\Button\\Contains.wav");
	}
	//title
	{
		ad.titleData.mBgm = loadSound("Assets\\sound\\Title\\Title.wav");
	}
	//pause
	{
		ad.pauseData.mSound = loadSound("Assets\\sound\\Pause\\Pause.wav");
	}
	//gamePlay
	{
		ad.playData.mBgm1 = loadSound("Assets\\sound\\Play\\BGM\\Play1.wav");
		ad.playData.mBgm2 = loadSound("Assets\\sound\\Play\\BGM\\Play2.wav");
	}
	//stageClear
	{
		ad.stageClearData.mSound = loadSound("Assets\\sound\\StageClear\\StageClear1.wav");
		ad.stageClearData.mSound2 = loadSound("Assets\\sound\\StageClear\\StageClear2.wav");
	}
	//gameOver
	{
		ad.overData.mSound = loadSound("Assets\\sound\\GameOver\\GameOver.wav");
	}
	//allClear
	{
		ad.allClearData.mBgm = loadSound("Assets\\sound\\AllClear\\AllClear.wav");
	}
	//アクター共通の音データ
	{
		ad.mDeadSound = loadSound("Assets\\sound\\Play\\AllActor\\Dead.wav");
	}
	//cannon
	{
		ad.cannonData.mJumpSound = loadSound("Assets\\sound\\Play\\Cannon\\Jump.wav");
		ad.cannonData.mDamageSound = loadSound("Assets\\sound\\Play\\Cannon\\Damage.wav");
		ad.cannonData.mGurdSound = loadSound("Assets\\sound\\Play\\Cannon\\Gurd.wav");
		ad.cannonData.mCyakuchi = loadSound("Assets\\sound\\Play\\Cannon\\Landing.wav");
		ad.cannonData.mSlide = loadSound("Assets\\sound\\Play\\Cannon\\Slide.wav");
		ad.cannonData.mOutOfBullets = loadSound("Assets\\sound\\Play\\Cannon\\OutOfBullets.wav");
		ad.cannonData.mFallSound = loadSound("Assets\\sound\\Play\\AllActor\\Fall.wav");
	}
	//item共通のデータ
	{
		ad.itemCommonData.mDropSound = loadSound("Assets\\sound\\Play\\Item\\Drop.wav");
	}
	//item固有のデータ
	{
		ad.recoverData.mSound1 = loadSound("Assets\\sound\\Play\\Item\\Recover.wav");
		ad.recoverData.mSound2 = loadSound("Assets\\sound\\Play\\Item\\Stock.wav");
		ad.powerData.mSound1 = loadSound("Assets\\sound\\Play\\Item\\Power.wav");
		ad.powerData.mSound2 = -1;
		ad.speedData.mSound1 = loadSound("Assets\\sound\\Play\\Item\\Speed.wav");
		ad.speedData.mSound2 = -1;
		ad.rapidData.mSound1 = loadSound("Assets\\sound\\Play\\Item\\Rapid.wav");
		ad.powerData.mSound2 = -1;
		ad.barrierData.mSound1 = loadSound("Assets\\sound\\Play\\Item\\Barrier.wav");
		ad.barrierData.mSound2 = -1;
	}

	{
		ad.mCloseSound = loadSound("Assets\\sound\\Play\\Item\\Close.wav");
	}
	//pBullet
	{
		ad.pBulletIData.mLaunchSound = loadSound("Assets\\sound\\Play\\Cannon\\Bullet_Launch.wav");
		ad.pBulletIData.mImpactSound = loadSound("Assets\\sound\\Play\\Cannon\\Bullet_Hit.wav");
		ad.pBulletIData.mItemStockSound = loadSound("Assets\\sound\\Play\\Item\\Stock.wav");
	}
	//tama
	{
		ad.tamaData.mDushSound = loadSound("Assets\\sound\\Play\\Tama\\Dush.wav");
	}
	//tamaWeapon
	{
		ad.tWeaponIData.mAttackSound = loadSound("Assets\\sound\\Play\\Tama\\Weapon.wav");
	}
	//sBullet
	{
		ad.sBulletIData.mLaunchSound = loadSound("Assets\\sound\\Play\\Satellite\\Bullet_Launch.wav");
		ad.sBulletIData.mImpactSound = loadSound("Assets\\sound\\Play\\Satellite\\Bullet_Hit.wav");
	}
	//sWing
	{
		ad.sWingData.mGurdSound = loadSound("Assets\\sound\\Play\\Cannon\\Gurd.wav");
		ad.sWingData.mClashSound = loadSound("Assets\\sound\\Play\\Satellite\\Wing_Clash.wav");
	}
}

void loadingImage(struct Container& ad)
{
	//uiScreen
	{
		ad.uiScreenData.mOffButton1 = loadImage("Assets/Image/Button/ButtonBlue.png");
		ad.uiScreenData.mOnButton1 = loadImage("Assets/Image/Button/ButtonYellow.png");
		ad.uiScreenData.mOffButton2 = loadImage("Assets/Image/Button/ButtonBlueSquare.png");
		ad.uiScreenData.mOnButton2 = loadImage("Assets/Image/Button/ButtonYellowSquare.png");
	}
	//title
	{
		ad.titleData.mBackImg = loadImage("assets\\Image\\Title\\Title.png");
	}
	//uiDialog
	{
		ad.mDialog1 = loadImage("Assets\\Image\\Dialog\\Dialog1.png");
		ad.mDialog2 = loadImage("Assets\\Image\\Dialog\\Dialog2.png");
	}
	//help1
	{
		ad.help1Data.mImage = loadImage("Assets\\Image\\Help\\Help1.png");
	}
	//help2
	{
		ad.help2Data.mImage = loadImage("Assets\\Image\\Help\\Help2.png");
	}
	//help3
	{
		ad.help3Data.mImage = loadImage("Assets\\Image\\Help\\Help3.png");
	}
	//allclear
	{
		ad.allClearData.mImg = loadImage("Assets\\Image\\AllClear\\AllClear.png");
	}
}