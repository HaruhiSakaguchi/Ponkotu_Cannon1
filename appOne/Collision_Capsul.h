#pragma once
#include "VECTOR.h"
#include "Segment.h"
#include "CharacterActor.h"
#include "MATRIX.h"

float calcPointLineDist //�ŒZ����
(
    const VECTOR& p, //�_ 
    const VECTOR& sp,//�n�_
    const VECTOR& ep,//�I�_
    VECTOR& mp, //�_p���璼���ɉ��낵�������̒[�_
    float& t //s.v�̒�����1�Ƃ������́us.sp����mp�܂ł̒����v�̊���
);

float calcLineLineDist
(
    const VECTOR*& s1,//�����P
    const VECTOR*& s2,//�����Q
    VECTOR& mp1, //�ŒZ���̒[�_�P
    VECTOR& mp2, //�ŒZ���̒[�_�Q
    float& t1, //s1.v�̒�����1�Ƃ������́us1.sp����mp1�܂ł̒����v�̊���
    float& t2 //s2.v�̒�����1�Ƃ������́us2.sp����mp2�܂ł̒����v�̊���
);

//0�`1�̊ԂɃN�����v(�l�������I�ɂ���͈͓��ɂ��邱��)
void clamp0to1(float& v);

//2�����Ԃ̍ŒZ����
float calcSegmentSegmentDist
(
    const VECTOR* s1,//�����P
    const VECTOR* s2,//�����Q
    VECTOR& mp1, //�ŒZ���̒[�_1(�n�_��I�_�ɂȂ邱�Ƃ�����)
    VECTOR& mp2, //�ŒZ���̒[�_2(�n�_��I�_�ɂȂ邱�Ƃ�����)
    float& t1, //s1.v�̒�����1�Ƃ������́us1.sp����mp1�܂ł̒����v�̊���
    float& t2 //s2.v�̒�����1�Ƃ������́us2.sp����mp2�܂ł̒����v�̊���
);

bool CollisionCircle(float radius1, float radius2, const VECTOR& pos, const VECTOR& pos2);

bool Intersect(CharacterActor* a, CharacterActor* b, bool flag = true);

