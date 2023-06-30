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
	SetNormalMesh(mTc);
	mTc = new TreeMeshComponent(this);
	mTc->SetTree("RecoverTran");
	mTc->SetOffsetPos(GetCapsulOffset());
	SetDamageMesh(mTc);
}

bool Recovery::update()
{
	auto p = static_cast<class PSideCharacterActor*>(mOwner);

	if (p->GetHp() < p->GetMaxHp())
	{
		new RecoveryCompo(p);
		setVolume(iData.mSound1, GetGame()->GetSoundVolumeManager()->GetEffectVolume() + mRecoverySoundVolumeOffset);
		playSound(iData.mSound1);
		std::ostringstream oss;
		oss << GetName() << "アイテムを使用。";
		GetGame()->GetActorManager()->GetStage()->GetLog()->AddText(oss.str());
	}
	else
	{
		if (mOwner->GetTag() == CharacterActor::CharactersTag::Cannon)
		{
			static_cast<class Cannon*>(p)->AddItemNum(iData.mNum);
			setVolume(iData.mSound2, GetGame()->GetSoundVolumeManager()->GetEffectVolume());
			playSound(iData.mSound2);
			std::ostringstream oss;
			oss << GetName() << "アイテムをストック。";
			GetGame()->GetActorManager()->GetStage()->GetLog()->AddText(oss.str());
		}
	}

	return true;
}

RecoveryCompo::RecoveryCompo(class PSideCharacterActor* owner)
	: ItemComponent(owner)
{
	Data = owner->GetGame()->GetAllData()->recoverCompoData;
	Data.mHp = Data.mMaxHp;
	if (owner->GetHp() < owner->GetMaxHp())
	{
		owner->SetHp(owner->GetHp() + mRecoveryHp);
	}
}

SpeedUp::SpeedUp(class Game* game)
	: Item(game)
{
	iData = GetGame()->GetAllData()->speedData;
	SetImageColor(iData.mColor);
	SetUp();
	mTc = new TreeMeshComponent(this);
	mTc->SetOffsetPos(GetCapsulOffset());
	mTc->SetTree("Speed");
	SetNormalMesh(mTc);

	mTc = new TreeMeshComponent(this);
	mTc->SetTree("SpeedTran");
	mTc->SetOffsetPos(GetCapsulOffset());
	SetDamageMesh(mTc);

}

bool SpeedUp::update()
{
	auto c = static_cast<class PSideCharacterActor*>(mOwner);
	if (c->GetTag() == CharacterActor::CharactersTag::Cannon)
	{
		if (!c->GetSpeed())
		{
			new SpeedUpCompo(c);
			c->GetSpeed()->SetInterval(GetGame()->GetAllData()->speedCompoData.mInterval);
			c->GetSpeed()->SetTime(c->GetSpeed()->GetInterval());
			c->GetSpeed()->SetColor(iData.mColor);
			std::ostringstream oss;
			oss << GetName() << "アイテムを使用。";
			GetGame()->GetActorManager()->GetStage()->GetLog()->AddText(oss.str());
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
			GetGame()->GetActorManager()->GetStage()->GetLog()->AddText(oss.str());
		}
	}
	setVolume(iData.mSound1, GetGame()->GetSoundVolumeManager()->GetEffectVolume() + mSpeedUpSoundVolumeOffset);
	playSound(iData.mSound1);

	return true;

}

SpeedUpCompo::SpeedUpCompo(class PSideCharacterActor* owner)
	: ItemComponent(owner)
{
	owner->SetSpeed(this);
	Data = owner->GetGame()->GetAllData()->speedCompoData;
	Data.mHp = Data.mMaxHp;
}

SpeedUpCompo::~SpeedUpCompo()
{
	class PSideCharacterActor* c = static_cast<class PSideCharacterActor*>(mOwner);
	if (c->GetState() == Actor::EActive && GetGame()->GetState() == Game::EGameplay)
	{
		c->SetAdvSpeed(GetGame()->GetAllData()->cannonData.mAdvSpeed);
		std::ostringstream oss;
		oss << c->GetSpeed()->GetName().c_str() << "の効果が切れた。";
		c->GetGame()->GetActorManager()->GetStage()->GetLog()->AddText(oss.str());
		c->SetSpeed(nullptr);
	}

}

