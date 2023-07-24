#include "Collision_Capsul.h"
#include "window.h"
#include "COLLISION_MAP.h"

//�_�ƒ����̍ŒZ����
float calcPointLineDist //�ŒZ����
(
	const VECTOR& p, //�_ 
	const VECTOR& sp,//�n�_
	const VECTOR& ep,//�I�_
	VECTOR& mp, //�_p���璼���ɉ��낵�������̒[�_
	float& t//s.v�̒�����1�Ƃ������́us.sp����mp�܂ł̒����v�̊���
)
{
	VECTOR v = ep - sp;
	t = 0.0f;
	float dvv = v.magSq();//dot(s.v, s.v)�Ɠ���
	if (dvv > 0.0f) {
		t = dot(v, p - sp) / dvv;
		// ��̎��̐���
		// dot(s.v, p-s.sp) �� |s.v||p-s.sp|cos��
		// dvv��|s.v|�̂Q��
		// ��̌v�Z�ŁAt�� |p-s.sp|cos�� / |s.v|�ƂȂ�B
		// �܂�t�́udot�œ��e����������s.v�̒����v�Ƃ��������ɂȂ�
	}
	mp = sp + v * t;
	return (p - mp).mag();
}

//�_�Ɛ����̍ŒZ����
float calcPointSegmentDist//�ŒZ����
(
	const VECTOR& p, //�_
	const VECTOR*& s, //����
	VECTOR& mp, //�_p��������܂ł̍ŒZ�����ƂȂ�_(�n�_��I�_�ɂȂ邱�Ƃ�����)
	float& t//s.v�̒�����1�Ƃ������́us.sp����mp�܂ł̒����v�̊���
)
{
	VECTOR sp = s[0];
	VECTOR ep = s[1];

	VECTOR v = ep - sp;
	//�Ƃ肠�����_���璼���܂ł̍ŒZ������mp,t�����߂Ă݂�
	float dist = calcPointLineDist(p, sp, ep, mp, t);
	//mp�������̊O�ɂ���(�n�_���)
	if (t < 0.0f) {
		//�_��������̎n�_�܂ł̋������ŒZ
		mp = sp;
		return (p - mp).mag();
	}
	//mp�������̊O�ɂ���(�I�_���)
	if (t > 1.0f) {
		//�_��������̏I�_�܂ł̋������ŒZ
		mp = ep;
		return(p - mp).mag();
	}
	//mp���������ɂ���
	//0 <= t <= 1
	return dist;
}

