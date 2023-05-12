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
#include "TamaPointer.h"

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
		mOwnerCompo->ChangeState("Move");
		return;
	}
}

void TamaMove::OnEnter()
{
	Tama* t = static_cast<Tama*>(mOwnerCompo->GetActor());

	mAdv = VECTOR((float)random(-1, 1), t->GetPosition().y, (float)random(-1, 1));
	if (!t->GetGame()->GetPHome())
	{
		mTarget = t->GetPosition() + mAdv;
	}
	else
	{
		mTarget = VECTOR(1000.0f, 1000.0f, 1000.0f);
	}

	mCnt = 0;

	for (auto pSide : t->GetGame()->GetPSide())
	{
		if (pSide->GetState() == Actor::EActive)
		{
			VECTOR target = pSide->GetPosition() + pSide->GetCapsulOffset();
			float distx = target.x - t->GetPosition().x;
			float disty = target.y - t->GetPosition().y;
			float distz = target.z - t->GetPosition().z;
			float dist = sqrtf(distx * distx + disty * disty + distz * distz);

			float tdistx = mTarget.x - t->GetPosition().x;
			float tdisty = mTarget.y - t->GetPosition().y;
			float tdistz = mTarget.z - t->GetPosition().z;
			float tdist = sqrtf(tdistx * tdistx + tdisty * tdisty + tdistz * tdistz);

			if (mTarget.x == 1000.0f && mTarget.y == 1000.0f && mTarget.z == 1000.0f)
			{
				mTarget = target;
			}
			else if (dist < tdist)
			{
				mTarget = target;
			}
		}
	}
}

void TamaMove::Update()
{
	Tama* t = static_cast<Tama*>(mOwnerCompo->GetActor());

	VECTOR prePos = t->GetPosition();
	VECTOR pos = prePos;

	/*if (t->GetGame()->GetPHome())
	{
		mTarget = t->GetGame()->GetPHome()->GetPosition();
	}*/

	VECTOR vec = mTarget - pos;

	vec.normalize();

	VECTOR angle = t->GetRotation();
	t->rotate(&angle, vec, 0.05f);
	t->SetRotation(angle);

	if (CollisionCircle(3.0f, 1.0f, t->GetPosition(), mTarget))
	{
		mOwnerCompo->ChangeState("RockOn");
		//mOwnerCompo->ChangeState("Wait");
		return;
	}

	pos.x += vec.x * t->GetAdvSpeed() * delta * 60.0f;
	pos.z += vec.z * t->GetAdvSpeed() * delta * 60.0f;

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

	if (++mCnt >= 20 || CollisionCircle(5.0f, 1.0f, t->GetPosition(), mTarget))
	{
		mOwnerCompo->ChangeState("Search");
		//mOwnerCompo->ChangeState("Wait");
		return;
	}


	/*for (auto pSide : t->GetGame()->GetPSide())
	{
		if (Intersect(t, pSide, false))
		{
			t->SetElapsedTime(t->GetMoveInterval());
			mOwnerCompo->ChangeState("RockOn");
			return;
		}
	}*/
}
void TamaSeache::OnEnter()
{
	mCnt = 0;
	mRotateCnt = 0;
}

void TamaSeache::Update()
{
	Tama* t = static_cast<Tama*>(mOwnerCompo->GetActor());

	t->SetRotationY(t->GetRotation().y + 0.17f);

	if (++mRotateCnt < 36)
	{
		if (t->GetTp()->CheckCollisionPSide())
		{
			//mCnt++;
			//if (mCnt > 0)
			{
				mOwnerCompo->ChangeState("RockOn");
				return;
			}
		}
	}
	else
	{
		mOwnerCompo->ChangeState("Wait");
		return;
	}


}

void TamaRockOn::OnEnter()
{
	Tama* t = static_cast<Tama*>(mOwnerCompo->GetActor());
	mTarget = VECTOR(1000.0f, 1000.0f, 1000.0f);

	for (auto pSide : t->GetGame()->GetPSide())
	{
		if (pSide->GetState() == Actor::EActive)
		{
			VECTOR target = pSide->GetPosition() + pSide->GetCapsulOffset();
			float distx = target.x - t->GetPosition().x;
			float disty = target.y - t->GetPosition().y;
			float distz = target.z - t->GetPosition().z;
			float dist = sqrtf(distx * distx + disty * disty + distz * distz);

			float tdistx = mTarget.x - t->GetPosition().x;
			float tdisty = mTarget.y - t->GetPosition().y;
			float tdistz = mTarget.z - t->GetPosition().z;
			float tdist = sqrtf(tdistx * tdistx + tdisty * tdisty + tdistz * tdistz);

			if (mTarget.x == 1000.0f && mTarget.y == 1000.0f && mTarget.z == 1000.0f)
			{
				mTarget = target;
			}
			else if (dist < tdist)
			{
				mTarget = target;
			}
		}
	}

	mCnt = 0;
	t->SetTargetPos(mTarget);
	t->SetAttackVector(mTarget - (t->GetPosition() + t->GetCapsulOffset()));
}

