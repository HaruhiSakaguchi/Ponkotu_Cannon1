#include "UIPSideCharacterStatus.h"
#include "CharacterActor.h"
#include "Cannon.h"
#include "Game.h"
#include "graphic.h"
#include "UIPSideCharacterStatusClose.h"
#include "PlayerHome.h"

UIPSideCharacterStatus::UIPSideCharacterStatus(class CharacterActor* owner)
	: UIPSideCharacterStatusBase(static_cast<PSideCharacterActor*>(owner))
	, mCylinderAngle(0.0f)
	, mStateCursorPos(width, 0.0f)
	, mStayButton(nullptr)
	, mReturnButton(nullptr)
	, mHomePatrollButton(nullptr)
	, mFieldPatrollButton(nullptr)
	, mCloseButton(nullptr)
{
	mPosition = VECTOR2(0.0f, (mGame->GetAllData()->itemCompoData.mUIOffsetPosY + 75.0f)) +
		VECTOR2(2600.0f,100.0f);

	mStayButton = AddButton("S",
		[this]() {
			class Cannon* c = static_cast<Cannon*>(mOwner);
			c->SetMoveState(Cannon::Stay);
			OtherPSideUIsTranslate(-VECTOR2(0.0f, mGame->GetAllData()->itemCompoData.mUIOffsetPosY + 75.0f));
			CloseMe();
			UIPSideCharacterStatusClose* ui = new UIPSideCharacterStatusClose(mOwner);
			ui->SetOffset(this->GetOffset());
		}
		, 2
			, nullptr
			, VECTOR2(mGame->GetAllData()->itemCompoData.mUIMinPosX - width / 2.0f + 50.0f, 215.0f)
			);


	mReturnButton = AddButton("R",
		[this]() {
			class Cannon* c = static_cast<Cannon*>(mOwner);
			c->SetMoveState(Cannon::Return);
			OtherPSideUIsTranslate(-VECTOR2(0.0f, mGame->GetAllData()->itemCompoData.mUIOffsetPosY + 75.0f));
			CloseMe();
			UIPSideCharacterStatusClose* ui = new UIPSideCharacterStatusClose(mOwner);
			ui->SetOffset(this->GetOffset());
		}
		, 2
			, nullptr
			, VECTOR2(mGame->GetAllData()->itemCompoData.mUIMinPosX - width / 2.0f + 100.0f, 215.0f - 69.0f)
			);


	mHomePatrollButton = AddButton("H",
		[this]() {
			class Cannon* c = static_cast<Cannon*>(mOwner);
			c->SetMoveState(Cannon::HomePatroll);
			OtherPSideUIsTranslate(-VECTOR2(0.0f, mGame->GetAllData()->itemCompoData.mUIOffsetPosY + 75.0f));
			CloseMe();
			auto ui = new UIPSideCharacterStatusClose(mOwner);
			ui->SetOffset(this->GetOffset());
		}
		, 2
			, nullptr
			, VECTOR2(mGame->GetAllData()->itemCompoData.mUIMinPosX - width / 2.0f + 150.0f, 215.0f - 69.0f * 2)
			);

	mFieldPatrollButton = AddButton("F",
		[this]() {
			class Cannon* c = static_cast<Cannon*>(mOwner);
			c->SetMoveState(Cannon::FieldPatroll);
			OtherPSideUIsTranslate(-VECTOR2(0.0f, mGame->GetAllData()->itemCompoData.mUIOffsetPosY + 75.0f));
			CloseMe();
			auto ui = new UIPSideCharacterStatusClose(mOwner);
			ui->SetOffset(this->GetOffset());
		}
		, 2
			, nullptr
			, VECTOR2(mGame->GetAllData()->itemCompoData.mUIMinPosX - width / 2.0f + 200.0f, 215.0f - 69.0f * 3)
			);


	mCloseButton = AddButton("¢",
		[this]() {
			OtherPSideUIsTranslate(-VECTOR2(0.0f, mGame->GetAllData()->itemCompoData.mUIOffsetPosY + 75.0f));
			CloseMe();
			auto ui = new UIPSideCharacterStatusClose(mOwner);
			ui->SetOffset(this->GetOffset());
		}
		, 2
			, nullptr
			, VECTOR2(mGame->GetAllData()->itemCompoData.mUIMinPosX - width / 2.0f + 50.0f, 215.0f)

			);

	mLvUpButton = AddButton("+LV",
		[this]() {
			OwnerLvUp();
		}
		, 2
			);


}

