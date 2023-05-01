#include "TamaStates.h"
#include "Tama.h"
#include "StateComponent.h"
#include "Collision_Capsul.h"
#include "Game.h"
#include "window.h"
#include "rand.h"
#include "TamaWeapon.h"
#include "COLLISION_MAP.h"
#include "PlayerHome.h"

void TamaWait::OnEnter()
{
	Tama* t = static_cast<Tama*>(mOwnerCompo->GetActor());

	elapsedTime = t->GetElapsedtime();
}

void TamaWait::Update()
{
	Tama* t = static_cast<Tama*>(mOwnerCompo->GetActor());

	elapsedTime += random(1) * delta * 60.0f;

	if (elapsedTime > t->GetMoveInterval())
	{
		elapsedTime = 0;
		t->SetElapsedTime(elapsedTime);
		for (auto pSide : t->GetGame()->GetPSide())
		{
			if (CollisionCircle(8, pSide->GetRadius(), t->GetPosition(), pSide->GetPosition()))
			{
				if (CollisionCircle(2.75f, pSide->GetRadius(), t->GetPosition(), pSide->GetPosition()))
				{
					mOwnerCompo->ChangeState("Attack");
					return;
				}
				else
				{
					mOwnerCompo->ChangeState("Chace");
					return;
				}
			}
		}
		if (t->GetJumpFlag() == 0)
		{
			mOwnerCompo->ChangeState("Move");
			return;
		}
	}

	if (Intersect(t, t->GetGame()->GetCannon()))
	{
		t->SetElapsedTime(t->GetMoveInterval());
	}

}

void TamaMove::OnEnter()
{
	Tama* t = static_cast<Tama*>(mOwnerCompo->GetActor());

	mAdv = VECTOR((float)random(-1, 1), t->GetPosition().y, (float)random(-1, 1));
	mTarget = t->GetPosition() + mAdv;
	mCnt = 0;
}

void TamaMove::Update()
{
	Tama* t = static_cast<Tama*>(mOwnerCompo->GetActor());

	VECTOR prePos = t->GetPosition();
	VECTOR pos = prePos;
	if (t->GetGame()->GetPHome())
	{
		mTarget = t->GetGame()->GetPHome()->GetPosition();
	}
	VECTOR vec = mTarget - pos;

	vec.normalize();

	VECTOR angle = t->GetRotation();
	t->rotate(&angle, vec, 0.05f, 1);
	t->SetRotation(angle);

	pos.x += vec.x * t->GetAdvSpeed() * delta * 6.0f;
	pos.z += vec.z * t->GetAdvSpeed() * delta * 6.0f;

	//移動先がマップエリア内か
	if (t->PositionOnMap(pos, t->GetRadius()))
	{
		t->SetPosition(pos);
	}
	else
	{
		t->SetPosition(prePos);
		mOwnerCompo->ChangeState("Wait");
		return;
	}

	if (++mCnt >= 20)
	{
		mOwnerCompo->ChangeState("Wait");
		return;
	}

	for (auto pSide : t->GetGame()->GetPSide())
	{
		if (Intersect(t, pSide))
		{
			t->SetElapsedTime(t->GetMoveInterval());
			mOwnerCompo->ChangeState("Wait");
			return;
		}
	}
}

void TamaChase::OnEnter()
{
	Tama* t = static_cast<Tama*>(mOwnerCompo->GetActor());

	for (auto pSide : t->GetGame()->GetPSide())
	{
		if (pSide->GetState() == Actor::EActive)
		{
			VECTOR target = pSide->GetPosition();
			float distx = target.x - t->GetPosition().x;
			float disty = target.y - t->GetPosition().y;
			float distz = target.z - t->GetPosition().z;
			float dist = sqrtf(distx * distx + disty * disty + distz * distz);

			float tdistx = mTarget.x - t->GetPosition().x;
			float tdisty = mTarget.y - t->GetPosition().y;
			float tdistz = mTarget.z - t->GetPosition().z;
			float tdist = sqrtf(tdistx * tdistx + tdisty * tdisty + tdistz * tdistz);

			if (mTarget.x == 0.0f && mTarget.y == 0.0f && mTarget.z == 0.0f)
			{
				mTarget = pSide->GetPosition();
			}
			else if (dist < tdist)
			{
				mTarget = pSide->GetPosition();
			}
		}
	}

	setVolume(t->GetDushSound(), t->GetGame()->GetEffectVolume() - 450);


	mDushFlag = true;
	playSound(t->GetDushSound());


	mCnt = 0;
}

