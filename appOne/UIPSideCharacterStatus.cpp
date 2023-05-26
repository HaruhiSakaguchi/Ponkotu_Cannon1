#include "UIPSideCharacterStatus.h"
#include "CharacterActor.h"
#include "Cannon.h"
#include "Game.h"
#include "graphic.h"
#include "UIPSideCharacterStatusClose.h"
#include <sstream>


UIPSideCharacterStatus::UIPSideCharacterStatus(class CharacterActor* owner)
	:UIScreen(owner->GetGame())
	, mOwner(owner)
	, mPos(0.0f, 0.0f)
	, mHpGaugeWidth(0.0f)
	, mCylinderAngle(0.0f)
	, mBarrierHpWidth(0.0f)
	, mStateCursorPos(width, 0.0f)
	, mStayButton(nullptr)
	, mReturnButton(nullptr)
	, mHomePatrollButton(nullptr)
	, mFieldPatrollButton(nullptr)
	, mCloseButton(nullptr)
{
	mPos = VECTOR2(0.0f, (mGame->GetAllData()->itemCompoData.mUIOffsetPosY + 75.0f)) +
		mGame->GetAllData()->itemCompoData.mUIInitPos;
	Data = mGame->GetAllData()->psData;


	AddButton("S",
		[this]() {
			class Cannon* c = static_cast<Cannon*>(mOwner);
			c->SetMoveState(Cannon::Stay);
		}
		, 2
			, nullptr
			//, mPos + VECTOR2( - 500.0f + 0.0f, 0.0f)
			, VECTOR2(mGame->GetAllData()->itemCompoData.mUIMinPosX - width / 2.0f + 50.0f, 215.0f)

			);

	mStayButton = GetButtons()[0];

	AddButton("R",
		[this]() {
			class Cannon* c = static_cast<Cannon*>(mOwner);
			c->SetMoveState(Cannon::Return);
		}
		, 2
			, nullptr
			, VECTOR2(mGame->GetAllData()->itemCompoData.mUIMinPosX - width / 2.0f + 100.0f, 215.0f - 69.0f)

			);

	mReturnButton = GetButtons()[1];

	AddButton("Ht",
		[this]() {
			class Cannon* c = static_cast<Cannon*>(mOwner);
			c->SetMoveState(Cannon::HomePatroll);
		}
		, 2
			, nullptr
			, VECTOR2(mGame->GetAllData()->itemCompoData.mUIMinPosX - width / 2.0f + 150.0f, 215.0f - 69.0f * 2)

			);
	mHomePatrollButton = GetButtons()[2];

	AddButton("F",
		[this]() {
			class Cannon* c = static_cast<Cannon*>(mOwner);
			c->SetMoveState(Cannon::FieldPatroll);
		}
		, 2
			, nullptr
			, VECTOR2(mGame->GetAllData()->itemCompoData.mUIMinPosX - width / 2.0f + 200.0f, 215.0f - 69.0f * 3)

			);
	mFieldPatrollButton = GetButtons()[3];

	AddButton("¢",
		[this]() {
			new UIPSideCharacterStatusClose(mOwner);
			CloseMe();
		}
		, 2
			, nullptr
			, VECTOR2(mGame->GetAllData()->itemCompoData.mUIMinPosX - width / 2.0f + 50.0f, 215.0f)

			);

	mCloseButton = GetButtons()[4];

}

void UIPSideCharacterStatus::draw()
{
	rectMode(CORNER);
	if (mOwner->GetState() == Actor::EActive)
	{

		class Cannon* c = static_cast<Cannon*>(mOwner);

		noStroke();
		fill(mGame->GetAllData()->itemStatusData.mTriangleColor);
		rect(mPos.x, mPos.y, mGame->GetAllData()->itemStatusData.mWidth, mGame->GetAllData()->itemStatusData.mHeight);
		rect(mPos.x + 150.0f, mPos.y + 20.0f, 100.0f, 100.0f);

		DrawItemCompoLifeSpanGauge(c->GetSpeed(), VECTOR2(175.0f, 45.0f));
		DrawItemCompoLifeSpanGauge(c->GetPower(), VECTOR2(225.0f, 45.0f));
		DrawItemCompoLifeSpanGauge(c->GetRapid(), VECTOR2(175.0f, 95.0f));
		DrawItemCompoLifeSpanGauge(c->GetBarrier(), VECTOR2(225.0f, 95.0f));

		DrawLaunchIntervalGauge();

		DrawHpGauge();

		DrawNameandLevel();
	}

}

