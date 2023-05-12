#include "SatelliteBullet.h"
#include "Game.h"
#include "Container.h"
#include "BatchMeshComponent.h"

SatelliteBullet::SatelliteBullet(Satellite* satellite, const VECTOR& pos, const VECTOR& dir)
	:SphereWeapon(satellite, pos, dir)
	, mCnt(0)
	, mDefDir(0.0f, 0.0f, 0.0f)
{
	Data = GetGame()->GetAllData()->sBulletData;
	iData = GetGame()->GetAllData()->sBulletIData;

	SetUp(Data);
	mDefDir = dir;
	BatchMeshComponent* bc = new BatchMeshComponent(this);
	bc->SetBatch("SatelliteBulletSphere");

	if (GetGame()->GetScene() == Game::EPlay && GetGame()->GetState() == Game::EGameplay)
	{
		setVolume(iData.mLaunchSound, GetGame()->GetEffectVolume());
		playSound(iData.mLaunchSound);
	}
}

void SatelliteBullet::UpdateActor()
{
	if (mOwner->GetHp() > 0)
	{

		if (++mCnt >= mChangeFlame)
		{
			VECTOR mTarget = VECTOR(0.0f, 0.0f, 0.0f);
			for (auto pSide : GetGame()->GetPSide())
			{
				if (pSide->GetHp() != 0)
				{
					VECTOR target = pSide->GetPosition();
					target.y += pSide->GetCapsulOffset().y;
					float distx = target.x - GetPosition().x;
					float disty = target.y - (GetPosition().y);
					float distz = target.z - GetPosition().z;
					float dist = sqrtf(distx * distx + disty * disty + distz * distz);

					float tdistx = mTarget.x - GetPosition().x;
					float tdisty = mTarget.y - (GetPosition().y);
					float tdistz = mTarget.z - GetPosition().z;
					float tdist = sqrtf(tdistx * tdistx + tdisty * tdisty + tdistz * tdistz);

					if (mTarget.x == 0.0f && mTarget.y == 0.0f && mTarget.z == 0.0f)
					{
						mTarget = pSide->GetPosition() + pSide->GetCapsulOffset();
					}
					else if (dist <= tdist)
					{
						mTarget = pSide->GetPosition() + pSide->GetCapsulOffset();
					}
				}
			}

			//if (mCnt % 2 == 0)
			{
				SetDir((mTarget - GetPosition()).normalize());
			}
			//else
			{
				//SetDir(mDefDir);
			}
			mMove->SetSpeed(GetAdvSpeed() * 2.0f);
			mMove->SetDirection(GetDir());
		}
		else if (static_cast<Satellite*>(mOwner)->GetId() == 1)
		{
			mMove->SetSpeed(0.0f);
		}

	}
	else
	{
		mMove->SetDirection(VECTOR(0.0f, -1.0f, 0.0f));
		mMove->SetSpeed(GetAdvSpeed() * 2.0f);
	}

	for (auto pSide : GetGame()->GetPSide())
	{
		if (Intersect(this, pSide, false))
		{
			SetState(Actor::EDead);
			setVolume(iData.mImpactSound, GetGame()->GetEffectVolume());
			playSound(iData.mImpactSound);
			pSide->Damage(1);
		}
	}

	if (!GetOnMapFlag())
	{
		SetState(Actor::EDead);
	}

	SphereWeapon::UpdateActor();
}

