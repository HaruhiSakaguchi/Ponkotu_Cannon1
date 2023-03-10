#include "Items.h"
#include "Game.h"
#include "TreeMeshComponent.h"
#include "UILog.h"
#include <sstream>

Recovery::Recovery(class Game* game)
	:Item(game)
{
	iData = GetGame()->GetAllData()->recoverData;
	SetImageColor(iData.mColor);
	SetUp();
	mTc = new TreeMeshComponent(this);
	mTc->SetOffsetPos(GetCapsulOffset());
	mTc->SetTree("Recover");
	mTc->SetDamageTree("RecoverTran");
}

bool Recovery::update()
{
	class Cannon* c = static_cast<class Cannon*>(GetGame()->GetCannon());
	if (GetGame()->GetCannon()->GetHp() < c->GetMaxHp())
	{
		new RecoveryCompo(c);
		setVolume(iData.mSound1, GetGame()->GetEffectVolume() + mRecoverySoundVolumeOffset);
		playSound(iData.mSound1);
		std::ostringstream oss;
		oss << GetName() << "アイテムを使用。";
		GetGame()->GetStage()->GetLog()->AddText(oss.str());
	}
	else
	{
		c->AddItemNum(iData.mNum);
		setVolume(iData.mSound2, GetGame()->GetEffectVolume());
		playSound(iData.mSound2);
		std::ostringstream oss;
		oss << GetName() << "アイテムをストック。";
		GetGame()->GetStage()->GetLog()->AddText(oss.str());
	}
	return true;
}

RecoveryCompo::RecoveryCompo(class Cannon* owner)
	:ItemComponent(owner)
{
	Data = owner->GetGame()->GetAllData()->recoverCompoData;
	Data.mHp = Data.mMaxHp;
	if (owner->GetHp() < owner->GetMaxHp())
	{
		owner->SetHp(owner->GetHp() + mRecoveryHp);
	}
}

SpeedUp::SpeedUp(class Game* game)
	:Item(game)
{
	iData = GetGame()->GetAllData()->speedData;
	SetImageColor(iData.mColor);
	SetUp();
	mTc = new TreeMeshComponent(this);
	mTc->SetOffsetPos(GetCapsulOffset());
	mTc->SetTree("Speed");
	mTc->SetDamageTree("SpeedTran");
}

bool SpeedUp::update()
{
	class Cannon* c = static_cast<class Cannon*>(GetGame()->GetCannon());
	if (!c->GetSpeed())
	{
		new SpeedUpCompo(c);
		c->GetSpeed()->SetInterval(GetGame()->GetAllData()->speedCompoData.mInterval);
		c->GetSpeed()->SetTime(c->GetSpeed()->GetInterval());
		c->GetSpeed()->SetColor(iData.mColor);
		std::ostringstream oss;
		oss << GetName() << "アイテムを使用。";
		GetGame()->GetStage()->GetLog()->AddText(oss.str());
	}
	else
	{
		if (c->GetSpeed()->GetLevel() != c->GetSpeed()->GetMaxLevel())
		{
			c->GetSpeed()->SetLevel(c->GetSpeed()->GetLevel() + 1);
		}
		c->GetSpeed()->SetTime(c->GetSpeed()->GetInterval());

		std::ostringstream oss;
		oss << c->GetSpeed()->GetName().c_str() << "がレベルアップ。";
		GetGame()->GetStage()->GetLog()->AddText(oss.str());
	}

	setVolume(iData.mSound1, GetGame()->GetEffectVolume() + mSpeedUpSoundVolumeOffset);
	playSound(iData.mSound1);

	return true;

}

SpeedUpCompo::SpeedUpCompo(class Cannon* owner)
	:ItemComponent(owner)
{
	owner->SetSpeed(this);
	Data = owner->GetGame()->GetAllData()->speedCompoData;
	Data.mHp = Data.mMaxHp;
}

SpeedUpCompo::~SpeedUpCompo()
{
	class Cannon* c = static_cast<class Cannon*>(mOwner->GetGame()->GetCannon());
	if (c)
	{
		c->SetAdvSpeed(mOwner->GetGame()->GetAllData()->cannonData.mAdvSpeed);
		std::ostringstream oss;
		oss << c->GetSpeed()->GetName().c_str() << "の効果が切れた。";
		c->GetGame()->GetStage()->GetLog()->AddText(oss.str());
		c->SetSpeed(nullptr);
	}

}