void UIPSideCharacterStatus::Update()
{
	if (mOwner->GetState() == Actor::EDead)
	{
		CloseMe();
	}

	if (mPos.x > mGame->GetAllData()->itemCompoData.mUIMinPosX)
	{
		mPos.x += mGame->GetAllData()->itemCompoData.mUIPosAdvSpeed;
	}

	mPos.y = (mGame->GetAllData()->itemCompoData.mUIOffsetPosY + 75.0f) * static_cast<Cannon*>(mOwner)->GetNum() + mGame->GetAllData()->itemCompoData.mUIInitPos.y;

	if (mOwner->GetState() == Actor::EActive)
	{
		Cannon* c = static_cast<Cannon*>(mOwner);
		float preWidth = mHpGaugeWidth;
		float wid = 100.0f * mOwner->GetHp() / mOwner->GetMaxHp();
		mHpGaugeWidth = preWidth + (wid - preWidth) * 0.05f;
		float preAngle = mCylinderAngle;
		mCylinderAngle = preAngle + (3.1415926f * 2.0f / mGame->GetAllData()->scopeData.mMaxItenNum * (-static_cast<Cannon*>(mOwner)->GetCnt()) - preAngle) * mGame->GetAllData()->scopeData.mChangeAngleSpeed;

		if (c->GetBarrier())
		{
			float preBWidth = mBarrierHpWidth;
			float Bwid = 50.0f * c->GetBarrier()->GetHp() / c->GetBarrier()->GetMaxHp();
			mBarrierHpWidth = preBWidth + (Bwid - preBWidth) * 0.05f;
		}
	}

	class Cannon* c = static_cast<Cannon*>(mOwner);
	VECTOR2 preCursorPos = mStateCursorPos;
	VECTOR2 cursorPos;

	if (c->GetMoveState() == Cannon::Stay)
	{
		cursorPos = mStayButton->GetPosition();
	}
	else if (c->GetMoveState() == Cannon::Return)
	{
		cursorPos = mReturnButton->GetPosition();

	}
	else if (c->GetMoveState() == Cannon::HomePatroll)
	{
		cursorPos = mHomePatrollButton->GetPosition();

	}
	else if (c->GetMoveState() == Cannon::FieldPatroll)
	{
		cursorPos = mFieldPatrollButton->GetPosition();

	}

	mStateCursorPos = preCursorPos + (cursorPos - preCursorPos) * 0.5f;

	mStayButton->SetPosition(mPos + VECTOR2(50.0f, 195.0f));
	mReturnButton->SetPosition(mPos + VECTOR2(50.0f + 50.0f, 195.0f));
	mHomePatrollButton->SetPosition(mPos + VECTOR2(50.0f + 100.0f, 195.0f));
	mFieldPatrollButton->SetPosition(mPos + VECTOR2(50.0f + 150.0f, 195.0f));
	mCloseButton->SetPosition(mPos + VECTOR2(50.0f + 200.0f, 195.0f));


}

void UIPSideCharacterStatus::DrawRing(const VECTOR2& pos, float radius, float sw, const COLOR& color)
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

void UIPSideCharacterStatus::DrawRingOnLine(const VECTOR2& pos, float radius, float dia, float angle, const COLOR& color)
{
	colorMode(RGB);
	VECTOR2 centerPos = pos;
	VECTOR2 lineEdgePos = VECTOR2(centerPos.x + cosf(angle) * radius, centerPos.y - sinf(angle) * radius);
	strokeWeight(Data.mLineSw);
	stroke(color);
	line(lineEdgePos.x - cosf(angle) * dia / 2.0f, lineEdgePos.y + sinf(angle) * dia / 2.0f, lineEdgePos.x + cosf(angle) * dia / 2.0f, lineEdgePos.y + -sinf(angle) * dia / 2.0f);
}

