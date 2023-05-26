#include "CharacterActor.h"
#include "Actor.h"
#include "Game.h"
#include "Component.h"
#include "window.h"
#include "Map.h"
#include "ItemComponent.h"
#include "Items.h"
#include "COLLISION_MAP.h"
#include "Container.h"

CharacterActor::CharacterActor(Game* game)
	: Actor(game)
	, mCapsulOffset(0.0f, 0.0f, 0.0f)
	, mRadius(0.0f)
	, mHeight(0.0f)
	, mJumpFlag(0)
	, mJumpVel(0.0f)
	, mGravity(0.0f)
	, mAdvSpeed(0.0f)
	, mSeg(nullptr)
	, mHp(1)
	, mMaxHp(1)
	, mDeadSound(-1)
	, mSlant(44.0f)//傾斜
	, mDamageInterval(0.0f)
	, mOnMapFlag(true)
	, mImageColor(255, 255, 255)
	, mDamageColor(255, 0, 0, 128)
	, mOffsetY(0.5f)
	, mLevel(0)
	, mName("Chara")

{
	SetCategory(Character);
	GetGame()->AddCharacter(this);
	mDeadSound = GetGame()->GetAllData()->mDeadSound;
}

CharacterActor::~CharacterActor()
{
	if (GetSeg())
	{
		delete mSeg;
	}
	GetGame()->RemoveCharacter(this);

}

void CharacterActor::Update()
{
	Actor::Update();
	if (GetState() == EActive)
	{
		if (GetSeg())
		{
			GetSeg()->update();
		}

		if (GetPosition().y < mMinPosY)
		{
			if (this == GetGame()->GetCannon())
			{
				SetPosition(GetInitPosition());
			}
			else
			{
				SetState(EDead);
			}

			FallOption();
		}

		//今、マップエリア内にいるか
		mOnMapFlag = PositionOnMap(GetPosition(), GetRadius());
	}

}

int CharacterActor::rotate(VECTOR* angle, const VECTOR& dir, float rotSpeed, int endOfRotationFlag)
{
	VECTOR b = dir;
	float angleBetweenX = -(acosf(-b.y) - 1.57f) - angle->x;
	angle->x += angleBetweenX * rotSpeed;
	//Ｙ軸回転させるangleを求める
	VECTOR a(sinf(angle->y), 0, cosf(angle->y));
	float dotProduct = a.x * b.x + a.z * b.z;
	float crossProduct = a.x * b.z - a.z * b.x;
	float angleBetweenY = atan2f(-crossProduct, dotProduct);
	angle->y += angleBetweenY * rotSpeed;
	//回転終了
	if (-0.017f < angleBetweenY && angleBetweenY < 0.017f)
	{
		mEndOfStateFlags |= endOfRotationFlag;
		return 1;
	}
	return 0;
}

int CharacterActor::mEndOfStateFlags = 0;
