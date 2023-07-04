#include "TamaPointer.h"
#include "Tama.h"
#include "BatchMeshComponent.h"
#include "Game.h"
#include "PlayerHome.h"

TamaPointer::TamaPointer(class Tama* owner)
	:CharacterActor(owner->GetGame())
	,mOwner(owner)
	,mTarget(0.0f,0.0f,0.0f)
{
	SetUp();
}

int TamaPointer::SetUp()
{
	auto nb = new BatchMeshComponent(this,false);
	nb->SetBatch("TamaSphere");
	SetNormalMesh(nb);
	auto db = new BatchMeshComponent(this,false);
	db->SetBatch("TamaDamageSphere");
	SetDamageMesh(db);
	SetCategory(CharacterActor::Object);
	SetRadius(0.4f);
	return 1;
}

void TamaPointer::UpdateActor()
{
	if (mOwner->GetState() == EDead)
	{
		SetState(EDead);
	}

	mTarget = VECTOR(1000.0f, 1000.0f, 1000.0f);

	for (auto pSide : GetGame()->GetActorManager()->GetPSide())
	{
		if (pSide->GetState() == Actor::EActive)
		{
			VECTOR target = pSide->GetPosition() + pSide->GetCapsulOffset();
			float distx = target.x - GetPosition().x;
			float disty = target.y - GetPosition().y;
			float distz = target.z - GetPosition().z;
			float dist = sqrtf(distx * distx + disty * disty + distz * distz);

			float tdistx = mTarget.x - GetPosition().x;
			float tdisty = mTarget.y - GetPosition().y;
			float tdistz = mTarget.z - GetPosition().z;
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

	VECTOR offset = mMaxOffsetPos;
	float tdistx = mTarget.x - GetPosition().x;
	float tdisty = mTarget.y - GetPosition().y;
	float tdistz = mTarget.z - GetPosition().z;
	float tdist = sqrtf(tdistx * tdistx + tdisty * tdisty + tdistz * tdistz);
	
	if ((mTarget.x != 1000.0f && mTarget.y != 1000.0f && mTarget.z != 1000.0f) && CollisionCircle(mMaxOffsetPos.z, 2.5f, GetPosition(), mTarget))
	{
		offset.x = tdistx;
		offset.y = tdisty;
		offset.z = tdistz;
	}

	Master.identity();
	Master.mulTranslate(mOwner->GetPosition() + mOwner->GetCapsulOffset());
	Master.mulRotateY(mOwner->GetRotation().y);
	Master.mulTranslate(offset);

	SetScale(VECTOR(GetRadius(),GetRadius(),GetRadius()));
	SetPosition(Master._14, Master._24,Master._34);


	if (GetCollisionPSide())
	{
		SetDamageInterval(1.0f);
	}
	else
	{
		SetDamageInterval(0.0f);
	}
}

bool TamaPointer::CheckCollisionPSide()
{
	int cnt = 0;
	for (auto pSide : GetGame()->GetActorManager()->GetPSide())
	{
		if (Intersect(this, pSide, false))
		{
			cnt++;
		}
	}



	if (cnt > 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}