void UIPSideCharacterStatus::DrawRing(const VECTOR2& pos, float radius, float sw, float angle, const COLOR& color, float angleOffset)
{
	VECTOR2 centerPos = pos;
	float defAngle = 3.14159264f / 180.0f * angleOffset;
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

void UIPSideCharacterStatus::DrawTimeGauge(ItemComponent* item)
{
	VECTOR2 Pos = item->GetPosition();

	noStroke();
	fill(Data.mTriangleColor);
	rect(Pos.x, Pos.y, Data.mWidth, Data.mHeight);

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

	Cannon* c = static_cast<Cannon*>(mOwner);
	if (item == c->GetBarrier())
	{
		fill(Data.mBarrierHpGaugeColor);
		noStroke();

		float preWidth = Data.mRectWidth;
		float WIDTH = Data.mBarrierHpGaugeWidth * c->GetHp() / c->GetMaxHp();

		Data.mRectWidth = preWidth + (WIDTH - preWidth) * Data.mChangeWidthSpeed;

		rectMode(CORNER);
		rect(Pos.x + Data.mWidth / 2.0f - Data.mBarrierHpGaugeWidth / 2.0f, Pos.y + Data.mBarrierHpGaugeOffsetPosY, Data.mRectWidth, Data.mBarrierHpGaugeHeight);
	}

}

void UIPSideCharacterStatus::DrawHpGauge()
{
	COLOR color;
	if (mOwner->GetHp() >= mOwner->GetMaxHp() * mGame->GetAllData()->hpGaugeUIData.mNormalHpRate)
	{
		color = mGame->GetAllData()->hpGaugeUIData.mNormalColor;
	}
	else if (mOwner->GetHp() >= mOwner->GetMaxHp() * mGame->GetAllData()->hpGaugeUIData.mDangerHpRate)
	{
		color = mGame->GetAllData()->hpGaugeUIData.mDangarColor;
	}
	else
	{
		color = mGame->GetAllData()->hpGaugeUIData.mDyingColor;
	}

	noStroke();
	rect(mPos.x + 25.0f, mPos.y + 125.0f, 100.0f, 10.0f);
	fill(color);
	rect(mPos.x + 25.0f, mPos.y + 125.0f, mHpGaugeWidth, 10.0f);
	fill(0, 0, 0);
	textSize(15.0f);
	text("Hp :" + (let)mOwner->GetHp() + "/" + (let)mOwner->GetMaxHp(), mPos.x + 4.5f * 15.0f / 2.0f, mPos.y + 140.0f - 2.5f);

	if (static_cast<Cannon*>(mOwner)->GetBarrier())
	{
		noStroke();
		fill(COLOR(0, 0, 255));
		rect(mPos.x + 25.0f - mBarrierHpWidth + 100.0f, mPos.y + 125.0f - 10.0f, mBarrierHpWidth, 5.0f);
	}
}

void UIPSideCharacterStatus::DrawLaunchIntervalGauge()
{
	class Cannon* c = static_cast<Cannon*>(mOwner);

	DrawRing(mPos + VECTOR2(75.0f, 75.0f), mGame->GetAllData()->scopeData.mGaugeRingRadius * 0.5f, 10 / 2, mGame->GetAllData()->scopeData.mWhiteRingColor);
	DrawRing(mPos + VECTOR2(75.0f, 75.0f), mGame->GetAllData()->scopeData.mGaugeRingRadius * 0.5f, 10 / 2, 3.1415926f / 180.0f * c->GetTimer() / c->GetInterval(), mGame->GetAllData()->scopeData.mGaugeRingColor);

	for (int i = 0; i < 8; i++)
	{
		DrawRingOnLine(mPos + VECTOR2(75.0f, 75.0f), (mGame->GetAllData()->scopeData.mGaugeRingRadius) / 2, 10 / 2, (float)(i * 3.14159264f / 4.0f), Data.mRingOnLineColor);
	}

	strokeWeight(mGame->GetAllData()->scopeData.mStockItemWeight);
	stroke(mGame->GetAllData()->scopeData.mColors[c->GetItemNums()[c->GetCnt()]]);
	point(mPos.x + 75.0f, mPos.y + 75.0f);

	for (int i = 0; i < (int)c->GetItemNums().size(); i++)
	{
		stroke(mGame->GetAllData()->scopeData.mColors[c->GetItemNums()[i]]);
		point(mPos.x + cosf(3.1415926f * 2.0f / mGame->GetAllData()->scopeData.mMaxItenNum * i + mCylinderAngle) * mGame->GetAllData()->scopeData.mItemCylinderRadius / 2 + 75.0f, mPos.y + -sinf(3.1415926f * 2.0f / mGame->GetAllData()->scopeData.mMaxItenNum * i + mCylinderAngle) * mGame->GetAllData()->scopeData.mItemCylinderRadius / 2 + 75.0f);
	}




}

void UIPSideCharacterStatus::DrawItemCompoLifeSpanGauge(class ItemComponent* item, const VECTOR2& pos)
{
	stroke(255, 255, 255);
	strokeWeight(5);

	COLOR color;
	if (!item)
	{
		color = mNoItemColor;
	}
	else
	{
		color = COLOR(255, 255, 255);
	}

	fill(color);
	circle(mPos.x + pos.x, mPos.y + pos.y, 45.0f);

	DrawRing(mPos + pos, 22.5f, 5, mGame->GetAllData()->scopeData.mWhiteRingColor);


	if (item)
	{
		float preTime = item->GetKeepTime();

		float Time = preTime + (item->GetTime() - preTime) * Data.mChangeTimeSpeed;

		item->SetKeepTime(Time);

		DrawRing(mPos + pos, 22.5f, 5, (3.1415926f / 180.0) * item->GetKeepTime() / item->GetInterval(), item->GetColor(), 90);
		fill(0, 0, 0);
		textSize(30);
		text(item->GetLevel(), mPos.x + pos.x - 30.0f / 4.0f, mPos.y + pos.y + 30.0f / 2.0f);

	}

	for (int i = 0; i < 8; i++)
	{
		DrawRingOnLine(mPos + pos, 22.5f, 5, (float)(i * 3.14159264f / 4.0f), COLOR(0, 0, 0));
	}
}

void UIPSideCharacterStatus::DrawNameandLevel()
{
	textSize(15);

	std::ostringstream oss;
	oss << mOwner->GetName().c_str() << " Lv." << mOwner->GetLevel();
	fill(0, 0, 0);
	text(oss.str().c_str(), mPos.x + 15.0f, mPos.y + 15.0f);
}

void UIPSideCharacterStatus::DrawStateCursor()
{

	stroke(255, 0, 0);
	strokeWeight(5);
	fill(0, 0, 0, 0);
	rectMode(CENTER);
	rect(mStateCursorPos.x, mStateCursorPos.y, 69.0f, 69.0f);
	rectMode(CORNER);
	fill(0, 0, 0);

}

void UIPSideCharacterStatus::DrawAfterButton()
{
	DrawStateCursor();
}

