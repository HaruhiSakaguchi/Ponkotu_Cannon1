#include "UIItemStatus.h"
#include "Game.h"
#include "graphic.h"
#include "Items.h"

UIItemStatus::UIItemStatus(class Cannon* owner)
	:UIScreen(owner->GetGame())
	, mOwner(owner)
{
	Data = mGame->GetAllData()->itemStatusData;
}

void UIItemStatus::Draw()
{
	for (auto item : mOwner->GetItemComponents())
	{
		if (item->GetInterval() != 0)
		{
			DrawTimeGauge(item);
		}
	}
}

void UIItemStatus::Update()
{
	if (!mGame->GetCannon())
	{
		CloseMe();
	}
}

void UIItemStatus::DrawRing(const VECTOR2& pos, float radius, float sw, const COLOR& color)
{
	for (int i = 0; i < 90; i++)
	{
		float px = cosf((float)i * 360.0f * 2.0f) * radius;
		float py = -sinf((float)i * 360.0f * 2.0f) * radius;
		fill(color);
		noStroke();
		circle(pos.x + px, pos.y + py, sw);
	}
}

void UIItemStatus::DrawRingOnLine(const VECTOR2& pos, float radius, float dia, float angle, const COLOR& color)
{
	colorMode(RGB);
	VECTOR2 centerPos = pos;
	VECTOR2 lineEdgePos = VECTOR2(centerPos.x + cosf(angle) * radius, centerPos.y - sinf(angle) * radius);
	strokeWeight(Data.mLineSw);
	stroke(color);
	line(lineEdgePos.x - cosf(angle) * dia / 2.0f, lineEdgePos.y + sinf(angle) * dia / 2.0f, lineEdgePos.x + cosf(angle) * dia / 2.0f, lineEdgePos.y + -sinf(angle) * dia / 2.0f);
}

void UIItemStatus::DrawRing(const VECTOR2& pos, float radius, float sw, float angle, const COLOR& color, float angleOffset)
{
	VECTOR2 centerPos = pos;
	float defAngle = 3.14159264f  / 180.0f* angleOffset;
	for (int i = 0; i < 360; i++)
	{
		float preAngle = angle;
		float Angle = (float)i * angle;

		Data.mAngle = defAngle + preAngle + (Angle - preAngle);

		float px = centerPos.x + cosf(Data.mAngle) * radius;
		float py = centerPos.y - sinf(Data.mAngle) * radius;
		stroke(color);
		strokeWeight(Data.mLineSw);
		line(px - cosf(Data.mAngle) * sw / 2.0f, py + sinf(Data.mAngle) * sw / 2.0f, px + cosf(Data.mAngle) * sw / 2.0f, py - sinf(Data.mAngle) * sw / 2.0f);
	}
}

