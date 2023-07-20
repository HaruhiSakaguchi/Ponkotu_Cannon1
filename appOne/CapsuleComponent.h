#pragma once
#include "SpriteComponent.h"
#include "VECTOR.h"
#include "graphic.h"
#include "MATRIX.h"
#include <vector>

class CapsuleComponent :
	public SpriteComponent
{
public:
	CapsuleComponent(class CharacterActor* owner);
	~CapsuleComponent();
	int SetUp();
	void Update()override;
	void Draw()override;
	bool OverlapSegment(class SEGMENT* s1, class SEGMENT* s2, float radius1, float radius2);
	bool OverlapActor(class CharacterActor* a1, class CharacterActor* a2);

	bool LineTraceHitActor(class SEGMENT* seg, CharacterActor** Actor, VECTOR* hitPos);
	class SEGMENT* GetSegment() { return mSeg; }
	//�f�o�b�O�p�`��֐�
	void createCapsule();
	void capsule(const VECTOR& t, const VECTOR& r, const COLOR& col, float segStartPos, float segEndPos, float radius);
	void createSegment();
	void segment(const VECTOR& sp, const VECTOR& ep, const COLOR& col, float thickness = 1);

	void SetSpandEp(const VECTOR& sp, const VECTOR& ep) { mOSp = sp; mOEp = ep; }
	void SetIsCollision(bool flag) { mIsCollision = flag; }
	bool IsCollision() { return mIsCollision; }
	void AddNotCollisionTags(int tag) { mNotCollisionTags.emplace_back(tag); }

	const VECTOR& GetOSp() { return mOSp; }
	const VECTOR& GetOEp() { return mOEp; }
	//�_�ƒ����̍ŒZ����
	float calcPointLineDist //�ŒZ����
	(
		const VECTOR& p, //�_ 
		class SEGMENT* s, //�����i���������j
		VECTOR& mp, //�_p���璼���ɉ��낵�������̒[�_
		float& t //s.v�̒�����1�Ƃ������́us.sp����mp�܂ł̒����v�̊���
	);

	//�_�Ɛ����̍ŒZ����
	float calcPointSegmentDist//�ŒZ����
	(
		const VECTOR& p, //�_
		class SEGMENT* s, //����
		VECTOR& mp, //�_p��������܂ł̍ŒZ�����ƂȂ�_(�n�_��I�_�ɂȂ邱�Ƃ�����)
		float& t //s.v�̒�����1�Ƃ������́us.sp����mp�܂ł̒����v�̊���
	);

	//�����ƒ����̍ŒZ����
	float calcLineLineDist
	(
		class SEGMENT* s1,//�����P
		class SEGMENT* s2,//�����Q
		VECTOR& mp1, //�ŒZ���̒[�_�P
		VECTOR& mp2, //�ŒZ���̒[�_�Q
		float& t1, //s1.v�̒�����1�Ƃ������́us1.sp����mp1�܂ł̒����v�̊���
		float& t2  //s2.v�̒�����1�Ƃ������́us2.sp����mp2�܂ł̒����v�̊���
	);

	//0�`1�̊ԂɃN�����v(�l�������I�ɂ���͈͓��ɂ��邱��)
	void clamp0to1(float& v) {
		if (v < 0.0f)  v = 0.0f;
		else if (v > 1.0f)  v = 1.0f;
	};

	//2�����Ԃ̍ŒZ����
	float calcSegmentSegmentDist
	(
		class SEGMENT* s1,//����1
		class SEGMENT* s2,//����2
		VECTOR& mp1, //�ŒZ���̒[�_1(�n�_��I�_�ɂȂ邱�Ƃ�����)
		VECTOR& mp2, //�ŒZ���̒[�_2(�n�_��I�_�ɂȂ邱�Ƃ�����)
		float& t1, //s1.v�̒�����1�Ƃ������́us1.sp����mp1�܂ł̒����v�̊���
		float& t2  //s2.v�̒�����1�Ƃ������́us2.sp����mp2�܂ł̒����v�̊���
	);

	float calcSegmentSegmentDist_
	(
		class SEGMENT* s1,//����1
		class SEGMENT* s2,//����2
		VECTOR& mp1, //�ŒZ���̒[�_1(�n�_��I�_�ɂȂ邱�Ƃ�����)
		VECTOR& mp2, //�ŒZ���̒[�_2(�n�_��I�_�ɂȂ邱�Ƃ�����)
		float& t1, //s1.v�̒�����1�Ƃ������́us1.sp����mp1�܂ł̒����v�̊���
		float& t2 //s2.v�̒�����1�Ƃ������́us2.sp����mp2�܂ł̒����v�̊���
	);

private:
	MATRIX gWorld;
	VECTOR mLightPos;
	class SEGMENT* mSeg;
	VECTOR* mCOp;
	VECTOR* mSOp;
	VECTOR* mSegP;
	VECTOR* mCapP;
	VECTOR mPos;
	VECTOR mOSp;
	VECTOR mOEp;

	COLOR mCapsuleColor;
	float mAmbient;
	float* mBright;
	bool mIsCollision;
	int mNumCAng;
	int mNumCVtx;
	int mNumSAng;
	int mNumSVtx;
	float mSegRadius;
	float mOpX[2] = { 0.0f,1.0f };
	std::vector<int>mNotCollisionTags;
};

