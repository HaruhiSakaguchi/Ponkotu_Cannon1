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
#include "CapsuleComponent.h"

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
		if (chara->GetTag() == CharacterActor::CharactersTag::EBarricade)
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
	SetTag(CharacterActor::CharactersTag::EBarricade);

	mCapsule = new CapsuleComponent(this);
	//mCapsule->SetIsCollision(false);
	mCapsule->AddNotCollisionTags((int)CharactersTag::EPHome);
	mCapsule->AddNotCollisionTags((int)CharactersTag::ESatellite);

	auto tree = new TreeMeshComponent(this);
	tree->SetTree("Barricade");
	SetNormalMesh(tree);

	//new HpGaugeSpriteComponent(this, VECTOR(0.0f, 0.75f, 0.0f));
	new CollisionMapComponent(this);

	auto ui = new UIPSideCharacterStatusClose(this);
	ui->UIOpenNexrOffsetPlus();

	int num = 0;
	for (auto chara : GetGame()->GetActorManager()->GetPSide())
	{
		if (chara->GetTag() == CharacterActor::CharactersTag::EBarricade)
		{
			num++;
		}
	}

	mNum = num - 1;

	std::ostringstream oss;
	oss << "Barricade" << mNum;
	SetName(oss.str().c_str());
	
	oss << "[Lv." << GetLevel() << "]" << "を設置。";
	GetGame()->GetActorManager()->GetStage()->GetLog()->AddText(oss.str());


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
		if (GetPosition().y > GetRadius() && mCapsule->OverlapActor(this, enemy) && enemy->GetHp() > 0)
		{
			enemy->Damage();
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
		SetState(Actor::State::EDead);
	}
}

void Barricade::Dead()
{
	PSideCharacterActor::Dead();

	std::ostringstream oss;
	oss << GetName().c_str() << "が壊れた";
	setVolume(mDeadSound, GetGame()->GetSoundVolumeManager()->GetEffectVolume());
	playSound(mDeadSound);
	GetGame()->GetActorManager()->GetStage()->GetLog()->AddText(oss.str().c_str());
	SpawnParticle(GetGame(), GetPosition(), "BarricadeBarricade", 10);
}
