#pragma once
#include "Actor.h"
#include "MATRIX.h"

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

	void SetHp(int hp) { mHp = hp; }
	int GetHp() { return mHp; }
	float GetSlant() const { return mSlant; }

	//���f����]�֐�
	int rotate(VECTOR* angle, const VECTOR& dir, float rotSpeed, int endOfRotation = 0);

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

	bool GetOnMapFlag() { return mOnMapFlag; }
private:
	//rotate()�p�f�[�^
	//��]�p�t���b�O
	static int mEndOfStateFlags;
public:
	void NoCollsionMap() { mCollisionMapFlag = false; }
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
	COLOR mDamageColor;
	//�~�j�}�b�v�p�J���[�@�A�N�^�[���ƂɎ�������
	COLOR mImageColor;
	//�}�b�v�G���A���ɂ��邩
	bool mOnMapFlag;
	bool mCollisionMapFlag;
	int mHp;
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