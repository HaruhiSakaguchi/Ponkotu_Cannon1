#include "TamaWeapon.h"
#include "Cannon.h"
#include "Game.h"
#include "window.h"
#include "BatchMeshComponent.h"

int TamaWeapon::mNum = 0;

TamaWeapon::TamaWeapon(class Tama* owner, const VECTOR& pos, const VECTOR& dir)
	: SphereWeapon(owner, owner->GetPosition(), dir)
	, mId(0)
{
	Tama* t = static_cast<Tama*>(mOwner);

	Data = GetGame()->GetAllData()->tWeaponData;
	iData = GetGame()->GetAllData()->tWeaponIData;
	UnMoveComponent();
	SetUp(Data);
	setVolume(iData.mAttackSound, GetGame()->GetEffectVolume());
	playSound(iData.mAttackSound);

	BatchMeshComponent* bc = new BatchMeshComponent(this);
	bc->SetBatch("TamaTamaSphere");

	t->SetWeaponCnt(t->GetWeaponCnt() + 1);

	mId = mNum % 2;
	mNum++;

	SetScale(VECTOR(Data.mSize, Data.mSize, Data.mSize));

}

TamaWeapon::~TamaWeapon()
{
	Tama* t = static_cast<Tama*>(mOwner);
	t->SetWeaponCnt(t->GetWeaponCnt() - 1);
	stopSound(iData.mAttackSound);
}

void TamaWeapon::UpdateActor()
{
	Tama* t = static_cast<Tama*>(mOwner);

	SetDamageInterval(t->GetDamageInterval());

	SphereWeapon::UpdateActor();

	setVolume(iData.mAttackSound, GetGame()->GetEffectVolume());

	VECTOR pos = t->GetPosition() + Data.mOffsetPos;
	VECTOR addPos;

	float preTime = Data.mTime;

	Data.mTime += delta * 2;

	MATRIX mat;
	mat.identity();
	mat.mulRotateY(t->GetRotation().y);

	if (mId == 0)
	{
		addPos.x = cosf(t->GetAnimAngle() * 10.0f) * Data.mTime;
		addPos.y = cosf(t->GetAnimAngle() * 10.0f);
		addPos.z = cosf(t->GetAnimAngle() * 10.0f) + sinf(t->GetAnimAngle() * 5.0f) * Data.mTime;
	}
	else
	{
		addPos.x = -cosf(t->GetAnimAngle() * 10.0f) * Data.mTime;
		addPos.y = cosf(t->GetAnimAngle() * 10.0f);
		addPos.z = cosf(t->GetAnimAngle() * 10.0f) + sinf(t->GetAnimAngle() * 5.0f) * Data.mTime;
	}

	addPos = mat * addPos;

	SetPosition(pos + addPos);

	//ownerのhpが0以上ならcannonと当たり判定させる
	bool ownerDead = GetOwner()->GetHp() > 0;

	if (GetGame()->GetCannon())
	{
		if (Intersect(this, GetGame()->GetCannon(), ownerDead) && ownerDead)
		{
			static_cast<class Cannon*>(GetGame()->GetCannon())->Damage(t);
		}
	}
	else
	{
		SetState(Actor::EDead);
	}
}