#include "TamaStates.h"
#include "Tama.h"
#include "StateComponent.h"
#include "Collision_Capsul.h"
#include "Game.h"
#include "window.h"
#include "rand.h"
#include "TamaWeapon.h"
#include "COLLISION_MAP.h"

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
		if (t->GetGame()->GetCannon())
		{
			if (CollisionCircle(8, t->GetGame()->GetCannon()->GetRadius(), t->GetPosition(), t->GetGame()->GetCannon()->GetPosition()))
			{
				if (CollisionCircle(2.75f, t->GetGame()->GetCannon()->GetRadius(), t->GetPosition(), t->GetGame()->GetCannon()->GetPosition()))
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
			else if (t->GetJumpFlag() == 0)
			{
				mOwnerCompo->ChangeState("Move");
				return;
			}
		}
		else
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
	mAdv = VECTOR((float)random(-3, 3), t->GetPosition().y, (float)random(-3, 3));
	mTarget = t->GetPosition() + mAdv;
	mCnt = 0;
}

void TamaMove::Update()
{
	Tama* t = static_cast<Tama*>(mOwnerCompo->GetActor());

	VECTOR prePos = t->GetPosition();
	VECTOR pos = prePos;
	VECTOR vec = mTarget - pos;
	vec.normalize();

	VECTOR angle = t->GetRotation();
	t->rotate(&angle, vec, 0.05f, 1);
	t->SetRotation(angle);

	pos.x += vec.x * delta;
	pos.z += vec.z * delta;

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

	if (Intersect(t, t->GetGame()->GetCannon()))
	{
		t->SetElapsedTime(t->GetMoveInterval());
		mOwnerCompo->ChangeState("Wait");
		return;
	}


}

void TamaChase::OnEnter()
{
	Tama* t = static_cast<Tama*>(mOwnerCompo->GetActor());
	if (t->GetGame()->GetCannon())
	{
		mTarget = t->GetGame()->GetCannon()->GetPosition();
		setVolume(t->GetDushSound(), t->GetGame()->GetEffectVolume() - 450);

		if (t->GetPosition().y == t->GetGame()->GetCannon()->GetPosition().y)
		{
			mDushFlag = true;
			playSound(t->GetDushSound());
		}
	}
	else
	{
		mAdv = VECTOR((float)random(-1, 1), t->GetPosition().y, (float)random(-1, 1));
		mTarget = t->GetPosition() + mAdv;
	}

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
		if (t->GetGame()->GetCollisionMap()->segment_triangles(pos,&mTarget, t->GetRadius()) == 1)
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

	if (Intersect(t, t->GetGame()->GetCannon()))
	{
		t->SetElapsedTime(t->GetMoveInterval());
		static_cast<class Cannon*>(t->GetGame()->GetCannon())->Damage(t);
		mOwnerCompo->ChangeState("Wait");
		return;
	}


	if (++mCnt >= 15)
	{
		mOwnerCompo->ChangeState("Wait");
		return;
	}

}

void TamaAttack::OnEnter()
{
	Tama* t = static_cast<Tama*>(mOwnerCompo->GetActor());
	mTime = 0;
	mRotation = t->GetRotation();

	if (t->GetGame()->GetCannon() && t->GetHp() > 0)
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

	if (Intersect(t, t->GetGame()->GetCannon()))
	{
		static_cast<class Cannon*>(t->GetGame()->GetCannon())->Damage(t);
	}

	if (mTime >= mMaxTime || !t->GetGame()->GetCannon())
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