void SpeedUpCompo::Update()
{
	class Cannon* c = static_cast<class Cannon*>(mOwner->GetGame()->GetCannon());
	if (c)
	{
		c->SetAdvSpeed(mOwner->GetGame()->GetAllData()->cannonData.mAdvSpeed + Data.mLevel * mOwner->GetGame()->GetAllData()->cannonData.mAdvSpeed * mSpeedUpRate);
	}
	ItemComponent::Update();
}

PowerUp::PowerUp(class Game* game)
	:Item(game)
{
	iData = GetGame()->GetAllData()->powerData;
	SetImageColor(iData.mColor);
	SetUp();
	mTc = new TreeMeshComponent(this);
	mTc->SetOffsetPos(GetCapsulOffset());
	mTc->SetTree("Power");
	mTc->SetDamageTree("PowerTran");
}

bool PowerUp::update()
{
	class Cannon* c = static_cast<class Cannon*>(GetGame()->GetCannon());
	if (!c->GetPower())
	{
		new PowerUpCompo(c);
		c->GetPower()->SetInterval(GetGame()->GetAllData()->powerCompoData.mInterval);
		c->GetPower()->SetTime(c->GetPower()->GetInterval());
		c->GetPower()->SetColor(iData.mColor);
		std::ostringstream oss;
		oss << GetName() << "アイテムを使用。";
		GetGame()->GetStage()->GetLog()->AddText(oss.str());
	}
	else
	{
		if (c->GetPower()->GetLevel() != c->GetPower()->GetMaxLevel())
		{
			c->GetPower()->SetLevel(c->GetPower()->GetLevel() + 1);
		}
		c->GetPower()->SetTime(c->GetPower()->GetInterval());
		std::ostringstream oss;
		oss << c->GetPower()->GetName().c_str() << "がレベルアップ。";
		GetGame()->GetStage()->GetLog()->AddText(oss.str());
	}

	setVolume(iData.mSound1, GetGame()->GetEffectVolume() + mPowerUpSoundVolumeOffset);
	playSound(iData.mSound1);

	return true;
}

PowerUpCompo::PowerUpCompo(class Cannon* owner)
	:ItemComponent(owner)
{
	owner->SetPower(this);
	Data = owner->GetGame()->GetAllData()->powerCompoData;
	Data.mHp = Data.mMaxHp;
}

PowerUpCompo::~PowerUpCompo()
{
	class Cannon* c = static_cast<class Cannon*>(mOwner);

	if (mOwner->GetGame()->GetCannon())
	{
		c->SetDamage(1);
		std::ostringstream oss;
		oss << c->GetPower()->GetName().c_str() << "の効果が切れた。";
		c->GetGame()->GetStage()->GetLog()->AddText(oss.str());
		c->SetPower(nullptr);
	}
}

void PowerUpCompo::Update()
{
	class Cannon* c = static_cast<class Cannon*>(mOwner);
	if (mOwner->GetGame()->GetCannon())
	{
		c->SetDamage(Data.mLevel + 1);
	}
	ItemComponent::Update();

}

RapidFire::RapidFire(class Game* game)
	:Item(game)
{
	iData = GetGame()->GetAllData()->rapidData;
	SetImageColor(iData.mColor);
	SetUp();
	mTc = new TreeMeshComponent(this);
	mTc->SetOffsetPos(GetCapsulOffset());
	mTc->SetTree("Rapid");
	mTc->SetDamageTree("RapidTran");
}

bool RapidFire::update()
{
	class Cannon* c = static_cast<class Cannon*>(GetGame()->GetCannon());

	if (!c->GetRapid())
	{
		new RapidFireCompo(c);
		c->GetRapid()->SetInterval(GetGame()->GetAllData()->rapidCompoData.mInterval);
		c->GetRapid()->SetTime(c->GetRapid()->GetInterval());
		c->GetRapid()->SetColor(iData.mColor);
		std::ostringstream oss;
		oss << GetName() << "アイテムを使用。";
		GetGame()->GetStage()->GetLog()->AddText(oss.str());
	}
	else
	{
		if (c->GetRapid()->GetLevel() != c->GetRapid()->GetMaxLevel())
		{
			c->GetRapid()->SetLevel(c->GetRapid()->GetLevel() + 1);
			std::ostringstream oss;
			oss << c->GetRapid()->GetName().c_str() << "がレベルアップ。";
			GetGame()->GetStage()->GetLog()->AddText(oss.str());
		}
		c->GetRapid()->SetTime(c->GetRapid()->GetInterval());
	}

	setVolume(iData.mSound1, GetGame()->GetEffectVolume() + mRapidFireSoundVolumeOffset);
	playSound(iData.mSound1);

	return true;

}