void UIPSideCharacterStatus::draw()
{
	rectMode(CORNER);
	if (mOwner->GetState() == Actor::State::EActive)
	{

		class Cannon* c = static_cast<Cannon*>(mOwner);

		fill(50, 150, 255);
		stroke(255, 255, 255);
		strokeWeight(15);
		rect(mPosition.x + mOffset.x, mPosition.y + mOffset.y, mGame->GetAllData()->itemStatusData.mWidth, mGame->GetAllData()->itemStatusData.mHeight * 1.75f);
		noStroke();
		fill(mGame->GetAllData()->itemStatusData.mTriangleColor);
		rect(mPosition.x + mOffset.x, mPosition.y + mOffset.y, mGame->GetAllData()->itemStatusData.mWidth, mGame->GetAllData()->itemStatusData.mHeight);
		rect(mPosition.x + 150.0f + mOffset.x, mPosition.y + mOffset.y + 20.0f, 100.0f, 100.0f);
		textSize(15);
		fill(0, 0, 0);

		text("number :" + (let)mOwner->GetNum(), mPosition.x + 150.0f + mOffset.x, mPosition.y + mOffset.y + 20.0f);
		DrawItemCompoLifeSpanGauge(c->GetSpeed(), VECTOR2(175.0f, 45.0f) + mOffset);
		DrawItemCompoLifeSpanGauge(c->GetPower(), VECTOR2(225.0f, 45.0f) + mOffset);
		DrawItemCompoLifeSpanGauge(c->GetRapid(), VECTOR2(175.0f, 95.0f) + mOffset);
		DrawItemCompoLifeSpanGauge(c->GetBarrier(), VECTOR2(225.0f, 95.0f) + mOffset);

		DrawLaunchIntervalGauge();

		DrawHpGauge(mPosition + mOffset + VECTOR2(0.0f, 105.0f));

		DrawNameandLevel();
	}

}

