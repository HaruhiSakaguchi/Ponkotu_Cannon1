#include "CollisionMapComponent.h"
#include "Game.h"
#include "COLLISION_MAP.h"
#include "window.h"

CollisionMapComponent::CollisionMapComponent(Actor* owner)
	:Component(owner)
	, mIsMapCollision(true)
{

}

void CollisionMapComponent::Update()
{
	CharacterActor* owner = static_cast<CharacterActor*>(mOwner);
	float floorY = 0;

	VECTOR pos = mOwner->GetPosition();
	float jumpVel = owner->GetJumpVel();
	int jumpFlag = owner->GetJumpFlag();

	if (mIsMapCollision)
	{
		if (mOwner->GetGame()->GetCollisionMap())
		{
			mOwner->GetGame()->GetCollisionMap()->capsule_triangles
			(&pos, &jumpVel, &jumpFlag, owner->GetAdvSpeed(), owner->GetRadius(), owner->GetOffsetY(), owner->GetSlant(), &floorY
			);
		}

	}
	if (jumpFlag == 0 && pos.y != floorY)
	{
		pos.y = floorY;
	}

	if (jumpFlag == 1)
	{
		pos.y += jumpVel * delta;
		jumpVel += owner->GetGravity() * delta;
	}

	owner->SetJumpVel(jumpVel);
	owner->SetJumpFlag(jumpFlag);
	owner->SetPosition(pos);

}