#include "Barricade.h"
#include "CollisionMapComponent.h"
#include "HpGaugeSpriteComponent.h"
#include "TreeMeshComponent.h"
#include "Collision_Capsul.h"
#include "Game.h"
#include "window.h"

Barricade::Barricade(Game* game)
	:CharacterActor(game)
{
	SetUp();
	GetGame()->AddPSide(this);
}

Barricade::~Barricade()
{
	GetGame()->RemovePSide(this);
}

int Barricade::SetUp()
{
	SetHp(Data.mHp);
	SetGravity(Data.mGravity);
	SetRadius(Data.mRadius);
	SetJumpFlag(Data.mJumpFlag);
	SetAdvSpeed(Data.mAdvSpeed);
	SetCapsulOffset(Data.mCapsuleOffset);
	SetImageColor(Data.mImageColor);

	TreeMeshComponent* tc = new TreeMeshComponent(this);
	tc->SetTree("Barricade");
	new HpGaugeSpriteComponent(this, VECTOR(0.0f, 0.75f, 0.0f));
	new CollisionMapComponent(this);

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
		setVolume(mDeadSound, GetGame()->GetEffectVolume());
		playSound(mDeadSound);
		GetGame()->GetStage()->AddText("Barricade����ꂽ");
		SetState(Actor::EDead);
	}
}