void TamaChase::Update()
{
	Tama* t = static_cast<Tama*>(mOwnerCompo->GetActor());
	setVolume(t->GetDushSound(), t->GetGame()->GetEffectVolume() - 450);

	VECTOR pos = t->GetPosition();
	VECTOR prePos = pos;
	VECTOR vec = mTarget - pos;
	vec.normalize();

	VECTOR angle = t->GetRotation();
	t->rotate(&angle, vec, 0.5f, 1);
	t->SetRotation(angle);

	pos.x += vec.x * delta * t->GetAdvSpeed() * 60.0f;
	pos.z += vec.z * delta * t->GetAdvSpeed() * 60.0f;

	if (t->PositionOnMap(pos, t->GetRadius()) && t->GetGame()->GetCollisionMap())
	{
		if (t->GetGame()->GetCollisionMap()->segment_triangles(pos, &mTarget, t->GetRadius()) == 1)
		{
			t->SetPosition(pos);
			t->SetElapsedTime(t->GetMoveInterval());
			mOwnerCompo->ChangeState("Wait");
			return;
		}
		else
		{
			t->SetPosition(pos);
		}
	}
	else
	{
		t->SetPosition(prePos);
		mOwnerCompo->ChangeState("Move");
		return;
	}

	for (auto pSide : t->GetGame()->GetPSide())
	{
		if (Intersect(t, pSide))
		{
			t->SetElapsedTime(t->GetMoveInterval());
			pSide->Damage(1);
			mOwnerCompo->ChangeState("Wait");
			return;
		}
		else
		{
			if (++mCnt >= 15)
			{
				mOwnerCompo->ChangeState("Wait");
				return;
			}
		}
	}


}

void TamaChase::OnExit()
{
	mTarget = VECTOR(0.0f, 0.0f, 0.0f);
}

void TamaAttack::OnEnter()
{
	Tama* t = static_cast<Tama*>(mOwnerCompo->GetActor());
	mTime = 0;
	mRotation = t->GetRotation();

	if (t->GetHp() > 0)
	{
		new TamaWeapon(t);
		new TamaWeapon(t);
	}

	t->SetRotationY(0);

}

void TamaAttack::Update()
{
	Tama* t = static_cast<Tama*>(mOwnerCompo->GetActor());
	mTime += delta * 60.0f;
	if (!t->GetOnMapFlag())
	{
		t->SetJumpFlag(0);
	}
	float animAngle = t->GetAnimAngle();

	animAngle += t->GetAnimSpeed();
	t->SetRotationY(t->GetRotation().y + animAngle);

	t->SetAnimAngle(animAngle);

	for (auto pSide : t->GetGame()->GetPSide())
	{
		if (pSide != t->GetGame()->GetCannon() && Intersect(t, pSide))
		{
			pSide->Damage();
		}
	}

	if (Intersect(t, t->GetGame()->GetCannon()))
	{
		static_cast<class Cannon*>(t->GetGame()->GetCannon())->Damage(t);
	}

	if (mTime >= mMaxTime)
	{
		mOwnerCompo->ChangeState("Wait");
		return;
	}
}

void TamaAttack::OnExit()
{
	Tama* t = static_cast<Tama*>(mOwnerCompo->GetActor());
	t->SetRotation(mRotation);
	if (!t->GetOnMapFlag())
	{
		t->SetJumpFlag(1);
	}
}



