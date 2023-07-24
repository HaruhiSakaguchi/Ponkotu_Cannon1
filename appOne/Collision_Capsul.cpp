#include "Collision_Capsul.h"
#include "window.h"
#include "COLLISION_MAP.h"

//点と直線の最短距離
float calcPointLineDist //最短距離
(
	const VECTOR& p, //点 
	const VECTOR& sp,//始点
	const VECTOR& ep,//終点
	VECTOR& mp, //点pから直線に下ろした垂線の端点
	float& t//s.vの長さを1とした時の「s.spからmpまでの長さ」の割合
)
{
	VECTOR v = ep - sp;
	t = 0.0f;
	float dvv = v.magSq();//dot(s.v, s.v)と同じ
	if (dvv > 0.0f) {
		t = dot(v, p - sp) / dvv;
		// 上の式の説明
		// dot(s.v, p-s.sp) は |s.v||p-s.sp|cosΘ
		// dvvは|s.v|の２乗
		// 上の計算で、tは |p-s.sp|cosΘ / |s.v|となる。
		// つまりtは「dotで投影した長さ÷s.vの長さ」という割合になる
	}
	mp = sp + v * t;
	return (p - mp).mag();
}

//点と線分の最短距離
float calcPointSegmentDist//最短距離
(
	const VECTOR& p, //点
	const VECTOR*& s, //線分
	VECTOR& mp, //点pから線分までの最短距離となる点(始点や終点になることもある)
	float& t//s.vの長さを1とした時の「s.spからmpまでの長さ」の割合
)
{
	VECTOR sp = s[0];
	VECTOR ep = s[1];

	VECTOR v = ep - sp;
	//とりあえず点から直線までの最短距離やmp,tを求めてみる
	float dist = calcPointLineDist(p, sp, ep, mp, t);
	//mpが線分の外にある(始点寄り)
	if (t < 0.0f) {
		//点から線分の始点までの距離が最短
		mp = sp;
		return (p - mp).mag();
	}
	//mpが線分の外にある(終点寄り)
	if (t > 1.0f) {
		//点から線分の終点までの距離が最短
		mp = ep;
		return(p - mp).mag();
	}
	//mpが線分内にある
	//0 <= t <= 1
	return dist;
}

