#include "BatchMeshComponent.h"
#include "Actor.h"
#include "Game.h"
#include "Renderer.h"
#include "CONTAINER/CONTAINER.h"
#include "CONTAINER/BATCH.h"

BatchMeshComponent::BatchMeshComponent(Actor* owner,bool isDraw)
	: MeshComponent(owner,isDraw)
	, mBatch(nullptr)
{
}

BatchMeshComponent::~BatchMeshComponent()
{
}

void BatchMeshComponent::SetBatch(const char* name)
{
	CONTAINER* c = mOwner->GetGame()->GetRenderer()->GetContainer();
	mBatch = c->batch(name);
}

void BatchMeshComponent::Draw()
{
	mWorld.identity();
	mWorld.mulTranslate(mOwner->GetPosition() + mOffsetPos);
	mWorld.mulRotateY(mOwner->GetRotation().x + mOffsetAngle.x);
	mWorld.mulRotateX(mOwner->GetRotation().y + mOffsetAngle.y);
	mWorld.mulRotateZ(mOwner->GetRotation().z + mOffsetAngle.z);
	mWorld.mulRotateY(mOffsetAngle.y);
	mWorld.mulRotateX(mOffsetAngle.x);
	mWorld.mulRotateZ(mOffsetAngle.z);
	mWorld.mulScaling(mOwner->GetScale());
}

void BatchMeshComponent::Update()
{
	if (mDrawFlag && mBatch)
	{
		mBatch->draw(mOwner->GetGame()->GetRenderer()->GetShader(), mWorld);
	}
}
