#include "SatelliteStates.h"
#include "Satellite.h"
#include "Game.h"
#include "Collision_Capsul.h"
#include "SatelliteBullet.h"
#include "window.h"

void SatelliteMove(Satellite* s)
{
	//VECTOR pos = s->GetInitPosition();
	VECTOR pos = s->GetPosition();
	if (s->GetHp() <= 0)
	{
		pos = s->GetPosition();
	}
	else 
	{
		/*if (s->GetId() == 0)
		{
			pos.z += sinf(s->GetAngleForPos()) * s->GetMoveRange();
		}
		else
		{
			pos.x += cosf(s->GetAngleForPos()) * s->GetMoveRange();
		}*/

		pos.z += delta * 0.1f;
	}

	s->SetPosition(pos);

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

	SatelliteMove(s);
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
		s->SetElapsedTime(0);
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
	SatelliteMove(s);

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