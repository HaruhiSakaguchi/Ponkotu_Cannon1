#include "HpGaugeSpriteComponent.h"
#include "Game.h"

HpGaugeSpriteComponent::HpGaugeSpriteComponent(Actor* owner, const VECTOR& offset)
	: MeshComponent(owner)
	, mMaxHp(static_cast<CharacterActor*>(owner)->GetHp())
	, mDivMaxHp((float)(1.0f / (float)(mMaxHp)) / 2)
	, mOffset(offset)
	, mGaugeWidth(0)
	, mGaugePosX(0)
	, mGauge(nullptr)
	, Master()
	, mNormal(nullptr)
	, mDanger(nullptr)
	, mDying(nullptr)
{
	CONTAINER* c = mOwner->GetGame()->GetRenderer()->GetContainer();
	mNormal = new TREE(c->treeOrigin("HpNormal"));
	mDanger = new TREE(c->treeOrigin("HpDanger"));
	mDying = new TREE(c->treeOrigin("HpDying"));
}

HpGaugeSpriteComponent::~HpGaugeSpriteComponent()
{
	SAFE_DELETE(mNormal);
	SAFE_DELETE(mDanger);
	SAFE_DELETE(mDying);
}

void HpGaugeSpriteComponent::Update()
{
	CharacterActor* owner = static_cast<CharacterActor*>(mOwner);
	VECTOR pos = owner->GetPosition() + mOffset + owner->GetCapsulOffset();
	float preWidth = mGaugeWidth;
	float Width = (float)(owner->GetHp()) / (float)owner->GetMaxHp();
	mGaugeWidth = preWidth + (Width - preWidth) * 0.05f;

	mDivMaxHp = ((float)(1.0f / (float)(owner->GetMaxHp())) / 2);

	float prePosX = mGaugePosX;
	float PosX = -mDivMaxHp * (owner->GetMaxHp() - owner->GetHp());
	mGaugePosX = prePosX + (PosX - prePosX) * 0.05f;

	Master.identity();

	Master.mulTranslate(pos.x, pos.y + owner->GetHeight() + 0.5f, pos.z);
	Master.mulTranslate(cosf(mOwner->GetGame()->GetCamera()->GetRotation().y) / owner->GetMaxHp(), 0.0f, sinf(mOwner->GetGame()->GetCamera()->GetRotation().y) / owner->GetMaxHp());
	Master.mulScaling(1.0f, 0.1f, 1.0f);
	Master.mulRotateY(mOwner->GetGame()->GetCamera()->GetRotation().y);
	Master.mulRotateX(3.141592f / 2);

	Master.mulTranslate(mGaugePosX, 0, 1.5f);
	Master.mulScaling(mGaugeWidth, 1, 1);

	if (owner->GetHp() >= owner->GetMaxHp() * 0.66f)
	{
		mGauge = mNormal;
	}
	else if (owner->GetHp() >= owner->GetMaxHp() * 0.33f)
	{
		mGauge = mDanger;
	}
	else
	{
		mGauge = mDying;
	}

	if (mGauge != nullptr)
	{
		mGauge->update(Master);
	}
}

void HpGaugeSpriteComponent::Draw()
{
	if (mGauge != nullptr)
	{
		mGauge->draw(GetOwner()->GetGame()->GetRenderer()->GetShader());
	}
}