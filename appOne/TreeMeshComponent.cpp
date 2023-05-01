#include "TreeMeshComponent.h"
#include "Actor.h"
#include "Game.h"
#include "Renderer.h"
#include "SHADER/SHADER.h"
#include "CONTAINER/CONTAINER.h"
#include "CONTAINER/TREE.h"
#include "CONTAINER/ANIMATION.h"

TreeMeshComponent::TreeMeshComponent(Actor* owner)
	:MeshComponent(owner)
	, mTree(nullptr)
	, mNormalTree(nullptr)
	, mDamageTree(nullptr)
	, mDrawFlag(true)
	, mPos(0.0f,0.0f,0.0f)
{
}

TreeMeshComponent::~TreeMeshComponent()
{
	delete mNormalTree;
	delete mDamageTree;
	mAnims.clear();
}

void TreeMeshComponent::SetTree(const char* name)
{
	CONTAINER* c = mOwner->GetGame()->GetRenderer()->GetContainer();
	mNormalTree = new TREE(c->treeOrigin(name));
}

void TreeMeshComponent::SetDamageTree(const char* name)
{
	CONTAINER* c = mOwner->GetGame()->GetRenderer()->GetContainer();
	mDamageTree = new TREE(c->treeOrigin(name));
}

void TreeMeshComponent::SetAnim(const char* name)
{
	CONTAINER* c = mOwner->GetGame()->GetRenderer()->GetContainer();
	mAnims.emplace_back(c->animation(name));
}

void TreeMeshComponent::SetAnimId(int id)
{
	mTree->setAnimation(mAnims[id]);
}

void TreeMeshComponent::SetNextAnimId(int id, float morphFrame, float startFrame)
{
	mTree->setNextAnimation(mAnims[id], morphFrame, startFrame);
}

void TreeMeshComponent::Update()
{
	mPos = mOwner->GetPosition();
	mWorld.identity();
	mWorld.mulTranslate(mPos + mOffsetPos);
	mWorld.mulRotateY(mOwner->GetRotation().y);
	mWorld.mulRotateX(mOwner->GetRotation().x);
	mWorld.mulRotateZ(mOwner->GetRotation().z);
	mWorld.mulRotateY(mOffsetAngle.y);
	mWorld.mulRotateX(mOffsetAngle.x);
	mWorld.mulRotateZ(mOffsetAngle.z);
	mWorld.mulScaling(mOwner->GetScale());

	if (static_cast<CharacterActor*>(mOwner)->GetDamageInterval() > 0.0f && mDamageTree != nullptr)
	{
		mTree = mDamageTree;
	}
	else
	{
		mTree = mNormalTree;
	}

	mTree->update(mWorld);
}

void TreeMeshComponent::Draw()
{
	if (mDrawFlag && mTree != nullptr)
	{
		mTree->draw(mOwner->GetGame()->GetRenderer()->GetShader());
	}
}