//直線と直線の最短距離
float calcLineLineDist
(
	const VECTOR*& s1,//直線１
	const VECTOR*& s2,//直線２
	VECTOR& mp1, //最短線の端点１
	VECTOR& mp2, //最短線の端点２
	float& t1, //s1.vの長さを1とした時の「s1.spからmp1までの長さ」の割合
	float& t2 //s2.vの長さを1とした時の「s2.spからmp2までの長さ」の割合
)
{
	VECTOR sp1 = s1[0];
	VECTOR ep1 = s1[1];

	VECTOR sp2 = s2[0];
	VECTOR ep2 = s2[1];

	VECTOR v1 = ep1 - sp1;
	VECTOR v2 = ep2 - sp2;

	//2直線が平行
	if (cross(v1, v2).magSq() < 0.000001f) {
		//線分1の始点から直線2までの最短距離問題に帰着する
		t1 = 0.0f;
		mp1 = sp1;
		float dist = calcPointLineDist(mp1, sp2, ep2, mp2, t2);
		return dist;
	}

	//2直線が平行でない
	//互いに垂直になるような最短線の端点mp1,mp2を求める
	//次の順で求めていく t1 -> mp1 -> t2 -> mp2
	//最初のt1を求める式の詳細は下のコメントにある
	/*
	両直線の最短距離を結ぶ線は、両直線に共通の垂線となる。。。その垂線の端点mp1,mp2
	mp1 = s1.sp + s1.v * t1
	mp2 = s2.sp + s2.v * t2

	t2を求める式
	t2 = dot(s2.v,mp1 - s2.sp) / dot(s2.v,s2.v)
	最終的にt1を求めたいので、t2をt1で表現するためmp1を置き換える
	t2 = dot(s2.v, s1.sp + s1.v * t1 - s2.sp) / dot(s2.v, s2.v)
	分配してt1を計算しやすくしておく
	t2 = dot(s2.v, s1.sp - s2.sp + s1.v * t1) / dot(s2.v, s2.v)
	t2 = { dot(s2.v, s1.sp - s2.sp) + dot(s2.v, s1.v) * t1 } / dot(s2.v, s2.v)


	次の式からt1を導く。バカ丁寧に変形していきます。
	0 = dot(s1.v,mp1-mp2)
	=dot(s1.v,(s1.sp+s1.v*t1)-(s2.sp+s2.v*t2))
	=dot(s1.v, s1.sp-s2.sp + s1.v*t1 - s2.v*t2)
	分配してt2を外に出す
	=dot(s1.v, s1.sp-s2.sp) + dot(s1.v,s1.v)*t1 - dot(s1.v,s2.v)*t2 )
	t2を消す
	=dot(s1.v, s1.sp-s2.sp) + dot(s1.v,s1.v)*t1 - dot(s1.v,s2.v)*{ dot(s2.v, s1.sp - s2.sp) + dot(s2.v, s1.v) * t1 } / dot(s2.v, s2.v)
	一番右の分母を消す（dot(s2.v, s2.v)を掛ける）
	=dot(s2.v,s2.v)*dot(s1.v, s1.sp-s2.sp) + dot(s2.v,s2.v)*dot(s1.v,s1.v)*t1 - dot(s1.v,s2.v) * {dot(s2.v, s1.sp - s2.sp) + dot(s2.v, s1.v) * t1}
	中かっこを消す
	=dot(s2.v,s2.v)*dot(s1.v, s1.sp-s2.sp) + dot(s2.v,s2.v)*dot(s1.v,s1.v)*t1 - dot(s1.v,s2.v) * dot(s2.v, s1.sp - s2.sp) - dot(s1.v,s2.v) * dot(s2.v,s1.v) * t1
	t1でくくる
	=dot(s2.v,s2.v)*dot(s1.v, s1.sp-s2.sp)  - dot(s1.v,s2.v) * dot(s2.v, s1.sp - s2.sp) + { dot(s2.v,s2.v)*dot(s1.v,s1.v) - dot(s1.v,s2.v) * dot(s2.v,s1.v) }* t1
	移項
	-{dot(s2.v,s2.v)*dot(s1.v,s1.v) - dot(s1.v,s2.v) * dot(s2.v,s1.v)}*t1 = dot(s2.v,s2.v)*dot(s1.v, s1.sp-s2.sp)  - dot(s1.v,s2.v) * dot(s2.v, s1.sp - s2.sp)
	t1=にして、整理していく
	t1 = dot(s2.v,s2.v)*dot(s1.v, s1.sp-s2.sp)  - dot(s1.v,s2.v) * dot(s2.v, s1.sp - s2.sp) / - {dot(s2.v,s2.v)*dot(s1.v,s1.v) - dot(s1.v,s2.v) * dot(s2.v,s1.v)}
	t1 = -dot(s2.v,s2.v)*dot(s1.v, s1.sp-s2.sp)  + dot(s1.v,s2.v) * dot(s2.v, s1.sp - s2.sp) / dot(s2.v,s2.v)*dot(s1.v,s1.v) - dot(s1.v,s2.v) * dot(s2.v,s1.v)
	t1 = dot(s1.v,s2.v) * dot(s2.v, s1.sp - s2.sp) -　dot(s2.v,s2.v) * dot(s1.v, s1.sp-s2.sp)  / dot(s1.v,s1.v) * dot(s2.v,s2.v) - dot(s1.v,s2.v) * dot(s1.v,s2.v)
	*/
	float dv1v2 = dot(v1, v2);
	float dv1v1 = v1.magSq();//dot(s1.v,s1.v)と同じ
	float dv2v2 = v2.magSq();//dot(s2.v,s2.v)と同じ
	VECTOR vp2p1 = sp1 - sp2;
	t1 = (dv1v2 * dot(v2, vp2p1) - dv2v2 * dot(v1, vp2p1))
		/ (dv1v1 * dv2v2 - dv1v2 * dv1v2);
	mp1 = sp1 + v1 * t1;
	t2 = dot(v2, mp1 - sp2) / dv2v2;
	mp2 = sp2 + v2 * t2;
	return (mp2 - mp1).mag();
}

