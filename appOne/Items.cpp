#include "Items.h"
#include "Game.h"
#include "TreeMeshComponent.h"
#include "BatchMeshComponent.h"
#include "UILog.h"
#include <sstream>

Recovery::Recovery(class Game* game)
	:ItemObject(game)
{
	iData = GetGame()->GetAllData()->recoverData;
	SetImageColor(iData.mColor);
	SetUp();

	mBatchName = "RecoverSphere";
	CreateMesh(mBatchName, "RecoverTranSphere");
}

bool Recovery::update()
{
	auto p = static_cast<class PSideCharacterActor*>(mOwner);

	if (p->GetHp() < p->GetMaxHp())
	{
		new RecoveryEffect(p);
		setVolume(iData.mSound1, GetGame()->GetSoundVolumeManager()->GetEffectVolume() + mRecoverySoundVolumeOffset);
		playSound(iData.mSound1);
		std::ostringstream oss;
		oss << GetName() << "アイテムを使用。";
		GetGame()->GetActorManager()->GetStage()->GetLog()->AddText(oss.str());
	}
	else
	{
		if (mOwner->GetTag() == CharacterActor::CharactersTag::ECannon)
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

RecoveryEffect::RecoveryEffect(class PSideCharacterActor* owner)
	: ItemEffect(owner)
{
	Data = owner->GetGame()->GetAllData()->recoverEffectData;
	SetHp(Data.mMaxHp);
	SetMaxHp(GetHp());
	SetName(Data.mName.c_str());
	SetMaxLevel(Data.mMaxLevel);

	if (owner->GetHp() < owner->GetMaxHp())
	{
		owner->SetHp(owner->GetHp() + mRecoveryHp);
	}
	mMeshName = "RecoverSphere";

	mOwner->SpawnParticle(GetGame(), mOwner->GetPosition() + mOwner->GetCapsulOffset(), mMeshName, 10);
}

SpeedUp::SpeedUp(class Game* game)
	: ItemObject(game)
{
	iData = GetGame()->GetAllData()->speedData;
	SetImageColor(iData.mColor);
	SetUp();

	mBatchName = "SpeedSphere";
	CreateMesh(mBatchName, "SpeedTranSphere");
}

bool SpeedUp::update()
{
	auto c = static_cast<class PSideCharacterActor*>(mOwner);
	if (c->GetTag() == CharacterActor::CharactersTag::ECannon)
	{
		if (!c->GetSpeed())
		{
			new SpeedUpEffect(c);
			c->GetSpeed()->SetInterval(GetGame()->GetAllData()->speedEffectData.mInterval);
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
			c->GetSpeed()->TimeReset();

			std::ostringstream oss;
			oss << c->GetSpeed()->GetName().c_str() << "がレベルアップ。";
			GetGame()->GetActorManager()->GetStage()->GetLog()->AddText(oss.str());
		}
	}
	setVolume(iData.mSound1, GetGame()->GetSoundVolumeManager()->GetEffectVolume() + mSpeedUpSoundVolumeOffset);
	playSound(iData.mSound1);

	return true;

}

SpeedUpEffect::SpeedUpEffect(class PSideCharacterActor* owner)
	: ItemEffect(owner)
{
	owner->SetSpeed(this);
	Data = owner->GetGame()->GetAllData()->speedEffectData;
	mMeshName = "SpeedSphere";
	SetHp(Data.mMaxHp);
	SetMaxHp(GetHp());
	SetName(Data.mName.c_str());
	SetMaxLevel(Data.mMaxLevel);


}

SpeedUpEffect::~SpeedUpEffect()
{

}

void SpeedUpEffect::UpdateActor()
{
	ItemEffect::UpdateActor();
	class PSideCharacterActor* c = static_cast<class PSideCharacterActor*>(mOwner);
	if (c && c->GetTag() == CharacterActor::CharactersTag::ECannon)
	{
		c->SetAdvSpeed(GetGame()->GetAllData()->cannonData.mAdvSpeed + Data.mLevel * GetGame()->GetAllData()->cannonData.mAdvSpeed * mSpeedUpRate);
	}
}

void SpeedUpEffect::Dead()
{
	ItemEffect::Dead();
	class PSideCharacterActor* c = static_cast<class PSideCharacterActor*>(mOwner);
	if (c && c->GetState() == Actor::State::EActive && GetGame()->GetState() == Game::GameState::EGameplay)
	{
		c->SetAdvSpeed(GetGame()->GetAllData()->cannonData.mAdvSpeed);
		c->GetGame()->GetActorManager()->GetStage()->GetLog()->AddText("スピードアップの効果が切れた。");
		c->SetSpeed(nullptr);
	}

}

PowerUp::PowerUp(class Game* game)
	:ItemObject(game)
{
	iData = GetGame()->GetAllData()->powerData;
	SetImageColor(iData.mColor);
	SetUp();

	mBatchName = "PowerSphere";
	CreateMesh(mBatchName, "PowerTranSphere");

}

bool PowerUp::update()
{
	class PSideCharacterActor* c = static_cast<class PSideCharacterActor*>(mOwner);
	if (c->GetTag() == CharacterActor::CharactersTag::ECannon)
	{
		if (!c->GetPower())
		{
			new PowerUpEffect(c);
			c->GetPower()->SetInterval(GetGame()->GetAllData()->powerEffectData.mInterval);
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
			c->GetPower()->TimeReset();
			std::ostringstream oss;
			oss << c->GetPower()->GetName().c_str() << "がレベルアップ。";
			GetGame()->GetActorManager()->GetStage()->GetLog()->AddText(oss.str());
		}
	}
	setVolume(iData.mSound1, GetGame()->GetSoundVolumeManager()->GetEffectVolume() + mPowerUpSoundVolumeOffset);
	playSound(iData.mSound1);

	return true;
}

PowerUpEffect::PowerUpEffect(class PSideCharacterActor* owner)
	:ItemEffect(owner)
{
	owner->SetPower(this);
	Data = owner->GetGame()->GetAllData()->powerEffectData;
	SetHp(Data.mMaxHp);
	mMeshName = "PowerSphere";
	SetMaxHp(GetHp());
	SetName(Data.mName.c_str());
	SetMaxLevel(Data.mMaxLevel);


}

PowerUpEffect::~PowerUpEffect()
{

}

void PowerUpEffect::UpdateActor()
{
	ItemEffect::UpdateActor();
	class PSideCharacterActor* c = static_cast<class PSideCharacterActor*>(mOwner);
	if (c)
	{
		c->SetDamage(Data.mLevel + 1);
	}

}

void PowerUpEffect::Dead()
{
	ItemEffect::Dead();
	class PSideCharacterActor* c = static_cast<class PSideCharacterActor*>(mOwner);
	if (c->GetState() == Actor::State::EActive && GetGame()->GetState() == Game::GameState::EGameplay)
	{
		c->SetDamage(1);
		c->GetGame()->GetActorManager()->GetStage()->GetLog()->AddText("攻撃力アップの効果が切れた。");
		c->SetPower(nullptr);
	}

}

RapidFire::RapidFire(class Game* game)
	:ItemObject(game)
{
	iData = GetGame()->GetAllData()->rapidData;
	SetImageColor(iData.mColor);
	SetUp();

	mBatchName = "RapidSphere";
	CreateMesh(mBatchName, "RapidTranSphere");
	SetMaxHp(GetHp());

}

bool RapidFire::update()
{
	class PSideCharacterActor* c = static_cast<class PSideCharacterActor*>(mOwner);

	if (c->GetTag() == CharacterActor::CharactersTag::ECannon)
	{
		if (!c->GetRapid())
		{
			new RapidFireEffect(c);
			c->GetRapid()->SetInterval(GetGame()->GetAllData()->rapidEffectData.mInterval);
			c->GetRapid()->SetTime(c->GetRapid()->GetInterval());
			c->GetRapid()->SetColor(iData.mColor);
			if (GetGame()->GetState() == Game::GameState::EGameplay)
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
				if (GetGame()->GetState() == Game::GameState::EGameplay)
				{
					std::ostringstream oss;
					oss << c->GetRapid()->GetName().c_str() << "がレベルアップ。";
					GetGame()->GetActorManager()->GetStage()->GetLog()->AddText(oss.str());
				}
			}
			c->GetRapid()->TimeReset();
		}
	}

	setVolume(iData.mSound1, GetGame()->GetSoundVolumeManager()->GetEffectVolume() + mRapidFireSoundVolumeOffset);
	playSound(iData.mSound1);

	return true;

}

RapidFireEffect::RapidFireEffect(class PSideCharacterActor* owner)
	: ItemEffect(owner)
{
	owner->SetRapid(this);
	Data = owner->GetGame()->GetAllData()->rapidEffectData;
	SetHp(Data.mMaxHp);
	mMeshName = "RapidSphere";
	SetMaxHp(GetHp());
	SetName(Data.mName.c_str());
	SetMaxLevel(Data.mMaxLevel);


}

RapidFireEffect::~RapidFireEffect()
{

}

void RapidFireEffect::UpdateActor()
{
	ItemEffect::UpdateActor();

	if (mOwner->GetTag() == CharacterActor::CharactersTag::ECannon)
	{
		static_cast<Cannon*>(mOwner)->SetInterval(GetGame()->GetAllData()->cannonData.mInterval - Data.mLevel * mLaunchIntervalDecreaseRate);
	}

}

void RapidFireEffect::Dead()
{
	ItemEffect::Dead();
	if (mOwner->GetTag() == CharacterActor::CharactersTag::ECannon && mOwner->GetState() == Actor::State::EActive && GetGame()->GetState() == Game::GameState::EGameplay)
	{
		auto c = static_cast<Cannon*>(mOwner);
		c->SetInterval(GetGame()->GetAllData()->cannonData.mInterval);
		c->GetGame()->GetActorManager()->GetStage()->GetLog()->AddText("発射間隔短縮の効果が切れた。");
		c->SetRapid(nullptr);
	}

}

Barrier::Barrier(class Game* game)
	:ItemObject(game)
{
	iData = GetGame()->GetAllData()->barrierData;
	SetImageColor(iData.mColor);
	SetUp();

	mBatchName = "BarrierSphere";
	CreateMesh(mBatchName, "BarrierTranSphere");

}

bool Barrier::update()
{
	class PSideCharacterActor* c = static_cast<class PSideCharacterActor*>(mOwner);

	if (!c->GetBarrier())
	{
		new BarrierEffect(c);
		c->GetBarrier()->SetInterval(GetGame()->GetAllData()->barrierEffectData.mInterval);
		c->GetBarrier()->SetTime(c->GetBarrier()->GetInterval());
		c->GetBarrier()->SetColor(iData.mColor);
		c->GetBarrier()->SetMaxHp(GetGame()->GetAllData()->barrierEffectData.mMaxHp);
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
		c->GetBarrier()->TimeReset();
	}

	setVolume(iData.mSound1, GetGame()->GetSoundVolumeManager()->GetEffectVolume() + mBarrierSoundVolumeOffset);
	playSound(iData.mSound1);

	return true;
}

BarrierEffect::BarrierEffect(class PSideCharacterActor* owner)
	:ItemEffect(owner)
{
	owner->SetBarrier(this);
	Data = owner->GetGame()->GetAllData()->barrierEffectData;
	SetHp(Data.mMaxHp);
	SetMaxHp(GetHp());
	SetMaxLevel(Data.mMaxLevel);

	mMeshName = "BarrierSphere";

}

BarrierEffect::~BarrierEffect()
{

}

void BarrierEffect::Dead()
{
	ItemEffect::Dead();
	class PSideCharacterActor* c = static_cast<class PSideCharacterActor*>(mOwner);

	if (c->GetState() == Actor::State::EActive && GetGame()->GetState() == Game::GameState::EGameplay)
	{
		c->SetRDamage(1);
		std::ostringstream oss;
		oss << c->GetBarrier()->GetName().c_str() << "の効果が切れた。";
		GetGame()->GetActorManager()->GetStage()->GetLog()->AddText(oss.str());
		c->SetBarrier(nullptr);
	}
}

void BarrierEffect::UpdateActor()
{
	ItemEffect::UpdateActor();
	class PSideCharacterActor* c = static_cast<class PSideCharacterActor*>(mOwner);

	if (c)
	{
		c->SetRDamage(0);
	}

}

