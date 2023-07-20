#include "CapsuleComponent.h"
#include "CharacterActor.h"
#include "mathUtil.h"
#include "Game.h"
#include "SEGMENT.h"
#include "ActorManager.h"

CapsuleComponent::CapsuleComponent(CharacterActor* owner)
	: SpriteComponent(owner)
	, mAmbient(0.0f)
	, mNumCAng(24)
	, mNumCVtx(0)
	, mCOp(nullptr)
	, mBright(nullptr)
	, mSegP(nullptr)
	, mSOp(nullptr)
	, mSegRadius(0.0005f)
	, mNumSAng(4)
	, mNumSVtx(0)
	, mSeg(nullptr)
	, mPos(0.0f, 0.0f, 0.0f)
	, mIsCollision(true)
{
	SetUp();
	createSegment();
	createCapsule();
	mOwner->GetGame()->GetActorManager()->AddCapsule(this);
}

CapsuleComponent::~CapsuleComponent()
{
	delete[] mCOp;
	delete[] mBright;
	delete[] mSOp;
	delete[] mSegP;
	delete[] mCapP;
	delete mSeg;

	while (!mNotCollisionTags.empty())
	{
		mNotCollisionTags.pop_back();
	}

	mOwner->GetGame()->GetActorManager()->RemoveCapsule(this);
}

int CapsuleComponent::SetUp()
{
	mNumCVtx = mNumCAng * (mNumCAng / 2) + 2;
	mNumSVtx = mNumSAng * 2;
	mCOp = new VECTOR[mNumCVtx];
	mBright = new float[mNumCVtx];
	mSegP = new VECTOR[mNumSVtx];
	mSOp = new VECTOR[mNumSVtx];
	mCapP = new VECTOR[mNumCVtx];
	mSeg = new SEGMENT;
	mLightPos = mOwner->GetGame()->GetAllData()->rendererData.mLightPos;
	mAmbient = mOwner->GetGame()->GetAllData()->rendererData.mAmbient;
	return 1;
}

void CapsuleComponent::Update()
{
	auto owner = static_cast<CharacterActor*>(mOwner);
	mPos = owner->GetPosition() + owner->GetCapsulOffset();
	VECTOR osp = VECTOR(0.0f, 0.0f, 0.0f);
	VECTOR oep = VECTOR(0.0f, 0.0f, 0.0f);

	if (mOEp.x == 0.0f && mOEp.y == 0.0f && mOEp.z == 0.0f)
	{
		mOEp = oep;
	}
	if (mOSp.x == 0.0f && mOSp.y == 0.0f && mOSp.z == 0.0f)
	{
		mOSp = osp;
	}

	mSeg->update(mPos, owner->GetRotation(), mOSp, mOEp);

	for (auto capsule : mOwner->GetGame()->GetActorManager()->GetCapsules())
	{
		auto capOwner = static_cast<CharacterActor*>(capsule->GetOwner());

		bool isNotCollision = false;

		for (auto tags : mNotCollisionTags)
		{
			if (static_cast<CharacterActor::CharactersTag>(tags) == capOwner->GetTag())
			{
				isNotCollision = true;
			}
		}

		if (capsule == this || isNotCollision || capOwner->GetHp() <= 0)
		{
			continue;
		}


		VECTOR mp1;
		VECTOR mp2;
		float t1;
		float t2;
		float dist = 0;

		dist = calcSegmentSegmentDist(mSeg, capsule->GetSegment(), mp1, mp2, t1, t2);
		if (mIsCollision && capsule->IsCollision() && dist < owner->GetRadius() + static_cast<CharacterActor*>(capsule->GetOwner())->GetRadius())
		{
			capsule->GetOwner()->SetPosition(capsule->GetOwner()->GetPosition() + (mp2 - mp1).normalize() * 0.05f);
			if (capsule->GetOwner()->GetPosition().y < 0.0f && capsule->GetOwner()->PositionOnMap(capsule->GetOwner()->GetPosition(), static_cast<CharacterActor*>(capsule->GetOwner())->GetRadius()))
			{
				capsule->GetOwner()->SetPosition(capsule->GetOwner()->GetPosition().x, 0.0f, capsule->GetOwner()->GetPosition().z);
			}
		}
	}
}

