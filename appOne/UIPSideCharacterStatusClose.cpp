#include "UIPSideCharacterStatusClose.h"
#include "Game.h"
#include "graphic.h"
#include "UIPSideCharacterStatus.h"
#include "PlayerHome.h"

UIPSideCharacterStatusClose::UIPSideCharacterStatusClose(class CharacterActor* owner)
	: UIPSideCharacterStatusBase(static_cast<PSideCharacterActor*>(owner))
	, mOpenButtun(nullptr)
{
	mPosition = VECTOR2(0.0f, (mGame->GetAllData()->itemCompoData.mUIOffsetPosY + 75.0f)) +
		VECTOR2(2600.0f,100.0f);

	if (mOwner->GetTag() == CharacterActor::CharactersTag::ECannon)
	{
		mOpenButtun = AddButton("¤",
			[this]() {

				OtherPSideUIsTranslate(VECTOR2(0.0f, mGame->GetAllData()->itemCompoData.mUIOffsetPosY + 75.0f));
				CloseMe();
				auto ui = new UIPSideCharacterStatus(mOwner);
				ui->SetOffset(this->GetOffset());
			}
			, 2
				, nullptr
				, VECTOR2(mGame->GetAllData()->itemCompoData.mUIMinPosX - width / 2.0f + 50.0f, 215.0f)

				);

	}

	mLvUpButton = AddButton("+LV",
		[this]() {
			OwnerLvUp();
		}
		, 2
			);
}

void UIPSideCharacterStatusClose::draw()
{
	rectMode(CORNER);
	if (mOwner->GetState() == Actor::State::EActive)
	{
		noStroke();
		fill(mGame->GetAllData()->itemStatusData.mTriangleColor);
		rect(mPosition.x + mOffset.x, mPosition.y + mOffset.y, mGame->GetAllData()->itemStatusData.mWidth, mGame->GetAllData()->itemStatusData.mHeight / 4);

		DrawHpGauge(mPosition + mOffset);
		DrawNameandLevel();
	}

}

void UIPSideCharacterStatusClose::update()
{
	if (mOwner->GetState() == Actor::State::EActive)
	{
		if (mOwner->GetTag() == CharacterActor::CharactersTag::ECannon)
		{
			mOpenButtun->SetPosition(VECTOR2(mPosition.x + 150.0f + 50.0f - 25.0f, mPosition.y + 25.0f) + mOffset);
		}

		mLvUpButton->SetPosition(VECTOR2(mPosition.x + 150.0f + 50.0f + 50.0f - 25.0f, mPosition.y + 25.0f) + mOffset);
	}
}


void UIPSideCharacterStatusClose::DrawNameandLevel()
{
	textSize(15);

	std::ostringstream oss;
	oss << mOwner->GetName().c_str() << " Lv." << mOwner->GetLevel();
	fill(0, 0, 0);
	text(oss.str().c_str(), mPosition.x + mOffset.x + 15.0f, mPosition.y + mOffset.y + 15.0f);
}
