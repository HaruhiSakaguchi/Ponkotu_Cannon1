#include "Barricade.h"
#include "CollisionMapComponent.h"
#include "HpGaugeSpriteComponent.h"
#include "TreeMeshComponent.h"
#include "Collision_Capsul.h"
#include "Game.h"
#include "window.h"
#include "UIPSideCharacterStatusClose.h"
#include <sstream>
#include "PlayerHome.h"

Barricade::Barricade(Game* game)
	:PSideCharacterActor(game)
	, mNum(0)
{
	SetUp();
}

Barricade::~Barricade()
{
	for (auto chara : GetGame()->GetActorManager()->GetPSide())
	{
		if (chara->GetTag() == CharacterActor::Barricade)
		{
			auto barri = static_cast<Barricade*>(chara);
			if (barri->GetBNum() > this->GetBNum())
			{
				barri->SetBNum(barri->GetBNum() - 1);
			}
		}
	}
}

int Barricade::SetUp()
{
	SetHp(Data.mHp);
	SetMaxHp(Data.mHp);
	SetGravity(Data.mGravity);
	SetRadius(Data.mRadius);
	SetJumpFlag(Data.mJumpFlag);
	SetAdvSpeed(Data.mAdvSpeed);
	SetCapsulOffset(Data.mCapsuleOffset);
	SetImageColor(Data.mImageColor);
	SetInitMaxHp(GetMaxHp());
	SetTag(CharacterActor::Barricade);


	mTc = new TreeMeshComponent(this);
	mTc->SetTree("Barricade");
	SetNormalMesh(mTc);

	new HpGaugeSpriteComponent(this, VECTOR(0.0f, 0.75f, 0.0f));
	new CollisionMapComponent(this);

	auto ui = new UIPSideCharacterStatusClose(this);
	ui->UIOpenNexrOffsetPlus();

	int num = 0;
	for (auto chara : GetGame()->GetActorManager()->GetPSide())
	{
		if (chara->GetTag() == CharacterActor::Barricade)
		{
			num++;
		}
	}

	mNum = num - 1;

	std::ostringstream oss;
	oss << "Barricade" << mNum;
	SetName(oss.str().c_str());

	return 1;
}

void Barricade::UpdateActor()
{
	if (GetDamageInterval() > 0.0f)
	{
		SetDamageInterval(GetDamageInterval() - delta);
	}

	for (auto enemy : GetGame()->GetActorManager()->GetEnemies())
	{
		if (enemy->GetTag() != CharacterActor::CharactersTag::Satellite)
		{
			Intersect(this, enemy);
		}

		if (GetPosition().y > GetRadius() && Intersect(this,enemy,false))
		{
			enemy->Damage();
		}
	}

	for (auto pSide : GetGame()->GetActorManager()->GetPSide())
	{
		if (pSide != this && pSide != GetGame()->GetActorManager()->GetPHome())
		{
			if (pSide->GetTag() != CharacterActor::Cannon)
			{
				Intersect(this, pSide);
			}
			else if (static_cast<class Cannon*>(pSide)->GetStateCompoState()->GetName() != "Generate")
			{
				Intersect(this, pSide);
			}
		}
	}
}

void Barricade::Damage(int damage)
{
	if (GetDamageInterval() <= 0.0f)
	{
		if (GetBarrier())
		{
			GetBarrier()->SetHp(GetBarrier()->GetHp() - 1);
		}
		else
		{
			SetHp(GetHp() - damage);
		}

		SetDamageInterval(Data.mMaxDamageInterval);
	}
	else
	{
		return;
	}

	if (GetHp() <= 0)
	{
		SetState(Actor::EDead);
	}
}

void Barricade::Dead()
{
	std::ostringstream oss;
	oss << GetName().c_str() << "‚ª‰ó‚ê‚½";
	setVolume(mDeadSound, GetGame()->GetSoundVolumeManager()->GetEffectVolume());
	playSound(mDeadSound);
	GetGame()->GetActorManager()->GetStage()->AddText(oss.str().c_str());
	SpawnParticle(GetPosition(), "BarricadeBarricade", 10);
}
