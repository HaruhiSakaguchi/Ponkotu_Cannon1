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
	class Tama* t = static_cast<class Tama*>(mOwner);

	Data = GetGame()->GetAllData()->tWeaponData;
	iData = GetGame()->GetAllData()->tWeaponIData;
	UnMoveComponent();
	SetUp(Data);
	if (GetGame()->GetState() == Game::EGameplay && GetGame()->GetCurState()->GetState() == UIMainState::State::EGamePlay)
	{
		setVolume(iData.mAttackSound, GetGame()->GetSoundVolumeManager()->GetEffectVolume());
		playSound(iData.mAttackSound);
	}

	BatchMeshComponent* bc = new BatchMeshComponent(this);
	bc->SetBatch("TamaTamaSphere");

	t->SetWeaponCnt(t->GetWeaponCnt() + 1);

	mId = mNum % 2;
	mNum++;

	SetScale(VECTOR(Data.mSize, Data.mSize, Data.mSize));

}

TamaWeapon::~TamaWeapon()
{
	class Tama* t = static_cast<class Tama*>(mOwner);
	t->SetWeaponCnt(t->GetWeaponCnt() - 1);
	stopSound(iData.mAttackSound);
}

void TamaWeapon::UpdateActor()
{
	class Tama* t = static_cast<class Tama*>(mOwner);

	SetDamageInterval(t->GetDamageInterval());

	SetRotationY(GetRotation().y + 0.17f);
	SphereWeapon::UpdateActor();

	setVolume(iData.mAttackSound, GetGame()->GetSoundVolumeManager()->GetEffectVolume());

	VECTOR pos = t->GetPosition() + Data.mOffsetPos;
	VECTOR addPos;

	float preTime = Data.mTime;

	Data.mTime += delta * 2;

	MATRIX mat;
	mat.identity();
	mat.mulRotateY(GetRotation().y);

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

	//owner‚Ìhp‚ª0ˆÈã‚È‚çcannon‚Æ“–‚½‚è”»’è‚³‚¹‚é
	bool ownerDead = GetOwner()->GetHp() > 0;

	for (auto pSide : GetGame()->GetActorManager()->GetPSide())
	{
		if (Intersect(this, pSide, false))
		{
			pSide->Damage(1);
		}
	}
}