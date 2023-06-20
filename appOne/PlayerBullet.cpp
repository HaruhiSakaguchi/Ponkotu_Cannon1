#include "Game.h"
#include "PlayerBullet.h"
#include "COLLISION_MAP.h"
#include "Container.h"
#include "BatchMeshComponent.h"
#include "UILog.h"
#include <sstream>
#include "EnemyHome.h"

PlayerBullet::PlayerBullet(class Cannon* cannon, const VECTOR& pos, const VECTOR& dir)
	:SphereWeapon(cannon, pos, dir)
{
	//�SActorsWeapon���ʂ̃f�[�^
	Data = GetGame()->GetAllData()->pBulletData;
	//PlayerBullet�ŗL�̃f�[�^
	iData = GetGame()->GetAllData()->pBulletIData;
	SetUp(Data);
	BatchMeshComponent* bc = new BatchMeshComponent(this);
	bc->SetBatch("PlayerBulletSphere");

	//GamePlayState�̒��ł�������炳�Ȃ�
	if (GetGame()->GetState() == Game::EGameplay && GetGame()->GetCurState()->GetState() == UIMainState::State::EGamePlay)
	{
		setVolume(iData.mLaunchSound, GetGame()->GetSoundVolumeManager()->GetEffectVolume() + iData.mLaunchSoundVolumeOffset);
		playSound(iData.mLaunchSound);
	}

	SetDamage(cannon->GetDamage() + (int)(mOwner->GetLevel() / 2.0f));
}

void PlayerBullet::UpdateActor()
{
	for (auto enemy : GetGame()->GetActorManager()->GetEnemies())
	{
		if (Intersect(this, enemy, false) && enemy->GetHp() > 0)
		{
			this->damage();
			enemy->Damage(Data.mDamage);
			if (enemy->GetHp() <= 0)
			{
				break;
			}
		}
	}

	//�ǂɓ��������������
	if (GetGame()->GetActorManager()->GetStage()->GetCollisionMap()->capsule_triangles_walls(this) != 0)
	{
		ActorsWeapon::damage();
	}

	for (auto item : GetGame()->GetActorManager()->GetItems())
	{
		if (Intersect(this, item, false))
		{
			if (mOwner && mOwner->GetState() == CharacterActor::EActive)
			{
				class Cannon* c = static_cast<class Cannon*>(mOwner);
				setVolume(iData.mItemStockSound, GetGame()->GetSoundVolumeManager()->GetEffectVolume() + iData.mItemStockSoundVolumeOffset);
				playSound(iData.mItemStockSound);
				//�A�C�e���ɒe��������ƃA�C�e���̔ԍ���Cannon�̃A�C�e���i���o�[�z��ɓo�^���ăX�g�b�N����B
				c->AddItemNum(static_cast<class Item*>(item)->GetNum());
				item->SetState(EDead);
				std::ostringstream oss;
				oss << item->GetName() << "�A�C�e�����X�g�b�N�B";
				GetGame()->GetActorManager()->GetStage()->GetLog()->AddText(oss.str());
			}
			item->SetState(EDead);
		}
	}

	for (auto weapon : GetGame()->GetActorManager()->GetWeapons())
	{
		if (Intersect(this, weapon, false) && this->GetOwner() != weapon->GetOwner() && this != weapon && weapon->GetHp() > 0)
		{
			this->damage();
			weapon->damage();
		}
	}

	if (GetGame()->GetActorManager()->GetEHome())
	{
		if (Intersect(this, GetGame()->GetActorManager()->GetEHome(), false))
		{
			this->damage();
			GetGame()->GetActorManager()->GetEHome()->Damage(Data.mDamage);
		}
	}

	SphereWeapon::UpdateActor();
}

void PlayerBullet::damage()
{
	if (GetGame()->GetState() == Game::EGameplay && GetGame()->GetCurState()->GetState() == UIMainState::State::EGamePlay)
	{
		setVolume(iData.mImpactSound, GetGame()->GetSoundVolumeManager()->GetEffectVolume() + iData.mImpactSoundVolumeOffset);
		playSound(iData.mImpactSound);
	}
	ActorsWeapon::damage();
}
