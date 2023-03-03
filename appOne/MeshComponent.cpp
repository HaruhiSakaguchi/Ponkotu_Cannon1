#include "MeshComponent.h"
#include "Actor.h"
#include "Game.h"
#include "Renderer.h"

MeshComponent::MeshComponent(Actor* owner)
	: Component(owner)
	, mOffsetPos(0.0f, 0.0f, 0.0f)
	, mOffsetAngle(0.0f,0.0f,0.0f)
{
	mOwner->GetGame()->GetRenderer()->AddMesh(this);
}

MeshComponent::~MeshComponent()
{
	mOwner->GetGame()->GetRenderer()->RemoveMesh(this);
}
