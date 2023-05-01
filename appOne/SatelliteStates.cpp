#include "SatelliteStates.h"
#include "Satellite.h"
#include "Game.h"
#include "Collision_Capsul.h"
#include "SatelliteBullet.h"
#include "window.h"
#include "rand.h"
#include "PlayerHome.h"

void SatelliteRotation(Satellite* s)
{
	float angleForPos = s->GetAngleForPos();
	angleForPos += s->GetAdvSpeed();

	s->SetAngleForPos(angleForPos);

	float animAngle = s->GetAnimAngle();
	animAngle += s->GetAnimSpeed();

	s->SetAnimAngle(animAngle);

	float elapsedTime = s->GetElapsedTime();
	elapsedTime += delta;

	s->SetElapsedTime(elapsedTime);

}

void SatelliteNormal::OnEnter()
{
	Satellite* s = static_cast<Satellite*>(mOwnerCompo->GetActor());
	s->SetElapsedTime(0.0f);
	for (auto wing : s->GetWings())
	{
		wing->SetAttackFlag(false);
	}
}

void SatelliteNormal::Update()
{
	Satellite* s = static_cast<Satellite*>(mOwnerCompo->GetActor());
	if (s->GetId() == 1)
	{
		s->SetRotationY(s->GetRotation().y + 0.17f * 0.5f);
	}

	SatelliteRotation(s);
	
	if (s->GetElapsedTime() >= s->GetLaunchInterval())
	{
		for (auto pSide : s->GetGame()->GetPSide())
		{
			if (CollisionCircle(7, pSide->GetRadius(), s->GetPosition(), pSide->GetPosition()))
			{
				mOwnerCompo->ChangeState("Attack");
				return;
			}

		}

		mOwnerCompo->ChangeState("Move");
		s->SetElapsedTime(0);
		return;
	}
}

void SatelliteMove::OnEnter()
{
	Satellite* s = static_cast<Satellite*>(mOwnerCompo->GetActor());
	s->SetElapsedTime(0.0f);
	mAdv = VECTOR((float)random(-1, 1), s->GetPosition().y, (float)random(-1, 1));
	mTarget = s->GetPosition() + mAdv;
	mCnt = 0;
}

void SatelliteMove::Update()
{
	Satellite* s = static_cast<Satellite*>(mOwnerCompo->GetActor());
	SatelliteRotation(s);
	VECTOR pos = s->GetPosition();

	if (s->GetGame()->GetPHome())
	{
		mTarget = s->GetGame()->GetPHome()->GetPosition();
	}

	VECTOR vec = mTarget - pos;
	
	vec.normalize();

	VECTOR angle = s->GetRotation();
	s->rotate(&angle, vec, 0.05f, 1);
	s->SetRotationY(angle.y);

	pos.x += vec.x * s->GetAdvSpeed() * delta * 60.0f;
	pos.z += vec.z * s->GetAdvSpeed() * delta * 60.0f;

	if (pos.y < s->GetHeight())
	{
		pos.y += (s->GetInitPosition() - pos).normalize().y * s->GetAdvSpeed() * delta * 60.0f;
	}
	
	s->SetPosition(pos);

	if (++mCnt >= 20)
	{
		mOwnerCompo->ChangeState("Normal");
		return;
	}
}

void SatelliteAttack::OnEnter()
{
	Satellite* s = static_cast<Satellite*>(mOwnerCompo->GetActor());
	s->SetElapsedTime(0);
	for (auto wing : s->GetWings())
	{
		wing->SetAttackFlag(true);
	}

}
void SatelliteAttack::Update()
{
	Satellite* s = static_cast<Satellite*>(mOwnerCompo->GetActor());
	SatelliteRotation(s);

	s->SetRotationY(s->GetRotation().y + 0.17f);

	if (s->GetElapsedTime() >= s->GetLaunchInterval())
	{
		for (auto pSide : s->GetGame()->GetPSide())
		{
			if (!CollisionCircle(7, pSide->GetRadius(), s->GetPosition(), pSide->GetPosition()))
			{
				mOwnerCompo->ChangeState("Normal");
				return;
			}
			else
			{
				VECTOR offset = s->GetBulletOffset();
				if (s->GetId() == 0)
				{
					new SatelliteBullet(s, s->GetPosition() + offset, VECTOR((pSide->GetPosition() + pSide->GetCapsulOffset()) - (s->GetPosition() + offset)).normalize());
				}
				else
				{
					new SatelliteBullet(s, s->GetPosition() + offset, VECTOR(s->GetTargetPosition() - (s->GetPosition() + offset)).normalize());
				}
			}
		}

		if (s->GetId() == 1)
		{
			s->SetElapsedTime(s->GetLaunchInterval() / 2);
		}
		else
		{
			s->SetElapsedTime(0.0f);
		}
	}
}

void SatelliteAttack::OnExit()
{
	Satellite* s = static_cast<Satellite*>(mOwnerCompo->GetActor());
	s->SetRotationY(0.0f);
}