void UIItemStatus::DrawTimeGauge(ItemComponent* item)
{
	VECTOR2 Pos = item->GetPosition();

	DrawRightTriangle(Pos, Data.mTriangleWidth, Data.mTriangleHeight, Data.mTriangleAngle120, Data.mTriangleColor);
	DrawRightTriangle(VECTOR2(Pos.x + Data.mWidth, Pos.y), Data.mTriangleWidth, Data.mHeight, Data.mTriangleAngle30, Data.mTriangleColor, "â∫");

	DrawRightTriangle(VECTOR2(Pos.x - Data.mWhiteTriangleOffsetPosX, Pos.y), Data.mTriangleWidth, Data.mHeight, Data.mTriangleAngle120,Data.mTriangleColor);
	DrawRightTriangle(VECTOR2(Pos.x + Data.mWidth + Data.mWhiteTriangleOffsetPosX, Pos.y), Data.mTriangleWidth, Data.mHeight, Data.mTriangleAngle30, Data.mTriangleColor, "â∫");


	noStroke();
	fill(Data.mTriangleColor);
	rect(Pos.x, Pos.y, Data.mWidth, Data.mHeight);

	DrawRightTriangle(Pos, Data.mWhiteTriangleOffsetPosX, Data.mItemNameTextSize, Data.mTriangleAngle120, Data.mWhiteTriangleColor);
	DrawRightTriangle(VECTOR2(Pos.x + Data.mWidth + Data.mItemNameTextSize + Data.mWhiteTriangleOffsetPosX, Pos.y), Data.mWhiteTriangleOffsetPosX, Data.mItemNameTextSize, Data.mTriangleAngle30, Data.mWhiteTriangleColor, "â∫");

	fill(Data.mWhiteTriangleColor);
	rect(Pos.x, Pos.y, Data.mWidth + Data.mItemNameTextSize + Data.mWhiteTriangleOffsetPosX, Data.mItemNameTextSize);

	VECTOR2 OffsetPos = VECTOR2(Data.mWidth / 2.0f, Data.mHeight / 2.0f + Data.mItemNameTextSize / 2.0f);
	DrawRing(Pos + OffsetPos, Data.mRadius, Data.mDiamiter, Data.mWhiteTriangleColor);

	for (int j = 0; j < 12; j++)
	{
		DrawRingOnLine(Pos + OffsetPos, Data.mRadius, Data.mDiamiter, j * 3.141592f / 6.0f, Data.mRingOnLineColor);
	}

	float preTime = item->GetKeepTime();

	float Time = preTime + (item->GetTime() - preTime) * Data.mChangeTimeSpeed;

	item->SetKeepTime(Time);

	fill(item->GetColor());
	circle(Pos.x + OffsetPos.x, Pos.y + OffsetPos.y, Data.mRadius * 2.0f - Data.mDiamiter);
	DrawRing(Pos + OffsetPos, Data.mRadius, Data.mDiamiter, (3.14159264f / 180.0f) * item->GetKeepTime() / item->GetInterval(), Data.mTimeGaugeColor, Data.mAngleOffset);
	textSize(Data.mItemNameTextSize);
	fill(Data.mItemNameTextColor);
	if (item->GetLevel() != item->GetMaxLevel())
	{
		text(item->GetName().c_str() + (let)" Lv." + (let)item->GetLevel(), Pos.x + Data.mWidth / 2.0f - (item->GetName().length() + 2) / 4.0f * Data.mItemNameTextSize, Pos.y + Data.mItemNameTextSize);
	}
	else
	{
		text(item->GetName().c_str() + (let)" Lv.Max", Pos.x + Data.mWidth / 2.0f - (item->GetName().length() + 4) / 4.0f * Data.mItemNameTextSize, Pos.y + Data.mItemNameTextSize);
	}

	if (item == mOwner->GetBarrier())
	{
		fill(Data.mBarrierHpGaugeColor);
		noStroke();

		float preWidth = Data.mRectWidth;
		float WIDTH = Data.mBarrierHpGaugeWidth * mOwner->GetBarrier()->GetHp() / mOwner->GetBarrier()->GetMaxHp();

		Data.mRectWidth = preWidth + (WIDTH - preWidth) * Data.mChangeWidthSpeed;

		rectMode(CORNER);
		rect(Pos.x + Data.mWidth / 2.0f - Data.mBarrierHpGaugeWidth / 2.0f, Pos.y + Data.mBarrierHpGaugeOffsetPosY, Data.mRectWidth, Data.mBarrierHpGaugeHeight);
	}

}

void UIItemStatus::DrawRightTriangle(const VECTOR2& pos, float width, float height, float angle, const COLOR& color, const char* vec)
{
	if (vec == "è„")
	{
		for (int i = 0; i < height; i++)
		{
			stroke(color);
			strokeWeight(Data.mLineSw);
			line(pos.x - ((width) / height) * i, pos.y + i, pos.x, pos.y + i);
		}
	}
	if (vec == "â∫")
	{

		for (int i = 0; i < height; i++)
		{
			stroke(color);
			strokeWeight(Data.mLineSw);
			line(pos.x, pos.y + i, pos.x + width - width / height * i, pos.y + i);
		}

	}
	if (vec == "ê^ÇÒíÜç∂")
	{
		for (int i = 0; i < height; i++)
		{
			stroke(color);
			strokeWeight(Data.mLineSw);
			line(pos.x + width - (width / height) * width * sinf(i * 3.141592f / height), pos.y + i, pos.x + width, pos.y + i);
		}
	}
	if (vec == "ê^ÇÒíÜâE")
	{
		for (int i = 0; i < height; i++)
		{
			stroke(color);
			strokeWeight(Data.mLineSw);
			line(pos.x, pos.y + i, pos.x + (width / height) * width * sinf(i * 3.141592f / height), pos.y + i);
		}
	}

}