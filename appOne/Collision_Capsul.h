#pragma once
#include "VECTOR.h"
#include "Segment.h"
#include "CharacterActor.h"
#include "MATRIX.h"

float calcPointLineDist //最短距離
(
    const VECTOR& p, //点 
    const VECTOR& sp,//始点
    const VECTOR& ep,//終点
    VECTOR& mp, //点pから直線に下ろした垂線の端点
    float& t //s.vの長さを1とした時の「s.spからmpまでの長さ」の割合
);

float calcLineLineDist
(
    const VECTOR*& s1,//直線１
    const VECTOR*& s2,//直線２
    VECTOR& mp1, //最短線の端点１
    VECTOR& mp2, //最短線の端点２
    float& t1, //s1.vの長さを1とした時の「s1.spからmp1までの長さ」の割合
    float& t2 //s2.vの長さを1とした時の「s2.spからmp2までの長さ」の割合
);

//0～1の間にクランプ(値を強制的にある範囲内にすること)
void clamp0to1(float& v);

//2線分間の最短距離
float calcSegmentSegmentDist
(
    const VECTOR* s1,//線分１
    const VECTOR* s2,//線分２
    VECTOR& mp1, //最短線の端点1(始点や終点になることもある)
    VECTOR& mp2, //最短線の端点2(始点や終点になることもある)
    float& t1, //s1.vの長さを1とした時の「s1.spからmp1までの長さ」の割合
    float& t2 //s2.vの長さを1とした時の「s2.spからmp2までの長さ」の割合
);

bool CollisionCircle(float radius1, float radius2, const VECTOR& pos, const VECTOR& pos2);

bool Intersect(CharacterActor* a, CharacterActor* b, bool flag = true);

