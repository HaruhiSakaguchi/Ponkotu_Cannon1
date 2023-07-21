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

	mCapsule = new CapsuleComponent(this);
	//mCapsule->SetIsCollision(false);
	mCapsule->AddNotCollisionTags((int)PHome);
	mCapsule->AddNotCollisionTags((int)Satellite);

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
		/*if (enemy->GetTag() != CharacterActor::CharactersTag::Satellite && enemy->GetHp() > 0)
		{
			if (Intersect(this, enemy))
			{
				if (enemy->GetPosition().y < 0.0f)
				{
					enemy->SetPosition(enemy->GetPosition().x, 0.0f, enemy->GetPosition().z);
				}
			}
		}*/

		if (GetPosition().y > GetRadius() && mCapsule->OverlapActor(this, enemy) && enemy->GetHp() > 0)
		{
			enemy->Damage();
		}
	}

	/*if (GetPosition().y <= 0.0f)
	{
		mCapsule->SetIsCollision(true);
	}*/
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
	GetGame()->GetActorManager()->GetStage()->GetLog()->AddText(oss.str().c_str());
	SpawnParticle(GetGame(), GetPosition(), "BarricadeBarricade", 10);
}
