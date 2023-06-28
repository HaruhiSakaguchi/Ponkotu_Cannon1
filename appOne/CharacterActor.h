#pragma once
#include "Actor.h"
#include "MATRIX.h"
#include <string>
#include "Particle.h"

class CharacterActor :public Actor
{
public:
	CharacterActor(class Game* game);
	virtual ~CharacterActor();
	void Update()override;
	virtual void Damage(int damage = 1) {};
	const VECTOR& GetCapsulOffset() { return mCapsulOffset; }
	void SetCapsulOffset(const VECTOR& offset) { mCapsulOffset = offset; }

	//�t�B�[���h�Ƃ̓����蔻��p�f�[�^
	float GetRadius() const { return mRadius; }
	float GetHeight() const { return mHeight; }
	void SetRadius(float rad) { mRadius = rad; }
	void SetHeight(float heigh) { mHeight = heigh; }

	virtual void SetJumpFlag(int flag) { mJumpFlag = flag; }
	virtual int GetJumpFlag() { return mJumpFlag; }
	virtual void SetJumpVel(float vel) { mJumpVel = vel; }
	virtual float GetJumpVel() { return mJumpVel; }
	virtual void SetAdvSpeed(float speed) { mAdvSpeed = speed; }
	virtual float GetAdvSpeed() { return mAdvSpeed; }
	virtual float GetGravity() { return mGravity; }
	virtual void SetGravity(float grav) { mGravity = grav; }

	float GetOffsetY() { return mOffsetY; }
	void SetHp(int hp) { mHp = hp; }
	int GetHp() { return mHp; }
	int GetMaxHp() { return mMaxHp; }
	void SetMaxHp(int hp) { mMaxHp = hp; }
	float GetSlant() const { return mSlant; }

	//���f����]�֐�
	int rotate(VECTOR* angle, const VECTOR& dir, float rotSpeed);

	float GetDamageInterval() { return mDamageInterval; }
	void SetDamageInterval(float interval) { mDamageInterval = interval; }
	const COLOR& GetDamageColor() { return mDamageColor; }

	//mDamageInterval > 0�̎��ɌĂяo���BDamage�ɉ����t�����������Ƃ��̂��́B
	virtual void DamageOption() {};
	//mPosition.y < -45�Ȃ�Ăяo���B
	virtual void FallOption() {};

	//�\������ۂ̓_�̐F
	const COLOR& GetImageColor() { return mImageColor; }
	void SetImageColor(const COLOR& color) { mImageColor = color; }

	std::string GetName() { return mName; }
	void SetName(const char* name) { mName = name; }
	int GetLevel() { return mLevel; }
	void SetLevel(int level) { mLevel = level; }
	int GetMaxLevel() { return mMaxLevel; }
	bool GetOnMapFlag() { return mOnMapFlag; }
	int GetInitMaxHp() { return mInitMaxHp; }
	void SetInitMaxHp(int hp) { mInitMaxHp = hp; }

	void SpawnParticle(const VECTOR& pos, const char* name,int num = 1, float maxLifeSpan = 1.0f, Particle::MeshType type = Particle::MeshType::EBatch);

	enum CharactersTag
	{
		Cannon,
		Satellite,
		Tama,
		Barricade,
		PHome,
		EHome,
		Waepon,
		Others
	};

	CharactersTag GetTag() { return mTag; }
	void SetTag(CharactersTag tag) { mTag = tag; }

private:
	VECTOR mCapsulOffset;
	float mHeight;
	float mRadius;
	float mSlant;
	int  mJumpFlag;
	float mJumpVel;
	float mGravity;
	float mAdvSpeed;
	float mDamageInterval;
	float mOffsetY;
	int mLevel;
	int mMaxLevel;
	COLOR mDamageColor;
	//�~�j�}�b�v�p�J���[�@�A�N�^�[���ƂɎ�������
	COLOR mImageColor;
	//�}�b�v�G���A���ɂ��邩
	bool mOnMapFlag;
	int mHp;
	int mMaxHp;
	int mInitMaxHp;
	std::string mName;
	CharactersTag mTag;
protected:
	int mDeadSound;
private:
	//�J�v�Z���̌`�󂪋��łȂ��ꍇ�̓����蔻��Ɏg�p��������N���X
public:
	class SEGMENT
	{
	private:
		VECTOR sp;
		VECTOR ep;
		VECTOR v;
		class CharacterActor* mOwner;
	public:
		SEGMENT(class CharacterActor* owner)
			: mOwner(owner)
			, sp(0, 0, 0)
			, ep(0, 0, 0)
			, v(0, 0, 0)
		{
		}
		void update()
		{
			MATRIX gWorld;
			gWorld.identity();
			VECTOR pos = mOwner->GetPosition() + mOwner->GetCapsulOffset();

			gWorld.mulRotateZ(mOwner->GetRotation().z);
			gWorld.mulRotateX(-mOwner->GetRotation().x);

			gWorld.mulRotateY(mOwner->GetRotation().y + 3.1415926f / 2);

			sp = pos + gWorld * mOwner->GetHeight() / 2;
			ep = pos + gWorld * -mOwner->GetHeight() / 2;

			v = ep - sp;
		}
		const VECTOR& GetSp() const { return sp; }
		const VECTOR& GetEp() const { return ep; }
		const VECTOR& GetV() const { return v; }
	};
private:
	class SEGMENT* mSeg;
public:
	SEGMENT* GetSeg() { return mSeg; }
	void SetSeg(SEGMENT* seg) { mSeg = seg; }
};