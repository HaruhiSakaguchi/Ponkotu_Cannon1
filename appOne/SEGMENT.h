#pragma once
#include "VECTOR.h"
#include "MATRIX.h"

class SEGMENT {
public:
	SEGMENT(const VECTOR& osp = VECTOR(0.0f, 0.0f, 0.0f), const VECTOR& oep = VECTOR(0.0f, 0.0f, 0.0f))
		: mOsp(osp)
		, mOep(oep)
	{};

	const VECTOR GetSP() { return mSp; }
	const VECTOR GetEP() { return mEp; }
	const VECTOR GetV() { return mV; }
	void SetSP(const VECTOR& pos) { mSp = pos; }
	void SetEP(const VECTOR& pos) { mEp = pos; }
	void SetOSP(const VECTOR& pos) { mOsp = pos; }
	void SetOEP(const VECTOR& pos) { mOep = pos; }

	void update(const VECTOR& tran, const VECTOR& rot)
	{
		gWorld.identity();
		gWorld.mulTranslate(tran);
		gWorld.mulRotateYXZ(rot);
		mSp = gWorld * mOsp;
		mEp = gWorld * mOep;
		mV = mEp - mSp;
	}

	void update(const VECTOR& tran, const VECTOR& rot,
		const VECTOR& osp, const VECTOR& oep)
	{
		gWorld.identity();
		gWorld.mulTranslate(tran);
		gWorld.mulRotateYXZ(rot);
		mSp = gWorld * osp;
		mEp = gWorld * oep;
		mV = mEp - mSp;
	}

private:
	VECTOR mSp;//�n�_
	VECTOR mEp;//�I�_
	VECTOR mOsp;
	VECTOR mOep;
	VECTOR mV;//�n�_����I�_�܂ł̃x�N�g��
	MATRIX gWorld;

};