void UIPSideCharacterStatus::update()
{
	if (mOwner->GetState() == Actor::State::EActive)
	{
		Cannon* c = static_cast<Cannon*>(mOwner);
		float preAngle = mCylinderAngle;
		mCylinderAngle = preAngle + (3.1415926f * 2.0f / mGame->GetAllData()->scopeData.mMaxItenNum * (-static_cast<Cannon*>(mOwner)->GetCnt()) - preAngle) * mGame->GetAllData()->scopeData.mChangeAngleSpeed;
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

	mStayButton->SetPosition(mPosition + VECTOR2(50.0f, 195.0f - 25.0f / 2) + mOffset);
	mReturnButton->SetPosition(mPosition + VECTOR2(50.0f + 50.0f, 195.0f - 25.0f / 2) + mOffset);
	mHomePatrollButton->SetPosition(mPosition + VECTOR2(50.0f + 100.0f, 195.0f - 25.0f / 2) + mOffset);
	mFieldPatrollButton->SetPosition(mPosition + VECTOR2(50.0f + 150.0f, 195.0f - 25.0f / 2) + mOffset);
	mCloseButton->SetPosition(mPosition + VECTOR2(50.0f + 200.0f - 25.0f - 50.0f, 195.0f + 50.0f - 25.0f / 2) + mOffset);
	mLvUpButton->SetPosition(mPosition + VECTOR2(50.0f + 200.0f - 25.0f, 195.0f + 50.0f - 25.0f / 2) + mOffset);

	mNextPSideStatusOffset = VECTOR2(0.0f, mGame->GetAllData()->itemCompoData.mUIOffsetPosY + 75.0f);
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

void UIPSideCharacterStatus::DrawLaunchIntervalGauge()
{
	class Cannon* c = static_cast<Cannon*>(mOwner);

	DrawRing(mPosition + mOffset + VECTOR2(75.0f, 75.0f), mGame->GetAllData()->scopeData.mGaugeRingRadius * 0.5f, 10 / 2, mGame->GetAllData()->scopeData.mWhiteRingColor);
	DrawRing(mPosition + mOffset + VECTOR2(75.0f, 75.0f), mGame->GetAllData()->scopeData.mGaugeRingRadius * 0.5f, 10 / 2, 3.1415926f / 180.0f * c->GetTimer() / c->GetInterval(), mGame->GetAllData()->scopeData.mGaugeRingColor);

	for (int i = 0; i < 8; i++)
	{
		DrawRingOnLine(mPosition + mOffset + VECTOR2(75.0f, 75.0f), (mGame->GetAllData()->scopeData.mGaugeRingRadius) / 2, 10 / 2, (float)(i * 3.14159264f / 4.0f), Data.mRingOnLineColor);
	}

	strokeWeight(mGame->GetAllData()->scopeData.mStockItemWeight);
	stroke(mGame->GetAllData()->scopeData.mColors[c->GetItemNums()[c->GetCnt()]]);
	point(mPosition.x + mOffset.x + 75.0f, mPosition.y + mOffset.y + 75.0f);

	for (int i = 0; i < (int)c->GetItemNums().size(); i++)
	{
		stroke(mGame->GetAllData()->scopeData.mColors[c->GetItemNums()[i]]);
		point(mPosition.x + mOffset.x + cosf(3.1415926f * 2.0f / mGame->GetAllData()->scopeData.mMaxItenNum * i + mCylinderAngle) * mGame->GetAllData()->scopeData.mItemCylinderRadius / 2 + 75.0f, mPosition.y + mOffset.y + -sinf(3.1415926f * 2.0f / mGame->GetAllData()->scopeData.mMaxItenNum * i + mCylinderAngle) * mGame->GetAllData()->scopeData.mItemCylinderRadius / 2 + 75.0f);
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
	circle(mPosition.x + pos.x, mPosition.y + pos.y, 45.0f);

	DrawRing(mPosition + pos, 22.5f, 5, mGame->GetAllData()->scopeData.mWhiteRingColor);


	if (item)
	{
		float preTime = item->GetKeepTime();

		float Time = preTime + (item->GetTime() - preTime) * Data.mChangeTimeSpeed;

		item->SetKeepTime(Time);

		DrawRing(mPosition + pos, 22.5f, 5, (3.1415926f / 180.0f) * item->GetKeepTime() / item->GetInterval(), item->GetColor(), 90);
		fill(0, 0, 0);
		textSize(30);
		text(item->GetLevel(), mPosition.x + pos.x - 30.0f / 4.0f, mPosition.y + pos.y + 30.0f / 2.0f);

	}

	for (int i = 0; i < 8; i++)
	{
		DrawRingOnLine(mPosition + pos, 22.5f, 5, (float)(i * 3.14159264f / 4.0f), COLOR(0, 0, 0));
	}
}

void UIPSideCharacterStatus::DrawNameandLevel()
{
	textSize(15);

	std::ostringstream oss;
	oss << mOwner->GetName().c_str() << " Lv." << mOwner->GetLevel();
	fill(0, 0, 0);
	text(oss.str().c_str(), mPosition.x + mOffset.x + 15.0f, mPosition.y + mOffset.y + 15.0f);
}

void UIPSideCharacterStatus::DrawStateCursor()
{

	stroke(255, 0, 0);
	strokeWeight(5);
	fill(0, 0, 0, 0);
	rectMode(CENTER);
	rect(mStateCursorPos.x + mOffset.x, mStateCursorPos.y, 69.0f, 69.0f);
	rectMode(CORNER);
	fill(0, 0, 0);

}

void UIPSideCharacterStatus::DrawAfterButton()
{
	DrawStateCursor();
}

