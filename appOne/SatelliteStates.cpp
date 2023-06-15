#include "SatelliteStates.h"
#include "Satellite.h"
#include "Game.h"
#include "Collision_Capsul.h"
#include "SatelliteBullet.h"
#include "window.h"
#include "rand.h"
#include "PlayerHome.h"
#include "EnemyHome.h"

void SatelliteCountElapsedTime(Satellite* s)
{
	float elapsedTime = s->GetElapsedTime();
	elapsedTime += delta;

	s->SetElapsedTime(elapsedTime);

}

void SatelliteRotation(Satellite* s)
{
	float angleForPos = s->GetAngleForPos();
	angleForPos += s->GetAdvSpeed();

	s->SetAngleForPos(angleForPos);

	float animAngle = s->GetAnimAngle();
	animAngle += s->GetAnimSpeed();

	s->SetAnimAngle(animAngle);
}

void SatelliteNormal::OnEnter()
{
	Satellite* s = static_cast<Satellite*>(mOwnerCompo->GetActor());
	//s->SetElapsedTime(0.0f);
	for (auto wing : s->GetWings())
	{
		wing->SetAttackFlag(false);
	}
}

void SatelliteNormal::Update()
{
	Satellite* s = static_cast<Satellite*>(mOwnerCompo->GetActor());

	/*if (s->GetId() == 1)
	{
		s->SetRotationY(s->GetRotation().y + 0.17f * 0.5f);
	}*/

	SatelliteRotation(s);
	SatelliteCountElapsedTime(s);

	if (s->GetElapsedTime() >= s->GetLaunchInterval())
	{
		for (auto pSide : s->GetGame()->GetPSide())
		{
			if (CollisionCircle(s->GetRange(), pSide->GetRadius(), s->GetPosition(), pSide->GetPosition()))
			{
				mOwnerCompo->ChangeState("RockOn");
				return;
			}
			else
			{
				s->SetElapsedTime(0.0f);
			}
		}
	}
	else
	{
		mOwnerCompo->ChangeState("Move");
		//s->SetElapsedTime(0);
		return;
	}
}