void TamaRockOn::Update()
{
	Tama* t = static_cast<Tama*>(mOwnerCompo->GetActor());

	VECTOR angle = t->GetRotation();
	VECTOR vec = t->GetAttackVector();
	vec.normalize();
	int EndOfRotate = t->rotate(&angle, vec, 0.05f);
	t->SetRotation(angle);

	if (EndOfRotate == 1)
	{
	//	if (CollisionCircle(t->GetTp()->GetMaxDist(), 1.5f, t->GetPosition(), mTarget))
		{
			mOwnerCompo->ChangeState("Charge");
			return;
		}
		/*else
		{
			mOwnerCompo->ChangeState("Wait");
			t->SetTargetPos(VECTOR(0.0f, 0.0f, 0.0f));
			t->SetAttackVector(VECTOR(0.0f, 0.0f, 0.0f));
			return;
		}*/
	}
}

void TamaRockOn::OnExit()
{
}

void TamaCharge::OnEnter()
{
	Tama* t = static_cast<Tama*>(mOwnerCompo->GetActor());
	mTarget = t->GetTargetPos();
	mDir = t->GetAttackVector();
	VECTOR vec = mDir.normalize();
	mCnt = 0;
	mReb = 1.0f;
	mNX = (int)(mDir.x / vec.x);
	mNZ = (int)(mDir.z / vec.z);

	mDir.normalize();
}

void TamaCharge::Update()
{
	Tama* t = static_cast<Tama*>(mOwnerCompo->GetActor());
	VECTOR pos = t->GetPosition();
	if (mCnt < 20)
	{
		pos.x -= mDir.x * delta * 6.0f;
		pos.z -= mDir.z * delta * 6.0f;
	}

	if (20 < mCnt && mCnt < 40)
	{
		pos.x += mDir.x * delta * mReb * 60.0f;
		pos.z += mDir.z * delta * mReb * 60.0f;
		mReb *= -1;
	}

	t->SetDamageInterval(0.13f);
	t->SetPosition(pos);

	if (++mCnt >= 40)
	{
		mOwnerCompo->ChangeState("Attack");
		return;
	}
}

void TamaCharge::OnExit()
{
	Tama* t = static_cast<Tama*>(mOwnerCompo->GetActor());
	mTarget = VECTOR(0.0f, 0.0f, 0.0f);
	mDir = VECTOR(0.0f, 0.0f, 0.0f);
}

void TamaAttack::OnEnter()
{
	Tama* t = static_cast<Tama*>(mOwnerCompo->GetActor());
	/*mTime = 0;
	mRotation = t->GetRotation();

	if (t->GetHp() > 0)
	{
		new TamaWeapon(t);
		new TamaWeapon(t);
	}

	t->SetRotationY(0);*/
	mCnt = 0;
	setVolume(t->GetDushSound(), t->GetGame()->GetEffectVolume());
	playSound(t->GetDushSound());
}

void TamaAttack::Update()
{
	Tama* t = static_cast<Tama*>(mOwnerCompo->GetActor());

	VECTOR pos = t->GetPosition();

	pos.x += t->GetAttackVector().x * t->GetAdvSpeed();
	pos.z += t->GetAttackVector().z * t->GetAdvSpeed();


	t->SetPosition(pos);

	for (auto pSide : t->GetGame()->GetPSide())
	{
		if (Intersect(t, pSide, false))
		{
			pSide->Damage(5);
			mOwnerCompo->ChangeState("Wait");
			return;
		}
	}

	if (++mCnt >= 30 || !t->GetGame()->GetPHome())
	{
		mOwnerCompo->ChangeState("Search");
		return;
	}
}

void TamaAttack::OnExit()
{
	Tama* t = static_cast<Tama*>(mOwnerCompo->GetActor());
	//t->SetRotation(mRotation);
	if (!t->GetOnMapFlag())
	{
		t->SetJumpFlag(1);
	}
	t->SetAttackVector(VECTOR(0.0f, 0.0f, 0.0f));
	t->SetTargetPos(VECTOR(0.0f, 0.0f, 0.0f));
}



