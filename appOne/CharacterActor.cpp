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
#include "rand.h"

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
	, mTag(Others)
	, mMaxLevel(10)
	, mInitMaxHp(1)
{
	SetCategory(Character);
	GetGame()->GetActorManager()->AddCharacter(this);
	mDeadSound = GetGame()->GetAllData()->mDeadSound;
}

CharacterActor::~CharacterActor()
{
	if (GetSeg())
	{
		delete mSeg;
	}
	GetGame()->GetActorManager()->RemoveCharacter(this);

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
			SetState(EDead);
			FallOption();
		}

		if (GetDamageInterval() > 0.0f)
		{
			if (mNormalMesh)
			{
				mNormalMesh->SetDrawFlag(false);
			}
			if (mDamageMesh)
			{
				mDamageMesh->SetDrawFlag(true);
			}
		}
		else
		{
			if (mNormalMesh)
			{
				mNormalMesh->SetDrawFlag(true);
			}
			if (mDamageMesh)
			{
				mDamageMesh->SetDrawFlag(false);
			}
		}

		//今、マップエリア内にいるか
		mOnMapFlag = PositionOnMap(GetPosition(), GetRadius());
	}

}

int CharacterActor::rotate(VECTOR* angle, const VECTOR& dir, float rotSpeed)
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
		return 1;
	}
	return 0;
}

void CharacterActor::SpawnParticle(const VECTOR& pos, const char* name, int num, float maxLifeSpan, Particle::MeshType type)
{
	for (int i = 0; i < num; i++)
	{
		VECTOR Pos = pos;
		int offsetX = random(-1.5f, 1.5f);
		int offsetY = random(-1.0f, 1.0f);
		int offsetZ = random(-1.5f, 1.5f);

		Pos.x += offsetX;
		Pos.y += offsetY;
		Pos.z += offsetZ;

		float advSpeed = random(0.1f, 10.0f);
		float lifeSpan = random(0.1f,maxLifeSpan);
		auto particle = new Particle(GetGame(), Pos, lifeSpan);
		particle->SetMeshType(type);
		particle->SetMesh(name);
		particle->SetAdvSpeed(advSpeed);
		particle->SetRotation(VECTOR(0.17f * (int)random(0.0f,20.0f), 0.17f * (int)random(0.0f, 20.0f), 0.17f * (int)random(0.0f, 20.0f)));
	}
}

