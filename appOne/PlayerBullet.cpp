#include "Game.h"
#include "PlayerBullet.h"
#include "COLLISION_MAP.h"
#include "Container.h"
#include "BatchMeshComponent.h"
#include "UILog.h"
#include <sstream>
#include "EnemyHome.h"

PlayerBullet::PlayerBullet(Cannon* cannon, const VECTOR& pos, const VECTOR& dir)
	:SphereWeapon(cannon, pos, dir)
{
	//�SActorsWeapon���ʂ̃f�[�^
	Data = GetGame()->GetAllData()->pBulletData;
	//PlayerBullet�ŗL�̃f�[�^
	iData = GetGame()->GetAllData()->pBulletIData;
	SetUp(Data);
	BatchMeshComponent* bc = new BatchMeshComponent(this);
	bc->SetBatch("PlayerBulletSphere");

	//��C���W�����v���Ă�����Bullet���G�ɗ^����_���[�W��2�{�ɂȂ�
	if (cannon->GetJumpFlag() == 0)
	{
		SetDamage(cannon->GetDamage());
	}
	else
	{
		SetDamage(cannon->GetDamage() * 2);
	}

	//GamePlayState�̒��ł�������炳�Ȃ�
	if (GetGame()->GetScene() == Game::EPlay && GetGame()->GetState() == Game::EGameplay)
	{
		setVolume(iData.mLaunchSound, GetGame()->GetEffectVolume() + iData.mLaunchSoundVolumeOffset);
		playSound(iData.mLaunchSound);
	}
}

void PlayerBullet::UpdateActor()
{
	class Cannon* c = static_cast<Cannon*>(mOwner);

	for (auto enemy : GetGame()->GetEnemies())
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
	if (GetGame()->GetCollisionMap()->capsule_triangles_walls(this) != 0)
	{
		ActorsWeapon::damage();
	}

	for (auto item : GetGame()->GetItems())
	{
		if (Intersect(this, item, false))
		{
			setVolume(iData.mItemStockSound, GetGame()->GetEffectVolume() + iData.mItemStockSoundVolumeOffset);
			playSound(iData.mItemStockSound);
			//�A�C�e���ɒe��������ƃA�C�e���̔ԍ���Cannon�̃A�C�e���i���o�[�z��ɓo�^���ăX�g�b�N����B
			c->AddItemNum(static_cast<class Item*>(item)->GetNum());
			item->SetState(EDead);
			std::ostringstream oss;
			oss << item->GetName() << "�A�C�e�����X�g�b�N�B";
			GetGame()->GetStage()->GetLog()->AddText(oss.str());
		}
	}

	for (auto weapon : GetGame()->GetWeapons())
	{
		if (Intersect(this, weapon, false) && this->GetOwner() != weapon->GetOwner() && this != weapon && weapon->GetHp() > 0)
		{
			this->damage();
			weapon->damage();
		}
	}

	if (GetGame()->GetEHome())
	{
		if (Intersect(this, GetGame()->GetEHome(), false))
		{
			this->damage();
			GetGame()->GetEHome()->Damage(Data.mDamage);
		}
	}

	SphereWeapon::UpdateActor();
}

void PlayerBullet::damage()
{
	setVolume(iData.mImpactSound, GetGame()->GetEffectVolume() + iData.mImpactSoundVolumeOffset);
	playSound(iData.mImpactSound);
	ActorsWeapon::damage();
}
