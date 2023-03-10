#include "SatelliteStates.h"
#include "Satellite.h"
#include "Game.h"
#include "Collision_Capsul.h"
#include "SatelliteBullet.h"
#include "window.h"

void SatelliteMove(Satellite* s)
{
	VECTOR pos = s->GetInitPosition();
	if (s->GetHp() <= 0)
	{
		pos = s->GetPosition();
	}
	if (s->GetHp() > 0)
	{
		if (s->GetId() == 0)
		{
			pos.z += sinf(s->GetAngleForPos()) * s->GetMoveRange();
		}
		else
		{
			pos.x += cosf(s->GetAngleForPos()) * s->GetMoveRange();
		}
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
		if (s->GetGame()->GetCannon() && CollisionCircle(7, s->GetGame()->GetCannon()->GetRadius(), s->GetPosition(), s->GetGame()->GetCannon()->GetPosition()))
		{
			mOwnerCompo->ChangeState("Attack");
			return;
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
	if (s->GetGame()->GetCannon())
	{
		if (s->GetElapsedTime() >= s->GetLaunchInterval())
		{
			if (!CollisionCircle(7, s->GetGame()->GetCannon()->GetRadius(), s->GetPosition(), s->GetGame()->GetCannon()->GetPosition()))
			{
				mOwnerCompo->ChangeState("Normal");
				return;
			}
			else
			{
				VECTOR offset = s->GetBulletOffset();
				if (s->GetId() == 0)
				{
					new SatelliteBullet(s, s->GetPosition() + offset, VECTOR(s->GetGame()->GetCannon()->GetPosition() + s->GetGame()->GetCannon()->GetCapsulOffset() - (s->GetPosition() + offset)).normalize());
				}
				else
				{
					new SatelliteBullet(s, s->GetPosition() + offset, VECTOR(s->GetTargetPosition() - (s->GetPosition() + offset)).normalize());
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
	else
	{
		mOwnerCompo->ChangeState("Normal");
		return;
	}

}

void SatelliteAttack::OnExit()
{
	Satellite* s = static_cast<Satellite*>(mOwnerCompo->GetActor());
	s->SetRotationY(0.0f);
}