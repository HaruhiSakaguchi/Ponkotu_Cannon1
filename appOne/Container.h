#pragma once
#include "COLOR.h"
#include "Renderer.h"
#include "UIButton.h"
#include "UIScreen.h"
#include "Title.h"
#include "Quit.h"
#include "Option.h"
#include "UIHelp1.h"
#include "UIHelp2.h"
#include "UIHelp3.h"
#include "Reconfirm.h"
#include "StageSelect.h"
#include "Pause.h"
#include "GamePlay.h"
#include "StageClear.h"
#include "GameOver.h"
#include "AllClear.h"
#include "Camera.h"
#include "Cannon.h"
#include "Items.h"
#include "PlayerBullet.h"
#include "Tama.h"
#include "TamaWeapon.h"
#include "SatelliteBullet.h"
#include "Satellite.h"
#include "SatelliteWing.h"
#include "ItemEffect.h"
#include "UIHpGauge.h"
#include "UIProgressGauge.h"
#include "UIMiniMap.h"
#include "UIScope.h"
#include "UIItemStatus.h"
#include "Map.h"
#include "UILog.h"
#include "UIPSideCharacterStatus.h"

struct Container {
	float mWidth = 0.0f, mHeight = 0.0f;
	int mFullScreenFlag = 0;
	int mInitAllSoundVolume = 0;
	int mInitEffectSoundVolume = 0;
	COLOR mBackGroundColor = COLOR(0, 0, 0);
	COLOR mNormalDisplayColor = COLOR(0,0,0);
	COLOR mPauseDisplayColor = COLOR(0, 0, 0);
	Renderer::DATA rendererData;
	Button::DATA buttonData;
	UIScreen::DATA uiScreenData;
	Title::DATA titleData;
	Quit::DATA quitData;
	Option::DATA optionData;
	UIHelp1::DATA help1Data;
	UIHelp2::DATA help2Data;
	UIHelp3::DATA help3Data;
	Reconfirm::DATA reconfirmData;
	StageSelect::DATA stageSelectData;
	Pause::DATA pauseData;
	GamePlay::DATA playData;
	StageClear::DATA stageClearData;
	GameOver::DATA overData;
	AllClear::DATA allClearData;
	Camera::DATA cameraData;
	Cannon::DATA cannonData;
	ItemObject::commonDATA itemCommonData;
	Recovery::individualDATA recoverData;
	PowerUp::individualDATA powerData;
	SpeedUp::individualDATA speedData;
	RapidFire::individualDATA rapidData;
	Barrier::individualDATA barrierData;
	RecoveryEffect::IDATA recoverEffectData;
	PowerUpEffect::IDATA powerEffectData;
	SpeedUpEffect::IDATA speedEffectData;
	RapidFireEffect::IDATA rapidEffectData;
	BarrierEffect::IDATA barrierEffectData;
	PlayerBullet::DATA pBulletData;
	PlayerBullet::IDATA pBulletIData;
	Tama::DATA tamaData;
	TamaWeapon::IDATA tWeaponIData;
	TamaWeapon::DATA tWeaponData;
	SatelliteBullet::DATA sBulletData;
	SatelliteBullet::IDATA sBulletIData;
	Satellite::DATA satelliteData;
	SatelliteWing::DATA sWingData;
	ItemEffect::CDATA itemEffectData;
	UIHpGauge::DATA hpGaugeUIData;
	UIProgressGauge::DATA progressData;
	UIMiniMap::DATA miniMapData;
	UIScope::DATA scopeData;
	UIItemStatus::DATA itemStatusData;
	Map::DATA mapData;
	UILog::DATA logData;
	UIPSideCharacterStatus::DATA psData;
	int mDeadSound = 0;
	int mCloseSound = 0;
	int mDialog1 = 0, mDialog2 = 0;
};

void SetAllData(struct Container& allData);
void loadGraphic(struct Container& allData);
void loadingSound(struct Container& allData);
void loadingImage(struct Container& allData);