RapidFireCompo::RapidFireCompo(class Cannon* owner)
	:ItemComponent(owner)
{
	owner->SetRapid(this);
	Data = owner->GetGame()->GetAllData()->rapidCompoData;
	Data.mHp = Data.mMaxHp;
}

RapidFireCompo::~RapidFireCompo()
{
	class Cannon* c = static_cast<class Cannon*>(mOwner);

	if (mOwner->GetGame()->GetCannon())
	{
		c->SetInterval(mOwner->GetGame()->GetAllData()->cannonData.mInterval);
		std::ostringstream oss;
		oss << c->GetRapid()->GetName().c_str() << "の効果が切れた。";
		c->GetGame()->GetStage()->GetLog()->AddText(oss.str());
		c->SetRapid(nullptr);
	}

}

void RapidFireCompo::Update()
{
	class Cannon* c = static_cast<class Cannon*>(mOwner);

	if (mOwner->GetGame()->GetCannon())
	{
		c->SetInterval(mOwner->GetGame()->GetAllData()->cannonData.mInterval - Data.mLevel * mLaunchIntervalDecreaseRate);
	}

	ItemComponent::Update();
}

Barrier::Barrier(class Game* game)
	:Item(game)
{
	iData = GetGame()->GetAllData()->barrierData;
	SetImageColor(iData.mColor);
	SetUp();
	mTc = new TreeMeshComponent(this);
	mTc->SetOffsetPos(GetCapsulOffset());
	mTc->SetTree("Barrier");
	mTc->SetDamageTree("BarrierTran");
}

bool Barrier::update()
{
	class Cannon* c = static_cast<class Cannon*>(GetGame()->GetCannon());

	if (!c->GetBarrier())
	{
		new BarrierCompo(c);
		c->GetBarrier()->SetInterval(GetGame()->GetAllData()->barrierCompoData.mInterval);
		c->GetBarrier()->SetTime(c->GetBarrier()->GetInterval());
		c->GetBarrier()->SetColor(iData.mColor);
		c->GetBarrier()->SetMaxHp(GetGame()->GetAllData()->barrierCompoData.mMaxHp);
		c->GetBarrier()->SetHp(c->GetBarrier()->GetMaxHp());
		std::ostringstream oss;
		oss << GetName() << "アイテムを使用。";
		GetGame()->GetStage()->GetLog()->AddText(oss.str());
	}
	else
	{
		if (c->GetBarrier()->GetLevel() != c->GetBarrier()->GetMaxLevel())
		{
			c->GetBarrier()->SetLevel(c->GetBarrier()->GetLevel() + 1);
			std::ostringstream oss;
			oss << c->GetBarrier()->GetName().c_str() << "がレベルアップ。";
			GetGame()->GetStage()->GetLog()->AddText(oss.str());;
		}
		c->GetBarrier()->SetMaxHp(c->GetBarrier()->GetLevel() * 2 + 1);
		c->GetBarrier()->SetHp(c->GetBarrier()->GetMaxHp());
		c->GetBarrier()->SetTime(c->GetBarrier()->GetInterval());
	}

	setVolume(iData.mSound1, GetGame()->GetEffectVolume() + mBarrierSoundVolumeOffset);
	playSound(iData.mSound1);

	return true;
}

BarrierCompo::BarrierCompo(class Cannon* owner)
	:ItemComponent(owner)
{
	owner->SetBarrier(this);
	Data = owner->GetGame()->GetAllData()->barrierCompoData;
	Data.mHp = Data.mMaxHp;
}

BarrierCompo::~BarrierCompo()
{
	class Cannon* c = static_cast<class Cannon*>(mOwner);

	if (mOwner->GetGame()->GetCannon())
	{
		c->SetRDamage(1);
		std::ostringstream oss;
		oss << c->GetBarrier()->GetName().c_str() << "の効果が切れた。";
		c->GetGame()->GetStage()->GetLog()->AddText(oss.str());
		c->SetBarrier(nullptr);
	}
}

void BarrierCompo::Update()
{
	class Cannon* c = static_cast<class Cannon*>(mOwner);

	if (c->GetGame()->GetCannon())
	{
		c->SetRDamage(0);
	}

	ItemComponent::Update();
}
