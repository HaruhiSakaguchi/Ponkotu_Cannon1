#include "CannonStates.h"
#include "Cannon.h"
#include "VECTOR2.h"
#include "InputComponent.h"
#include "StateComponent.h"
#include "window.h"
#include "PlayerBullet.h"
#include "Game.h"
#include "UIScope.h"
#include "COLLISION_MAP.h"

void MoveCannon(Cannon* p)
{
	VECTOR mDir = VECTOR(0.0f, 0.0f, 0.0f);

	//ˆÚ“®•ûŒü‚ðŒˆ‚ß‚é

	if (p->GetIn()->Up()) { mDir.z = -1; }
	if (p->GetIn()->Down()) { mDir.z = 1; }
	if (p->GetIn()->Left()) { mDir.x = -1; }
	if (p->GetIn()->Right()) { mDir.x = 1; }

	if (p->GetIn()->Vertical() && p->GetScope())
	{
		if (p->GetIn()->Up())
		{
			p->GetScope()->SetOffsetPos(VECTOR2(p->GetScope()->GetOffset().x, sinf(p->GetGame()->GetCamera()->GetRotation().x) * 500.0f));
		}
		else if (p->GetIn()->Down())
		{
			p->GetScope()->SetOffsetPos(VECTOR2(p->GetScope()->GetOffset().x, sinf(-p->GetGame()->GetCamera()->GetRotation().x) * (height / 2.0f)));
		}
	}
	else
	{
		p->GetScope()->SetOffsetPos(VECTOR2(p->GetScope()->GetOffset().x, 0));
	}
	if (p->GetIn()->Side() && p->GetScope())
	{
		if (p->GetGame()->GetCollisionMap()->capsule_triangles_walls(p) == 0)
		{
			if (p->GetIn()->Left())
			{
				p->GetScope()->SetOffsetPos(VECTOR2(width / 4.0f + 100.0f, p->GetScope()->GetOffset().y));
			}
			else if (p->GetIn()->Right())
			{
				p->GetScope()->SetOffsetPos(VECTOR2(-(width / 4.0f + 100.0f), p->GetScope()->GetOffset().y));
			}
		}
		else
		{
			p->GetScope()->SetOffsetPos(VECTOR2(0.0f, p->GetScope()->GetOffset().y));
		}
	}
	else
	{
		p->GetScope()->SetOffsetPos(VECTOR2(0, p->GetScope()->GetOffset().y));
	}

	p->SetBarrelDir(mDir);
	if (mDir.x == 0 && mDir.z == 0)return;


	if (p->GetIn()->Up())
	{
		p->SetWheelRotateZ(p->GetWheelRotateZ() + delta);
	}
	else if (p->GetIn()->Down())
	{
		p->SetWheelRotateZ(p->GetWheelRotateZ() - delta);
	}

	if (p->GetIn()->Left())
	{
		p->SetWheelRotateX(-3.1415926f / 12.0f);
	}
	else if (p->GetIn()->Right())
	{
		p->SetWheelRotateX(3.1415926f / 12.0f);
	}
	else
	{
		p->SetWheelRotateX(0.0f);
	}

	//s—ñ‚ðƒJƒƒ‰‚ÌŠp“x‚É‡‚í‚¹‚ÄYŽ²‰ñ“]‚³‚¹‚é
	MATRIX m;
	m.identity();
	m.mulRotateY(p->GetGame()->GetCamera()->GetRotation().y);
	mDir = m * mDir;
	mDir.normalize();

	//ˆÚ“®
	p->SetPosition(p->GetPosition() + mDir * p->GetAdvSpeed());
}

void CountInterval(Cannon* p)
{
	float mTimer = p->GetTimer();
	mTimer += delta * 0.25f;
	p->SetTimer(mTimer);
	if (p->GetTimer() >= p->GetInterval())
	{
		p->SetTimer(p->GetInterval());
	}
}

void UpCounter(Cannon* c)
{
	if (c->GetIn()->SelectBullet()) {
		if ((int)c->GetItemNums().size() != 0)
		{
			int Scnt = c->GetSlideCnt();
			Scnt++;
			c->SetSlideCnt(Scnt);
			if (c->GetSlideCnt() > 15)
			{
				int cnt = c->GetCnt();
				++cnt %= (int)c->GetItemNums().size();
				c->SetCnt(cnt);
				if (c->GetItemNums().size() != 1)
				{
					setVolume(c->GetSlide(), c->GetGame()->GetEffectVolume());
					playSound(c->GetSlide());
				}
				else
				{
					setVolume(c->GetOutOfBullets(), c->GetGame()->GetEffectVolume());
					playSound(c->GetOutOfBullets());
				}
				c->SetSlideCnt(0);
			}
		}
	}
	else
	{
		c->SetSlideCnt(20);
	}

}

void Launch(Cannon* p)
{
	if (p->GetGame()->GetScene() == Game::EPlay && p->GetGame()->GetState() == Game::EGameplay && p->GetIn()->StartLaunch() && p->GetTimer() == p->GetInterval())
	{
		if (p->GetCnt() == 0)
		{
			p->SetLaunchTime(50.0f);
			new PlayerBullet(p, p->GetPosition() + p->GetCapsulOffset(), VECTOR(p->GetTargetPosition() - (p->GetPosition() + p->GetCapsulOffset())).normalize());
		}
		if (p->GetCnt() != 0)
		{
			p->DropItems(p->GetItemNums()[p->GetCnt()]);
			p->RemoveItemNum(p->GetCnt());
			p->SetCnt(0);
		}

		p->SetTimer(0);
	}
}


void CannonWait::Update()
{
	Cannon* p = static_cast<Cannon*>(mOwnerCompo->GetActor());

	CountInterval(p);
	Launch(p);
	UpCounter(p);
	p->SetWheelRotateX(0.0f);


	if (p->GetIn()->StartMove())
	{
		mOwnerCompo->ChangeState("Move");
		return;
	}
	if (p->GetIn()->Jump())
	{
		mOwnerCompo->ChangeState("Jump");
		return;
	}
}


void CannonMove::Update()
{
	Cannon* p = static_cast<Cannon*>(mOwnerCompo->GetActor());

	MoveCannon(p);
	CountInterval(p);
	Launch(p);
	UpCounter(p);


	if (p->GetIn()->StopMove())
	{
		mOwnerCompo->ChangeState("Wait");
		return;

	}
	if (p->GetIn()->Jump())
	{
		mOwnerCompo->ChangeState("Jump");
		return;
	}
}

void CannonJump::OnEnter()
{
	Cannon* p = static_cast<Cannon*>(mOwnerCompo->GetActor());

	p->SetJumpFlag(1);
	p->SetJumpVel(10.0f);

	//‚¿‚å‚Á‚Æ•‚‚©‚¹‚é
	VECTOR pos = p->GetPosition();
	pos.y += p->GetJumpVel() * delta;
	p->SetPosition(pos);
}

void CannonJump::Update()
{
	Cannon* p = static_cast<Cannon*>(mOwnerCompo->GetActor());

	MoveCannon(p);
	Launch(p);
	CountInterval(p);
	UpCounter(p);


	p->GetScope()->SetOffsetPos(VECTOR2(0, 100));

	if (p->GetJumpFlag() == 0)
	{
		mOwnerCompo->ChangeState("Wait");
		p->GetScope()->SetOffsetPos(VECTOR2(0, 0));

		return;
	}
}

