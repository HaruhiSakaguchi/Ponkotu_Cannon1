#include "UIScope.h"
#include "Game.h"
#include "input.h"

UIScope::UIScope(Game* game, Cannon* owner)
	:UIScreen(game)
	, mOwner(owner)
{
	Data = mGame->GetAllData()->scopeData;
}

void UIScope::Update()
{
	VECTOR Pos = mOwner->GetTargetPosition();
	float x = Pos.x / Pos.z * 10.0f;
	float y = Pos.y / Pos.z * 10.0f;

	VECTOR2 vec(x, y);
	vec.normalize();

	VECTOR2 scopePos(mouseX + Data.mOffsetPos.x + sinf(mGame->GetCamera()->GetRotation().x) * Data.mMouseOffsetPosX, mouseY + Data.mMouseOffsetPosY + Data.mOffsetPos.y);

	VECTOR2 prePos = Data.mPos;
	Data.mPos = prePos + (scopePos - prePos) * Data.mChangePositionSpeed;

	float preAngle = Data.mCylinderAngle;
	Data.mCylinderAngle = preAngle + (3.1415926f * 2.0f / Data.mMaxItenNum * (-mOwner->GetCnt()) - preAngle) * Data.mChangeAngleSpeed;
}

void UIScope::Draw()
{
	if (mGame->GetScene() == Game::EPlay && mGame->GetState() != Game::EPaused)
	{
		stroke(Data.mCrossColor);
		strokeWeight(Data.mCrossSw);

		line(Data.mPos.x, Data.mPos.y - Data.mCrossPosOffset, Data.mPos.x, Data.mPos.y + Data.mCrossPosOffset);
		line(Data.mPos.x - Data.mCrossPosOffset, Data.mPos.y, Data.mPos.x + Data.mCrossPosOffset, Data.mPos.y);
		DrawRing(Data.mPos, Data.mGaugeRingRadius, Data.mGaugeRingWeight, Data.mWhiteRingColor);
		DrawRing(Data.mPos, Data.mGaugeRingRadius, Data.mGaugeRingWeight, 3.1415926f / 180.0f * mOwner->GetTimer() / mOwner->GetInterval(), Data.mGaugeRingColor);

		for (int i = 0; i < 8; i++)
		{
			DrawRingOnLine(Data.mPos, Data.mGaugeRingRadius, Data.mGaugeRingWeight, (float)(i * 3.14159264f / 4.0f), Data.mRingOnLineColor);
		}

		for (int i = 0; i < (int)mOwner->GetItemNums().size(); i++)
		{
			strokeWeight(Data.mStockItemWeight);
			stroke(Data.mColors[mOwner->GetItemNums()[i]]);
			point(Data.mPos.x + cosf(3.1415926f * 2.0f / Data.mMaxItenNum * i + Data.mCylinderAngle) * Data.mItemCylinderRadius, Data.mPos.y + -sinf(3.1415926f * 2.0f / Data.mMaxItenNum * i + Data.mCylinderAngle) * Data.mItemCylinderRadius);
		}

		rectMode(CORNER);
		strokeWeight(Data.mItemNameWindowSw);
		textSize(Data.mItemNameTextSize);
		stroke(Data.mItemNameWindowStrokeColor);
		fill(Data.mItemNameWindowColor);
		rect(Data.mPos.x + Data.mStockItemOffsetX, Data.mPos.y - Data.mItemNameTextSize / 2.0f, (int)strlen(Data.mStrings[mOwner->GetItemNums()[mOwner->GetCnt()]]) * Data.mItemNameTextSize / 2.0f, Data.mItemNameTextSize);
		fill(Data.mItemNameColor);
		text(Data.mStrings[mOwner->GetItemNums()[mOwner->GetCnt()]], Data.mPos.x + Data.mStockItemOffsetX, Data.mPos.y + Data.mItemNameTextSize / 2.0f);

		DrawRing(Data.mPos + VECTOR2(Data.mItemCylinderRadius,0.0f), Data.mItemNameTextSize / 2.0f, Data.mItemNameWindowSw, Data.mGaugeRingColor);

	}
}

void UIScope::DrawRing(const VECTOR2& pos, float radius, float sw, const COLOR& color)
{
	for (int i = 0; i < 90; i++)
	{
		float px = cosf((float)i * 2.0f * 360.0f) * radius;
		float py = -sinf((float)i * 2.0f * 360.0f) * radius;
		fill(color);
		noStroke();
		circle(pos.x + px, pos.y + py, sw);
	}
}

void UIScope::DrawRingOnLine(const VECTOR2& pos, float radius, float dia, float angle, const COLOR& color)
{
	colorMode(RGB);
	VECTOR2 centerPos = pos;
	VECTOR2 lineEdgePos = VECTOR2(centerPos.x + cosf(angle) * radius, centerPos.y - sinf(angle) * radius);
	strokeWeight(1);
	stroke(color);
	line(lineEdgePos.x - cosf(angle) * dia / 2.0f, lineEdgePos.y + sinf(angle) * dia / 2.0f, lineEdgePos.x + cosf(angle) * dia / 2, lineEdgePos.y + -sinf(angle) * dia / 2.0f);
}

void UIScope::DrawRing(const VECTOR2& pos, float radius, float sw, float angle, const COLOR& color)
{
	VECTOR2 centerPos = pos;
	for (int i = 0; i < 360; i++)
	{
		float px = centerPos.x + cosf((float)i * angle) * radius;
		float py = centerPos.y + -sinf((float)i * angle) * radius;
		stroke(color);
		strokeWeight(1);
		line(px - cosf((float)i * angle) * sw / 2.0f, py + sinf((float)i * angle) * sw / 2.0f, px + cosf((float)i * angle) * sw / 2, py - sinf((float)i * angle) * sw / 2.0f);
	}
}