//�����ƒ����̍ŒZ����
float calcLineLineDist
(
	const VECTOR*& s1,//�����P
	const VECTOR*& s2,//�����Q
	VECTOR& mp1, //�ŒZ���̒[�_�P
	VECTOR& mp2, //�ŒZ���̒[�_�Q
	float& t1, //s1.v�̒�����1�Ƃ������́us1.sp����mp1�܂ł̒����v�̊���
	float& t2 //s2.v�̒�����1�Ƃ������́us2.sp����mp2�܂ł̒����v�̊���
)
{
	VECTOR sp1 = s1[0];
	VECTOR ep1 = s1[1];

	VECTOR sp2 = s2[0];
	VECTOR ep2 = s2[1];

	VECTOR v1 = ep1 - sp1;
	VECTOR v2 = ep2 - sp2;

	//2���������s
	if (cross(v1, v2).magSq() < 0.000001f) {
		//����1�̎n�_���璼��2�܂ł̍ŒZ�������ɋA������
		t1 = 0.0f;
		mp1 = sp1;
		float dist = calcPointLineDist(mp1, sp2, ep2, mp2, t2);
		return dist;
	}

	//2���������s�łȂ�
	//�݂��ɐ����ɂȂ�悤�ȍŒZ���̒[�_mp1,mp2�����߂�
	//���̏��ŋ��߂Ă��� t1 -> mp1 -> t2 -> mp2
	//�ŏ���t1�����߂鎮�̏ڍׂ͉��̃R�����g�ɂ���
	/*
	�������̍ŒZ���������Ԑ��́A�������ɋ��ʂ̐����ƂȂ�B�B�B���̐����̒[�_mp1,mp2
	mp1 = s1.sp + s1.v * t1
	mp2 = s2.sp + s2.v * t2

	t2�����߂鎮
	t2 = dot(s2.v,mp1 - s2.sp) / dot(s2.v,s2.v)
	�ŏI�I��t1�����߂����̂ŁAt2��t1�ŕ\�����邽��mp1��u��������
	t2 = dot(s2.v, s1.sp + s1.v * t1 - s2.sp) / dot(s2.v, s2.v)
	���z����t1���v�Z���₷�����Ă���
	t2 = dot(s2.v, s1.sp - s2.sp + s1.v * t1) / dot(s2.v, s2.v)
	t2 = { dot(s2.v, s1.sp - s2.sp) + dot(s2.v, s1.v) * t1 } / dot(s2.v, s2.v)


	���̎�����t1�𓱂��B�o�J���J�ɕό`���Ă����܂��B
	0 = dot(s1.v,mp1-mp2)
	=dot(s1.v,(s1.sp+s1.v*t1)-(s2.sp+s2.v*t2))
	=dot(s1.v, s1.sp-s2.sp + s1.v*t1 - s2.v*t2)
	���z����t2���O�ɏo��
	=dot(s1.v, s1.sp-s2.sp) + dot(s1.v,s1.v)*t1 - dot(s1.v,s2.v)*t2 )
	t2������
	=dot(s1.v, s1.sp-s2.sp) + dot(s1.v,s1.v)*t1 - dot(s1.v,s2.v)*{ dot(s2.v, s1.sp - s2.sp) + dot(s2.v, s1.v) * t1 } / dot(s2.v, s2.v)
	��ԉE�̕���������idot(s2.v, s2.v)���|����j
	=dot(s2.v,s2.v)*dot(s1.v, s1.sp-s2.sp) + dot(s2.v,s2.v)*dot(s1.v,s1.v)*t1 - dot(s1.v,s2.v) * {dot(s2.v, s1.sp - s2.sp) + dot(s2.v, s1.v) * t1}
	��������������
	=dot(s2.v,s2.v)*dot(s1.v, s1.sp-s2.sp) + dot(s2.v,s2.v)*dot(s1.v,s1.v)*t1 - dot(s1.v,s2.v) * dot(s2.v, s1.sp - s2.sp) - dot(s1.v,s2.v) * dot(s2.v,s1.v) * t1
	t1�ł�����
	=dot(s2.v,s2.v)*dot(s1.v, s1.sp-s2.sp)  - dot(s1.v,s2.v) * dot(s2.v, s1.sp - s2.sp) + { dot(s2.v,s2.v)*dot(s1.v,s1.v) - dot(s1.v,s2.v) * dot(s2.v,s1.v) }* t1
	�ڍ�
	-{dot(s2.v,s2.v)*dot(s1.v,s1.v) - dot(s1.v,s2.v) * dot(s2.v,s1.v)}*t1 = dot(s2.v,s2.v)*dot(s1.v, s1.sp-s2.sp)  - dot(s1.v,s2.v) * dot(s2.v, s1.sp - s2.sp)
	t1=�ɂ��āA�������Ă���
	t1 = dot(s2.v,s2.v)*dot(s1.v, s1.sp-s2.sp)  - dot(s1.v,s2.v) * dot(s2.v, s1.sp - s2.sp) / - {dot(s2.v,s2.v)*dot(s1.v,s1.v) - dot(s1.v,s2.v) * dot(s2.v,s1.v)}
	t1 = -dot(s2.v,s2.v)*dot(s1.v, s1.sp-s2.sp)  + dot(s1.v,s2.v) * dot(s2.v, s1.sp - s2.sp) / dot(s2.v,s2.v)*dot(s1.v,s1.v) - dot(s1.v,s2.v) * dot(s2.v,s1.v)
	t1 = dot(s1.v,s2.v) * dot(s2.v, s1.sp - s2.sp) -�@dot(s2.v,s2.v) * dot(s1.v, s1.sp-s2.sp)  / dot(s1.v,s1.v) * dot(s2.v,s2.v) - dot(s1.v,s2.v) * dot(s1.v,s2.v)
	*/
	float dv1v2 = dot(v1, v2);
	float dv1v1 = v1.magSq();//dot(s1.v,s1.v)�Ɠ���
	float dv2v2 = v2.magSq();//dot(s2.v,s2.v)�Ɠ���
	VECTOR vp2p1 = sp1 - sp2;
	t1 = (dv1v2 * dot(v2, vp2p1) - dv2v2 * dot(v1, vp2p1))
		/ (dv1v1 * dv2v2 - dv1v2 * dv1v2);
	mp1 = sp1 + v1 * t1;
	t2 = dot(v2, mp1 - sp2) / dv2v2;
	mp2 = sp2 + v2 * t2;
	return (mp2 - mp1).mag();
}

//0�`1�̊ԂɃN�����v(�l�������I�ɂ���͈͓��ɂ��邱��)
void clamp0to1(float& v) {
	if (v < 0.0f)  v = 0.0f;
	else if (v > 1.0f)  v = 1.0f;
}

