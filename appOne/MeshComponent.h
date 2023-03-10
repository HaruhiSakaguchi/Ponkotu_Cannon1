#pragma once
#include "Component.h"
#include "MATRIX.h"

class MeshComponent :
	public Component
{
public:
	MeshComponent(class Actor*);
	virtual ~MeshComponent();
	virtual void Update() {};
	virtual void Draw() {};
	void SetOffsetPos(const VECTOR& pos) { mOffsetPos = pos; }
	void SetOffsetAngle(const VECTOR& angle) { mOffsetPos = angle; }

protected:
	MATRIX mWorld;
	VECTOR mOffsetPos;
	VECTOR mOffsetAngle;
};

