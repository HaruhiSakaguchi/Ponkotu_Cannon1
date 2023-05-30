#include "Barricade.h"
#include "CollisionMapComponent.h"
#include "HpGaugeSpriteComponent.h"
#include "TreeMeshComponent.h"
#include "Collision_Capsul.h"
#include "Game.h"
#include "window.h"
#include "UIPSideCharacterStatusClose.h"
#include <sstream>

Barricade::Barricade(Game* game)
	:PSideCharacterActor(game)
	,mNum(0)
{
	SetUp();
}

Barricade::~Barricade()
{
	for (auto chara : GetGame()->GetPSide())
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
	SetTag(CharacterActor::Barricade);


	TreeMeshComponent* tc = new TreeMeshComponent(this);
	tc->SetTree("Barricade");
	new HpGaugeSpriteComponent(this, VECTOR(0.0f, 0.75f, 0.0f));
	new CollisionMapComponent(this);

	new UIPSideCharacterStatusClose(this);

	int num = 0;
	for (auto chara : GetGame()->GetPSide())
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
		//std::ostringstream oss;
		//oss << GetName().c_str() << "‚ª‰ó‚ê‚½";
		setVolume(mDeadSound, GetGame()->GetEffectVolume());
		playSound(mDeadSound);
		//GetGame()->GetStage()->AddText(oss.str().c_str());
		SetState(Actor::EDead);
	}

	for (auto enemy : GetGame()->GetEnemies())
	{
		Intersect(this, enemy);
	}
}