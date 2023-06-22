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
	,mNum(0)
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
	new HpGaugeSpriteComponent(this, VECTOR(0.0f, 0.75f, 0.0f));
	new CollisionMapComponent(this);

	new UIPSideCharacterStatusClose(this);

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
		mTc->SetDrawFlag(false);
	}
	else
	{
		mTc->SetDrawFlag(true);
	}
}

void Barricade::Damage(int damage)
{
	if (GetDamageInterval() <= 0.0f)
	{
		SetHp(GetHp() - damage);
		SetDamageInterval(Data.mMaxDamageInterval);
	}
	else
	{
		return;
	}

	if (GetHp() <= 0)
	{
		std::ostringstream oss;
		oss << GetName().c_str() << "‚ª‰ó‚ê‚½";
		setVolume(mDeadSound, GetGame()->GetSoundVolumeManager()->GetEffectVolume());
		playSound(mDeadSound);
		GetGame()->GetActorManager()->GetStage()->AddText(oss.str().c_str());
		SetState(Actor::EDead);
	}

	for (auto enemy : GetGame()->GetActorManager()->GetEnemies())
	{
		Intersect(this, enemy);
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