void SatelliteMove::OnEnter()
{
	Satellite* s = static_cast<Satellite*>(mOwnerCompo->GetActor());
	//s->SetElapsedTime(0.0f);
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

	if (s->GetId() == 1)
	{
		s->SetRotationY(s->GetRotation().y + 0.17f);
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

void SatelliteRockOn::OnEnter()
{
	Satellite* s = static_cast<Satellite*>(mOwnerCompo->GetActor());
	mTarget = VECTOR(0.0f, 0.0f, 0.0f);

	for (auto pSide : s->GetGame()->GetPSide())
	{
		if (pSide->GetHp() != 0)
		{
			VECTOR target = pSide->GetPosition();
			target.y += pSide->GetCapsulOffset().y;
			float distx = target.x - s->GetPosition().x;
			float disty = target.y - (s->GetPosition().y);
			float distz = target.z - s->GetPosition().z;
			float dist = sqrtf(distx * distx + disty * disty + distz * distz);

			float tdistx = mTarget.x - s->GetPosition().x;
			float tdisty = mTarget.y - (s->GetPosition().y);
			float tdistz = mTarget.z - s->GetPosition().z;
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

	s->SetTargetPos(mTarget);
	mAdv = mTarget - (s->GetPosition() + s->GetBulletOffset());
	mAdv.normalize();
	s->SetRange(mTarget.y);
	s->SetTmpRotation(s->GetRotation());
	s->SetTmpTargetVec(mAdv);
	s->SetElapsedTime(0.0f);
}

void SatelliteRockOn::Update()
{
	Satellite* s = static_cast<Satellite*>(mOwnerCompo->GetActor());
	VECTOR angle = s->GetRotation();
	int endOfRotate = s->rotate(&angle, mAdv, 0.05f);

	s->SetRotation(angle);

	if (endOfRotate == 1)
	{
		mOwnerCompo->ChangeState("Attack");
		return;
	}
}


void SatelliteAttack::OnEnter()
{
	Satellite* s = static_cast<Satellite*>(mOwnerCompo->GetActor());
	for (auto wing : s->GetWings())
	{
		wing->SetAttackFlag(true);
	}
	mCnt = 0;

	mMaxCnt = (int)(36 / (2 + (int)(s->GetLevel() / 2)));

	mRotateCnt = 36;
}

void SatelliteAttack::Update()
{
	Satellite* s = static_cast<Satellite*>(mOwnerCompo->GetActor());
	SatelliteRotation(s);

	s->SetRotationY(s->GetRotation().y + 0.17f);

	VECTOR offset = s->GetBulletOffset();

	VECTOR angle = s->GetRotation();

	int EndOfRotate = 0;

	/*if (s->GetId() == 0)
	{
		EndOfRotate = s->rotate(&angle, -s->GetType1TargetVec(), -0.05f);
	}
	else
	{
		EndOfRotate = s->rotate(&angle, -s->GetTargetPosition(), -0.05f);
	}*/

	s->SetRotation(angle);

	/*if (s->GetId() == 0)
	{
		if (mCnt <= mMaxCnt)
		{
			new SatelliteBullet(s, s->GetPosition() + offset, s->GetType1TargetVec());
		}
	}
	else
	{*/
	if (mCnt % mMaxCnt == 0)
	{
		if (s->GetId() == 0)
		{
			new SatelliteBullet(s, s->GetPosition() + offset, s->GetType1TargetVec());
		}
		else
		{
			new SatelliteBullet(s, s->GetTargetPosition(), VECTOR(0.0f, 0.0f, 0.0f));
		}
	}
	//}

	if (++mCnt >= mRotateCnt)
	{
		//VECTOR dir = VECTOR(0.0f, s->GetPosition().y, 0.0f);
		//int EndOfRotate = s->rotate(&angle, dir.normalize(), 0.05f);
		//s->SetRotation(angle);
		s->SetRotationX(0.0f);
		//if (EndOfRotate == 1)
		{
			mOwnerCompo->ChangeState("Normal");
			return;
		}
	}

}

void SatelliteAttack::OnExit()
{
	Satellite* s = static_cast<Satellite*>(mOwnerCompo->GetActor());
	s->SetRange(s->GetGame()->GetAllData()->satelliteData.mMaxRange);
	VECTOR angle = s->GetRotation();
	//s->SetRotationY(0.0f);
}


void SatelliteGenerate::OnEnter()
{
	Satellite* s = static_cast<Satellite*>(mOwnerCompo->GetActor());

	mFirstTargetCompleteFlag = false;
	mFirstTarget = VECTOR(s->GetGame()->GetEHome()->GetPosition() + VECTOR(0.0f, 2.0f, 10.0f));
}

void SatelliteGenerate::OnExit()
{
	Satellite* s = static_cast<Satellite*>(mOwnerCompo->GetActor());
	s->SetRotationX(0.0f);
}

void SatelliteGenerate::Update()
{
	Satellite* s = static_cast<Satellite*>(mOwnerCompo->GetActor());

	mTargetPos = s->GetInitPosition();
	if (!mFirstTargetCompleteFlag)
	{
		mTargetPos = mFirstTarget;
	}

	VECTOR vec = mTargetPos - s->GetPosition();
	vec.normalize();
	VECTOR angle = s->GetRotation();
	int endOfRotate = s->rotate(&angle, vec, 0.05f);
	s->SetRotation(angle);

	if (!mSwitch && endOfRotate == 1)
	{
		mRotateFlag = true;
		mSwitch = true;
	}

	if (s->GetGame()->GetEHome())
	{
		if (mRotateFlag)
		{
			if (!mFirstTargetCompleteFlag)
			{
				if (s->GetGame()->GetEHome()->GetOpenComplete())
				{
					s->SetPosition(s->GetPosition() + vec * s->GetAdvSpeed() * 10.0f);
					SatelliteRotation(s);
					if (s->GetId() == 1)
					{
						s->SetRotationY(s->GetRotation().y + 0.17f);
					}
				}
			}
			else
			{
				s->SetPosition(s->GetPosition() + vec * s->GetAdvSpeed() * 10.0f);
				SatelliteRotation(s);
				if (s->GetId() == 1)
				{
					s->SetRotationY(s->GetRotation().y + 0.17f);
				}
			}
		}

		if (CollisionCircle(s->GetRadius(), 0.5f, s->GetPosition(), mFirstTarget))
		{
			mFirstTargetCompleteFlag = true;
			mSwitch = false;
			mRotateFlag = false;
		}
		if (CollisionCircle(s->GetRadius(), 0.5f, s->GetPosition(), s->GetInitPosition()))
		{
			mOwnerCompo->ChangeState("Normal");
			return;
		}
	}
	else
	{
		mOwnerCompo->ChangeState("Normal");
		return;
	}
}