void SpeedUpCompo::UpdateActor()
{
	ItemComponent::UpdateActor();
	class PSideCharacterActor* c = static_cast<class PSideCharacterActor*>(mOwner);
	if (c && c->GetTag() == CharacterActor::Cannon)
	{
		c->SetAdvSpeed(GetGame()->GetAllData()->cannonData.mAdvSpeed + Data.mLevel * GetGame()->GetAllData()->cannonData.mAdvSpeed * mSpeedUpRate);
	}
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
	SetNormalMesh(mTc);
	mTc = new TreeMeshComponent(this);
	mTc->SetOffsetPos(GetCapsulOffset());
	mTc->SetTree("PowerTran");
	SetDamageMesh(mTc);

}

bool PowerUp::update()
{
	class PSideCharacterActor* c = static_cast<class PSideCharacterActor*>(mOwner);
	if (c->GetTag() == CharacterActor::CharactersTag::Cannon)
	{
		if (!c->GetPower())
		{
			new PowerUpCompo(c);
			c->GetPower()->SetInterval(GetGame()->GetAllData()->powerCompoData.mInterval);
			c->GetPower()->SetTime(c->GetPower()->GetInterval());
			c->GetPower()->SetColor(iData.mColor);
			std::ostringstream oss;
			oss << GetName() << "アイテムを使用。";
			GetGame()->GetActorManager()->GetStage()->GetLog()->AddText(oss.str());
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
			GetGame()->GetActorManager()->GetStage()->GetLog()->AddText(oss.str());
		}
	}
	setVolume(iData.mSound1, GetGame()->GetSoundVolumeManager()->GetEffectVolume() + mPowerUpSoundVolumeOffset);
	playSound(iData.mSound1);

	return true;
}

PowerUpCompo::PowerUpCompo(class PSideCharacterActor* owner)
	:ItemComponent(owner)
{
	owner->SetPower(this);
	Data = owner->GetGame()->GetAllData()->powerCompoData;
	Data.mHp = Data.mMaxHp;
}

PowerUpCompo::~PowerUpCompo()
{
	class PSideCharacterActor* c = static_cast<class PSideCharacterActor*>(mOwner);

	if (c->GetState() == Actor::EActive && GetGame()->GetState() == Game::EGameplay)
	{
		c->SetDamage(1);
		std::ostringstream oss;
		oss << c->GetPower()->GetName().c_str() << "の効果が切れた。";
		c->GetGame()->GetActorManager()->GetStage()->GetLog()->AddText(oss.str());
		c->SetPower(nullptr);
	}
}

void PowerUpCompo::UpdateActor()
{
	ItemComponent::UpdateActor();
	class PSideCharacterActor* c = static_cast<class PSideCharacterActor*>(mOwner);
	if (c)
	{
		c->SetDamage(Data.mLevel + 1);
	}

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
	SetNormalMesh(mTc);
	mTc = new TreeMeshComponent(this);
	mTc->SetOffsetPos(GetCapsulOffset());
	mTc->SetTree("RapidTran");
	SetDamageMesh(mTc);

}

bool RapidFire::update()
{
	class PSideCharacterActor* c = static_cast<class PSideCharacterActor*>(mOwner);

	if (c->GetTag() == CharacterActor::CharactersTag::Cannon)
	{
		if (!c->GetRapid())
		{
			new RapidFireCompo(c);
			c->GetRapid()->SetInterval(GetGame()->GetAllData()->rapidCompoData.mInterval);
			c->GetRapid()->SetTime(c->GetRapid()->GetInterval());
			c->GetRapid()->SetColor(iData.mColor);
			if (GetGame()->GetState() == Game::EGameplay)
			{
				std::ostringstream oss;
				oss << GetName() << "アイテムを使用。";
				GetGame()->GetActorManager()->GetStage()->GetLog()->AddText(oss.str());
			}
		}
		else
		{
			if (c->GetRapid()->GetLevel() != c->GetRapid()->GetMaxLevel())
			{
				c->GetRapid()->SetLevel(c->GetRapid()->GetLevel() + 1);
				if (GetGame()->GetState() == Game::EGameplay)
				{
					std::ostringstream oss;
					oss << c->GetRapid()->GetName().c_str() << "がレベルアップ。";
					GetGame()->GetActorManager()->GetStage()->GetLog()->AddText(oss.str());
				}
			}
			c->GetRapid()->SetTime(c->GetRapid()->GetInterval());
		}
	}

	setVolume(iData.mSound1, GetGame()->GetSoundVolumeManager()->GetEffectVolume() + mRapidFireSoundVolumeOffset);
	playSound(iData.mSound1);

	return true;

}