//2�����Ԃ̍ŒZ����
float calcSegmentSegmentDist
(
	const VECTOR* s1,//����1
	const VECTOR* s2,//����2
	VECTOR& mp1, //�ŒZ���̒[�_1(�n�_��I�_�ɂȂ邱�Ƃ�����)
	VECTOR& mp2, //�ŒZ���̒[�_2(�n�_��I�_�ɂȂ邱�Ƃ�����)
	float& t1, //s1.v�̒�����1�Ƃ������́us1.sp����mp1�܂ł̒����v�̊���
	float& t2  //s2.v�̒�����1�Ƃ������́us2.sp����mp2�܂ł̒����v�̊���
)
{
	VECTOR sp1 = s1[0];
	VECTOR ep1 = s1[1];

	VECTOR sp2 = s2[0];
	VECTOR ep2 = s2[1];

	VECTOR v1 = ep1 - sp1;
	VECTOR v2 = ep2 - sp2;
	float dist = 0;

	{
		// s1.v���k�ނ��Ă���H
		if (v1.magSq() < 0.000001f) {
			// s2.v���k�ށH
			if (v2.magSq() < 0.000001f) {
				// �_�Ɠ_�̋����̖��ɋA��
				dist = (sp2 - sp1).mag();
				mp1 = sp1;
				mp2 = sp2;
				t1 = t2 = 0.0f;
				return dist;
			}
			else {
				// s1.sp��s2.v�̍ŒZ���ɋA��
				dist = calcPointSegmentDist(sp1, s2, mp2, t2);
				mp1 = sp1;
				t1 = 0.0f;
				clamp0to1(t2);
				return dist;
			}
		}
		// s2.v���k�ނ��Ă���H
		else if (v2.magSq() < 0.000001f) {
			// s2.sp��sv1�̍ŒZ���ɋA��
			float dist = calcPointSegmentDist(sp2, s1, mp1, t1);
			mp2 = sp2;
			clamp0to1(t1);
			t2 = 0.0f;
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
	mp2 = sp2 + v2 * t2;
	dist = calcPointLineDist(mp2, sp1, ep1, mp1, t1);
	if (0.0f <= t1 && t1 <= 1.0f) {
		//mp1���������ɂ�����
		return dist;
	}
	//mp1���������ɂȂ������̂Ŏ���

	//----------------------------------------------------------------
	//mp2,t2�����ߒ��� �� t1��0�`1�ɃN�����v����mp1����s2.v�ɐ������~�낵�Ă݂�
	clamp0to1(t1);
	mp1 = sp1 + v1 * t1;
	dist = calcPointLineDist(mp1, sp2, ep2, mp2, t2);
	if (0.0f <= t2 && t2 <= 1.0f) {
		//mp2���������ɂ�����
		return dist;
	}
	//mp2���������ɂȂ������̂Ŏ���

	//----------------------------------------------------------------
	//t2���N�����v����mp2���Čv�Z����ƁAmp1����mp2�܂ł��ŒZ
	clamp0to1(t2);
	mp2 = sp2 + v2 * t2;
	return (mp2 - mp1).mag();
}

bool CollisionCircle(float radius1, float radius2, const VECTOR& a, const VECTOR& b)
{
	float distx = b.x - a.x;
	float disty = b.y - a.y;
	float distz = b.z - a.z;

	if (radius1 + radius2 > sqrtf(distx * distx + disty * disty + distz * distz))
	{
		return true;
	}

	return false;
}

bool Intersect(CharacterActor* a, CharacterActor* b, bool flag)
{
	if (a->GetState() != Actor::State::EActive || b->GetState() != Actor::State::EActive)
	{
		return false;
	}
	else
	{
		float aRadius = a->GetRadius();
		float bRadius = b->GetRadius();
		float aHeight = a->GetHeight();
		float bHeight = b->GetHeight();

		VECTOR aPos = a->GetPosition() + a->GetCapsulOffset();
		VECTOR bPos = b->GetPosition() + b->GetCapsulOffset();
		VECTOR asp = VECTOR(aPos.x, aPos.y - aHeight / 2, aPos.z);
		VECTOR aep = VECTOR(aPos.x, aPos.y + aHeight / 2, aPos.z);
		VECTOR bsp = VECTOR(bPos.x, bPos.y - bHeight / 2, bPos.z);
		VECTOR bep = VECTOR(bPos.x, bPos.y + bHeight / 2, bPos.z);
		VECTOR mp1;
		VECTOR mp2;

		float t1;
		float t2;
		VECTOR s1[2] = { asp,aep };
		VECTOR s2[2] = { bsp,bep };

		float dist = abs(calcSegmentSegmentDist(s1, s2, mp1, mp2, t1, t2));

		float ABRadius = aRadius + bRadius;
		VECTOR abDist = mp2 - mp1;
		if (dist < ABRadius)
		{
			if (flag == true)
			{
				VECTOR apos = a->GetPosition();
				VECTOR bpos = b->GetPosition();

				float radA = aRadius + 0.1f;
				float radB = bRadius + 0.1f;
				
				VECTOR abvec = bpos - apos;
				abvec.normalize();

				if (b->GetGame()->GetActorManager()->GetStage()->GetCollisionMap()->capsule_triangles_walls(b) == 0 && b->GetHp() > 0 && a->GetHp() > 0)
				{
					VECTOR maxDist = abvec * (radA + radB);

					b->SetPosition(VECTOR(apos.x + maxDist.x, apos.y + maxDist.y, apos.z + maxDist.z));
				}
			}
			return true;
		}
	}

	return false;

}