void CapsuleComponent::Draw()
{
	auto owner = static_cast<CharacterActor*>(mOwner);

	segment(mSeg->GetSP(), mSeg->GetEP(), COLOR(255, 255, 255), 100);
	capsule(mPos, owner->GetRotation(), mCapsuleColor, mOSp.y, mOEp.y, owner->GetRadius());
}

bool CapsuleComponent::OverlapSegment(SEGMENT* s1, SEGMENT* s2, float radius1, float radius2)
{
	VECTOR mp1;
	VECTOR mp2;
	float t1;
	float t2;
	float dist = 0;

	dist = calcSegmentSegmentDist(s1, s2, mp1, mp2, t1, t2);

	if (dist < radius1 + radius2)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool CapsuleComponent::OverlapActor(class CharacterActor* a1, class CharacterActor* a2)
{
	auto seg1 = a1->GetCapsule()->GetSegment();
	auto seg2 = a2->GetCapsule()->GetSegment();

	VECTOR a1Pos = a1->GetPosition() + a1->GetCapsulOffset();
	VECTOR a2Pos = a2->GetPosition() + a2->GetCapsulOffset();

	seg1->update(a1Pos, a1->GetRotation(), a1->GetCapsule()->GetOSp(), a1->GetCapsule()->GetOEp());
	seg2->update(a2Pos, a2->GetRotation(), a2->GetCapsule()->GetOSp(), a2->GetCapsule()->GetOEp());

	return OverlapSegment(seg1, seg2, a1->GetRadius(), a2->GetRadius());
}

bool CapsuleComponent::LineTraceHitActor(class SEGMENT* seg, CharacterActor** Actor, VECTOR* hitPos)
{
	auto owner = static_cast<CharacterActor*>(mOwner);
	VECTOR pos = owner->GetPosition() + owner->GetCapsulOffset();
	seg->update(pos, owner->GetRotation());

	bool flag = false;

	for (auto chara : owner->GetGame()->GetActorManager()->GetCharacters())
	{
		if (chara == owner || (!chara->GetCapsule() || chara->GetCategory() == CharacterActor::Object))
		{
			continue;
		}

		auto Cseg = chara->GetCapsule()->GetSegment();
		Cseg->update(chara->GetPosition() + chara->GetCapsulOffset(), chara->GetRotation(), chara->GetCapsule()->GetOSp(), chara->GetCapsule()->GetOEp());
		VECTOR mp1;
		VECTOR mp2;
		float t1;
		float t2;
		float dist = 0;

		dist = calcSegmentSegmentDist(Cseg, seg, mp1, mp2, t1, t2);

		if ((0.1f + chara->GetRadius()) > dist)
		{
			seg->SetEP(seg->GetSP() + seg->GetV() * t2);
			flag = true;
			*Actor = chara;
			*hitPos = seg->GetEP();
			break;
		}
	}

	return flag;
}

void CapsuleComponent::createSegment()
{
	//���_����E�֐L�т�p�������
	float angle = 3.1415926f / (mNumSAng / 2);
	for (int j = 0; j < 2; j++) {
		for (int i = 0; i < mNumSAng; i++) {
			mSOp[mNumSAng * j + i].x = mOpX[j];
			mSOp[mNumSAng * j + i].y = Cos(angle * i) * mSegRadius;
			mSOp[mNumSAng * j + i].z = Sin(angle * i) * mSegRadius;
		}
	}
}

void CapsuleComponent::segment(const VECTOR& sp, const VECTOR& ep, const COLOR& col, float thickness)
{
	VECTOR* p = new VECTOR[mNumSVtx];
	VECTOR v = ep - sp;
	float len = v.mag();
	float angleY = Atan2(-v.z, v.x);
	v.normalize();
	float angleZ = Acos(-v.y) - 1.57f;
	gWorld.identity();
	gWorld.mulTranslate(sp.x, sp.y, sp.z);
	gWorld.mulRotateY(angleY);
	gWorld.mulRotateZ(angleZ);
	gWorld.mulScaling(len, thickness, thickness);
	for (int i = 0; i < mNumSVtx; i++) {
		p[i] = gWorld * mSOp[i];
		p[i] = mOwner->GetGame()->GetRenderer()->GetView() * p[i];
		p[i] = mOwner->GetGame()->GetRenderer()->GetProj() * p[i];
	}
	//�`��
	for (int i = 0; i < mNumSAng; i++) {
		//���ʂł���l�p�`
		int a = i;
		int b = i + mNumSAng;
		int c = (i + 1) % mNumSAng == 0 ? i + 1 - mNumSAng : i + 1;
		int d = (b + 1) % mNumSAng == 0 ? b + 1 - mNumSAng : b + 1;
		triangle3D(p[a], p[b], p[c], col, col, col);
		triangle3D(p[c], p[b], p[d], col, col, col);
	}

	delete[]p;
}

void CapsuleComponent::createCapsule()
{
	VECTOR light = mLightPos;
	light.normalize();
	float radius = 0.2f;
	float angle = 3.1415926f / (mNumCAng / 2);
	int j, k = 0;
	//�k�ɂɂȂ钸�_
	mCOp[k].set(0, 1, 0);
	mBright[k] = dot(light, mCOp[k]);
	if (mBright[k] < mAmbient)mBright[k] = mAmbient;
	k++;
	//�㔼��
	for (j = 0; j < (mNumCAng / 4); j++) {
		float y = Cos(angle * (j + 1));
		float r = Sin(angle * (j + 1));
		//�~�̍��W
		for (int i = 0; i < mNumCAng; i++) {
			mCOp[k].set(Cos(angle * i) * r, y, Sin(angle * i) * r);
			//op[k]��@���Ƃ��Ē��_�̖��邳���v�Z
			mBright[k] = dot(light, mCOp[k]);
			if (mBright[k] < mAmbient)mBright[k] = mAmbient;
			k++;
		}
	}
	//������
	for (j -= 1; j < (mNumCAng / 2) - 1; j++) {
		float y = Cos(angle * (j + 1));
		float r = Sin(angle * (j + 1));
		//�~�̍��W
		for (int i = 0; i < mNumCAng; i++) {
			mCOp[k].set(Cos(angle * i) * r, y, Sin(angle * i) * r);
			//op[k]��@���Ƃ��Ē��_�̖��邳���v�Z
			mBright[k] = dot(light, mCOp[k]);
			if (mBright[k] < mAmbient)mBright[k] = mAmbient;
			k++;
		}
	}
	//��ɂɂȂ钸�_
	mCOp[k].set(0, -1, 0);
	mBright[k] = dot(light, mCOp[k]);
	if (mBright[k] < mAmbient)mBright[k] = mAmbient;
}
void CapsuleComponent::capsule(const VECTOR& t, const VECTOR& r,
	const COLOR& col, float segStartPos, float segEndPos, float radius)
{
	//�㔼��
	MATRIX world;
	world.identity();
	world.mulTranslate(t);
	world.mulRotateYXZ(r);
	world.mulTranslate(0, segStartPos, 0);
	world.mulScaling(radius, radius, radius);
	for (int i = 0; i < mNumCVtx / 2; i++) {
		mCapP[i] = world * mCOp[i];
		mCapP[i] = mOwner->GetGame()->GetRenderer()->GetView() * mCapP[i];
		mCapP[i] = mOwner->GetGame()->GetRenderer()->GetProj() * mCapP[i];
	}
	//������
	world.identity();
	world.mulTranslate(t);
	world.mulRotateYXZ(r);
	world.mulTranslate(0, segEndPos, 0);
	world.mulScaling(radius, radius, radius);
	for (int i = mNumCVtx / 2; i < mNumCVtx; i++) {
		mCapP[i] = world * mCOp[i];
		mCapP[i] = mOwner->GetGame()->GetRenderer()->GetView() * mCapP[i];
		mCapP[i] = mOwner->GetGame()->GetRenderer()->GetProj() * mCapP[i];
	}
	//�`��
	//��W�I�Ȗʂ͎O�p�`�ŕ`��
	for (int i = 0; i < mNumCAng; i++) {
		int a = 0;
		int b = i + 1;
		int c = (i + 1) % mNumCAng == 0 ? i + 2 - mNumCAng : i + 2;
		VECTOR m = mCapP[b] - mCapP[a];
		VECTOR n = mCapP[c] - mCapP[a];
		//if (m.x * n.y - m.y * n.x <= 0)
		{
			triangle3D(mCapP[a], mCapP[b], mCapP[c], col * mBright[a], col * mBright[b], col * mBright[c]);
			//	print("p(" + (let)p[a].x + "," + (let)p[a].y + "," + (let)p[a].z + ")");
		}
	}
	//���ʓI�Ȗʂ͎l�p�`�ŕ`��
	for (int i = 0; i < mNumCVtx - mNumCAng - 2; i++) {
		int a = i + 1;
		int b = a + mNumCAng;
		int c = (i + 1) % mNumCAng == 0 ? b + 1 - mNumCAng : b + 1;
		int d = (i + 1) % mNumCAng == 0 ? a + 1 - mNumCAng : a + 1;
		VECTOR m = mCapP[b] - mCapP[a];
		VECTOR n = mCapP[c] - mCapP[a];
		//if (m.x * n.y - m.y * n.x <= 0)
		//{
		triangle3D(mCapP[a], mCapP[b], mCapP[c], col * mBright[a], col * mBright[b], col * mBright[c]);
		triangle3D(mCapP[a], mCapP[c], mCapP[d], col * mBright[a], col * mBright[c], col * mBright[d]);
		//print("p(" + (let)p[a].x + "," + (let)p[a].y + "," + (let)p[a].z + ")");
	//}
	}
	////���W�I�Ȗʂ͎O�p�`�ŕ`��
	for (int i = mNumCVtx - mNumCAng - 1; i < mNumCVtx - 1; i++) {
		int a = mNumCVtx - 1;
		int b = i;
		int c = i % mNumCAng == 0 ? i + 1 - mNumCAng : i + 1;
		VECTOR m = mCapP[b] - mCapP[a];
		VECTOR n = mCapP[c] - mCapP[a];
		//if (m.x * n.y - m.y * n.x >= 0)
		{
			triangle3D(mCapP[a], mCapP[b], mCapP[c], col * mBright[a], col * mBright[b], col * mBright[c]);
		}
	}

	/*for (int i = 0; i < mNumCVtx - 1; i++)
	{
		print("cop[" + (let)i + "](" + (let)mCOp[i].x + "," + (let)mCOp[i].y + "," + (let)mCOp[i].z + ")");
	}*/
	//print("Chimpo");
}

//�_�ƒ����̍ŒZ����
float CapsuleComponent::calcPointLineDist //�ŒZ����
(
	const VECTOR& p, //�_ 
	SEGMENT* s, //�����i���������j
	VECTOR& mp, //�_p���璼���ɉ��낵�������̒[�_
	float& t //s.v�̒�����1�Ƃ������́us.sp����mp�܂ł̒����v�̊���
)
{
	t = 0.0f;
	float dvv = s->GetV().magSq();//dot(s.v, s.v)�Ɠ���
	if (dvv > 0.0f) {
		t = dot(s->GetV(), p - s->GetSP()) / dvv;
		// ��̎��̐���
		// dot(s.v, p-s.sp) �� |s.v||p-s.sp|cos��
		// dvv��|s.v|�̂Q��
		// ��̌v�Z�ŁAt�� |p-s.sp|cos�� / |s.v|�ƂȂ�B
		// �܂�t�́udot�œ��e����������s.v�̒����v�Ƃ��������ɂȂ�
	}
	mp = s->GetSP() + s->GetV() * t;
	return (p - mp).mag();
}

//�_�Ɛ����̍ŒZ����
float CapsuleComponent::calcPointSegmentDist//�ŒZ����
(
	const VECTOR& p, //�_
	SEGMENT* s, //����
	VECTOR& mp, //�_p��������܂ł̍ŒZ�����ƂȂ�_(�n�_��I�_�ɂȂ邱�Ƃ�����)
	float& t //s.v�̒�����1�Ƃ������́us.sp����mp�܂ł̒����v�̊���
)
{
	//�Ƃ肠�����_���璼���܂ł̍ŒZ������mp,t�����߂Ă݂�
	float dist = calcPointLineDist(p, s, mp, t);
	//mp�������̊O�ɂ���(�n�_���)
	if (t < 0.0f) {
		//�_��������̎n�_�܂ł̋������ŒZ
		mp = s->GetSP();
		return (p - mp).mag();
	}
	//mp�������̊O�ɂ���(�I�_���)
	if (t > 1.0f) {
		//�_��������̏I�_�܂ł̋������ŒZ
		mp = s->GetEP();
		return(p - mp).mag();
	}
	//mp���������ɂ���
	//0 <= t <= 1
	return dist;
}

//�����ƒ����̍ŒZ����
float CapsuleComponent::calcLineLineDist
(
	SEGMENT* s1,//�����P
	SEGMENT* s2,//�����Q
	VECTOR& mp1, //�ŒZ���̒[�_�P
	VECTOR& mp2, //�ŒZ���̒[�_�Q
	float& t1, //s1.v�̒�����1�Ƃ������́us1.sp����mp1�܂ł̒����v�̊���
	float& t2  //s2.v�̒�����1�Ƃ������́us2.sp����mp2�܂ł̒����v�̊���
)
{
	//2���������s
	if (cross(s1->GetV(), s2->GetV()).magSq() < 0.000001f) {
		//����1�̎n�_���璼��2�܂ł̍ŒZ�������ɋA������
		t1 = 0.0f;
		mp1 = s1->GetSP();
		float dist = calcPointLineDist(mp1, s2, mp2, t2);
		return dist;
	}

	float dv1v2 = dot(s1->GetV(), s2->GetV());
	float dv1v1 = s1->GetV().magSq();//dot(s1.v,s1.v)�Ɠ���
	float dv2v2 = s2->GetV().magSq();//dot(s2.v,s2.v)�Ɠ���
	VECTOR vp2p1 = s1->GetSP() - s2->GetSP();
	t1 = (dv1v2 * dot(s2->GetV(), vp2p1) - dv2v2 * dot(s1->GetV(), vp2p1))
		/ (dv1v1 * dv2v2 - dv1v2 * dv1v2);
	mp1 = s1->GetSP() + s1->GetV() * t1;
	t2 = dot(s2->GetV(), mp1 - s2->GetSP()) / dv2v2;
	mp2 = s2->GetSP() + s2->GetV() * t2;
	return (mp2 - mp1).mag();
}

//2�����Ԃ̍ŒZ����
float CapsuleComponent::calcSegmentSegmentDist
(
	SEGMENT* s1,//����1
	SEGMENT* s2,//����2
	VECTOR& mp1, //�ŒZ���̒[�_1(�n�_��I�_�ɂȂ邱�Ƃ�����)
	VECTOR& mp2, //�ŒZ���̒[�_2(�n�_��I�_�ɂȂ邱�Ƃ�����)
	float& t1, //s1.v�̒�����1�Ƃ������́us1.sp����mp1�܂ł̒����v�̊���
	float& t2  //s2.v�̒�����1�Ƃ������́us2.sp����mp2�܂ł̒����v�̊���
)
{
	float dist = 0;
	{
		// s1.v���k�ނ��Ă���H
		if (s1->GetV().magSq() < 0.000001f) {
			// s2.v���k�ށH
			if (s2->GetV().magSq() < 0.000001f) {
				// �_�Ɠ_�̋����̖��ɋA��
				dist = (s2->GetSP() - s1->GetSP()).mag();
				mp1 = s1->GetSP();
				mp2 = s2->GetSP();
				t1 = t2 = 0.0f;
				//segment(mp1, mp2, COLOR(255, 255, 255), 10);
				return dist;
			}
			else {
				// s1.sp��s2.v�̍ŒZ���ɋA��
				dist = calcPointSegmentDist(s1->GetSP(), s2, mp2, t2);
				mp1 = s1->GetSP();
				t1 = 0.0f;
				clamp0to1(t2);
				//segment(mp1, mp2, COLOR(255, 255, 255), 10);
				return dist;
			}
		}
		// s2.v���k�ނ��Ă���H
		else if (s2->GetV().magSq() < 0.000001f) {
			// s2.sp��sv1�̍ŒZ���ɋA��
			float dist = calcPointSegmentDist(s2->GetSP(), s1, mp1, t1);
			mp2 = s2->GetSP();
			clamp0to1(t1);
			t2 = 0.0f;
			//segment(mp1, mp2, COLOR(255, 255, 255), 10);
			return dist;
		}
	}

	//----------------------------------------------------------------
	//�Ƃ肠����2�����Ԃ̍ŒZ����,mp1,mp2,t1,t2�����߂Ă݂�
	dist = calcLineLineDist(s1, s2, mp1, mp2, t1, t2);
	if (0.0f <= t1 && t1 <= 1.0f &&
		0.0f <= t2 && t2 <= 1.0f) {
		//mp1,mp2�������Ƃ��������ɂ�����
		return dist;
	}
	//mp1,mp2�̗����A�܂��͂ǂ��炩���������ɂȂ������̂Ŏ���

	//----------------------------------------------------------------
	//mp1,t1�����ߒ��� �� t2��0�`1�ɃN�����v����mp2����s1.v�ɐ������~�낵�Ă݂�
	clamp0to1(t2);
	mp2 = s2->GetSP() + s2->GetV() * t2;
	dist = calcPointLineDist(mp2, s1, mp1, t1);
	if (0.0f <= t1 && t1 <= 1.0f) {
		//mp1���������ɂ�����
		return dist;
	}
	//mp1���������ɂȂ������̂Ŏ���

	//----------------------------------------------------------------
	//mp2,t2�����ߒ��� �� t1��0�`1�ɃN�����v����mp1����s2.v�ɐ������~�낵�Ă݂�
	clamp0to1(t1);
	mp1 = s1->GetSP() + s1->GetV() * t1;
	dist = calcPointLineDist(mp1, s2, mp2, t2);
	if (0.0f <= t2 && t2 <= 1.0f) {
		//mp2���������ɂ�����
		return dist;
	}
	//mp2���������ɂȂ������̂Ŏ���

	//----------------------------------------------------------------
	//t2���N�����v����mp2���Čv�Z����ƁAmp1����mp2�܂ł��ŒZ
	clamp0to1(t2);
	mp2 = s2->GetSP() + s2->GetV() * t2;
	return (mp2 - mp1).mag();
}

//�⏕���o�[�W����
COLOR Yellow(255, 255, 0);
COLOR Red(255, 0, 0);
COLOR Green(0, 255, 0);
COLOR Cyan(0, 255, 255);
float CapsuleComponent::calcSegmentSegmentDist_
(
	SEGMENT* s1,//����1
	SEGMENT* s2,//����2
	VECTOR& mp1, //�ŒZ���̒[�_1(�n�_��I�_�ɂȂ邱�Ƃ�����)
	VECTOR& mp2, //�ŒZ���̒[�_2(�n�_��I�_�ɂȂ邱�Ƃ�����)
	float& t1, //s1.v�̒�����1�Ƃ������́us1.sp����mp1�܂ł̒����v�̊���
	float& t2 //s2.v�̒�����1�Ƃ������́us2.sp����mp2�܂ł̒����v�̊���
)
{
	//�����Ɍ����邽�߂̕⏕��
	segment(s1->GetSP() - s1->GetV() * 3, s1->GetEP() + s1->GetV() * 3, COLOR(90, 90, 90), 2);
	segment(s2->GetSP() - s2->GetV() * 3, s2->GetEP() + s2->GetV() * 3, COLOR(90, 90, 90), 2);

	float dist = 0;

	//------------------------------------------------------------------------
	//check1:�Ƃ肠����2�����Ԃ̍ŒZ����,mp1,mp2,t1,t2�����߂Ă݂�
	dist = calcLineLineDist(s1, s2, mp1, mp2, t1, t2);
	fill(Yellow);
	if (0.0f <= t1 && t1 <= 1.0f &&
		0.0f <= t2 && t2 <= 1.0f) {
		//mp1,mp2�������Ƃ��������ɂ�����
		print((let)"check1�Z�Ft1=" + t1 + " t2=" + t2);
		segment(mp1, mp2, Yellow, 10);
		point(mp1, Yellow, 30);
		point(mp2, Yellow, 30);
		return dist;
		//return 0;

	}
	print((let)"check1�~�Ft1=" + t1 + " t2=" + t2);
	segment(mp1, mp2, Yellow, 2);
	point(mp2, Yellow, 12);
	point(mp1, Yellow, 12);
	//mp1,mp2�̗����A�܂��͂ǂ��炩���������ɂȂ������̂Ŏ���

	//------------------------------------------------------------------------
	//check2:mp1,t1�����ߒ��� �� t2��0�`1�ɃN�����v����mp2����s1.v�ɐ������~�낵�Ă݂�
	clamp0to1(t2);
	mp2 = s2->GetSP() + s2->GetV() * t2;
	dist = calcPointLineDist(mp2, s1, mp1, t1);
	fill(Red);
	if (0.0f <= t1 && t1 <= 1.0f) {
		//mp1���������ɂ�����
		print((let)"check2�Z�Ft1=" + t1 + " t2=" + t2);
		segment(mp1, mp2, Red, 10);
		point(mp1, Red, 30);
		return dist;
		//return 0;

	}
	print((let)"check2�~�Ft1=" + t1 + " t2=" + t2);
	segment(mp1, mp2, Red, 2);
	point(mp1, Red, 12);
	//mp1���������ɂȂ������̂Ŏ���

	//------------------------------------------------------------------------
	//check3:mp2,t2�����ߒ��� �� t1��0�`1�ɃN�����v����mp1����s2.v�ɐ������~�낵�Ă݂�
	clamp0to1(t1);
	mp1 = s1->GetSP() + s1->GetV() * t1;
	dist = calcPointLineDist(mp1, s2, mp2, t2);
	fill(Green);
	if (0.0f <= t2 && t2 <= 1.0f) {
		//mp2���������ɂ�����
		print((let)"check3�Z�Ft1=" + t1 + " t2=" + t2);
		segment(mp1, mp2, Green, 10);
		point(mp2, Green, 30);
		return dist;
		//return 0;
	}
	print((let)"check3�~�Ft1=" + t1 + " t2=" + t2);
	segment(mp1, mp2, Green, 2);
	point(mp2, Green, 12);
	//mp2���������ɂȂ������̂Ŏ���

	//-----------------------------------------------------------------------
	clamp0to1(t2);
	mp2 = s2->GetSP() + s2->GetV() * t2;
	fill(Cyan);
	print((let)"last  �@�Ft1=" + t1 + " t2=" + t2);
	segment(mp1, mp2, COLOR(Cyan), 10);
	//�����܂ŗ�����Amp1����mp2�܂ł��ŒZ
	return (mp2 - mp1).mag();
}