RapidFireCompo::RapidFireCompo(class PSideCharacterActor* owner)
	: ItemComponent(owner)
{
	owner->SetRapid(this);
	Data = owner->GetGame()->GetAllData()->rapidCompoData;
	Data.mHp = Data.mMaxHp;
}

RapidFireCompo::~RapidFireCompo()
{
	if (mOwner->GetTag() == CharacterActor::Cannon && mOwner->GetState() == Actor::EActive && GetGame()->GetState() == Game::EGameplay)
	{
		auto c = static_cast<Cannon*>(mOwner);
		c->SetInterval(GetGame()->GetAllData()->cannonData.mInterval);
		std::ostringstream oss;
		oss << c->GetRapid()->GetName().c_str() << "の効果が切れた。";
		c->GetGame()->GetActorManager()->GetStage()->GetLog()->AddText(oss.str());
		c->SetRapid(nullptr);
	}

}

void RapidFireCompo::UpdateActor()
{
	ItemComponent::UpdateActor();

	if (mOwner->GetTag() == CharacterActor::CharactersTag::Cannon)
	{
		static_cast<Cannon*>(mOwner)->SetInterval(GetGame()->GetAllData()->cannonData.mInterval - Data.mLevel * mLaunchIntervalDecreaseRate);
	}

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
	SetNormalMesh(mTc);
	mTc = new TreeMeshComponent(this);
	mTc->SetOffsetPos(GetCapsulOffset());
	mTc->SetTree("BarrierTran");
	SetDamageMesh(mTc);

}

bool Barrier::update()
{
	class PSideCharacterActor* c = static_cast<class PSideCharacterActor*>(mOwner);

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
		GetGame()->GetActorManager()->GetStage()->GetLog()->AddText(oss.str());
	}
	else
	{
		if (c->GetBarrier()->GetLevel() != c->GetBarrier()->GetMaxLevel())
		{
			c->GetBarrier()->SetLevel(c->GetBarrier()->GetLevel() + 1);
			std::ostringstream oss;
			oss << c->GetBarrier()->GetName().c_str() << "がレベルアップ。";
			GetGame()->GetActorManager()->GetStage()->GetLog()->AddText(oss.str());;
		}
		c->GetBarrier()->SetMaxHp(c->GetBarrier()->GetLevel() * 2 + 1);
		c->GetBarrier()->SetHp(c->GetBarrier()->GetMaxHp());
		c->GetBarrier()->SetTime(c->GetBarrier()->GetInterval());
	}

	setVolume(iData.mSound1, GetGame()->GetSoundVolumeManager()->GetEffectVolume() + mBarrierSoundVolumeOffset);
	playSound(iData.mSound1);

	return true;
}

BarrierCompo::BarrierCompo(class PSideCharacterActor* owner)
	:ItemComponent(owner)
{
	owner->SetBarrier(this);
	Data = owner->GetGame()->GetAllData()->barrierCompoData;
	Data.mHp = Data.mMaxHp;
}

BarrierCompo::~BarrierCompo()
{
	class PSideCharacterActor* c = static_cast<class PSideCharacterActor*>(mOwner);

	if (c->GetState() == Actor::EActive && GetGame()->GetState() == Game::EGameplay)
	{
		c->SetRDamage(1);
		std::ostringstream oss;
		oss << c->GetBarrier()->GetName().c_str() << "の効果が切れた。";
		GetGame()->GetActorManager()->GetStage()->GetLog()->AddText(oss.str());
		c->SetBarrier(nullptr);
	}
}

void BarrierCompo::UpdateActor()
{
	ItemComponent::UpdateActor();
	class PSideCharacterActor* c = static_cast<class PSideCharacterActor*>(mOwner);

	if (c)
	{
		c->SetRDamage(0);
	}

}