//0～1の間にクランプ(値を強制的にある範囲内にすること)
void clamp0to1(float& v) {
	if (v < 0.0f)  v = 0.0f;
	else if (v > 1.0f)  v = 1.0f;
}

//2線分間の最短距離
float calcSegmentSegmentDist
(
	const VECTOR* s1,//線分1
	const VECTOR* s2,//線分2
	VECTOR& mp1, //最短線の端点1(始点や終点になることもある)
	VECTOR& mp2, //最短線の端点2(始点や終点になることもある)
	float& t1, //s1.vの長さを1とした時の「s1.spからmp1までの長さ」の割合
	float& t2  //s2.vの長さを1とした時の「s2.spからmp2までの長さ」の割合
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
		// s1.vが縮退している？
		if (v1.magSq() < 0.000001f) {
			// s2.vも縮退？
			if (v2.magSq() < 0.000001f) {
				// 点と点の距離の問題に帰着
				dist = (sp2 - sp1).mag();
				mp1 = sp1;
				mp2 = sp2;
				t1 = t2 = 0.0f;
				return dist;
			}
			else {
				// s1.spとs2.vの最短問題に帰着
				dist = calcPointSegmentDist(sp1, s2, mp2, t2);
				mp1 = sp1;
				t1 = 0.0f;
				clamp0to1(t2);
				return dist;
			}
		}
		// s2.vが縮退している？
		else if (v2.magSq() < 0.000001f) {
			// s2.spとsv1の最短問題に帰着
			float dist = calcPointSegmentDist(sp2, s1, mp1, t1);
			mp2 = sp2;
			clamp0to1(t1);
			t2 = 0.0f;
			return dist;
		}
	}

	//----------------------------------------------------------------
	//とりあえず2直線間の最短距離,mp1,mp2,t1,t2を求めてみる
	dist = calcLineLineDist(s1, s2, mp1, mp2, t1, t2);
	if (0.0f <= t1 && t1 <= 1.0f &&
		0.0f <= t2 && t2 <= 1.0f) {
		//mp1,mp2が両方とも線分内にあった
		return dist;
	}
	//mp1,mp2の両方、またはどちらかが線分内になかったので次へ

	//----------------------------------------------------------------
	//mp1,t1を求め直す ⇒ t2を0～1にクランプしてmp2からs1.vに垂線を降ろしてみる
	clamp0to1(t2);
	mp2 = sp2 + v2 * t2;
	dist = calcPointLineDist(mp2, sp1, ep1, mp1, t1);
	if (0.0f <= t1 && t1 <= 1.0f) {
		//mp1が線分内にあった
		return dist;
	}
	//mp1が線分内になかったので次へ

	//----------------------------------------------------------------
	//mp2,t2を求め直す ⇒ t1を0～1にクランプしてmp1からs2.vに垂線を降ろしてみる
	clamp0to1(t1);
	mp1 = sp1 + v1 * t1;
	dist = calcPointLineDist(mp1, sp2, ep2, mp2, t2);
	if (0.0f <= t2 && t2 <= 1.0f) {
		//mp2が線分内にあった
		return dist;
	}
	//mp2が線分内になかったので次へ

	//----------------------------------------------------------------
	//t2をクランプしてmp2を再計算すると、mp1からmp2までが最短
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


