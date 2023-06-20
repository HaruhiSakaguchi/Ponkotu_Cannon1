#include "StageClear.h"
#include "Quit.h"
#include "Title.h"
#include "GamePlay.h"
#include "AllClear.h"
#include <sstream>
#include "Game.h"
#include "TransitionFade.h"
#include "Container.h"

StageClear::StageClear(Game* game)
	: UIMainState(game)
	, mIsChangeTitle(false)
	, mIsChangeAllClear(false)
{
	Data = mGame->GetAllData()->stageClearData;
	std::ostringstream oss;

	oss << "Stage" << "Clear!";

	mTitle = oss.str();
	mTitleColor = Data.mTitleColor;
	mTextSize = Data.mTextSize;;
	mTitlePos = Data.mTitlePos;
	mButtonPos = Data.mButtonPos;
	mState = State::EStageClear;

	mGame->GetRenderer()->GetTransition()->inTrigger();
	mGame->SetPhase(static_cast<Game::StagePhase>(mGame->GetPhase() + 1));

	mClearTimeText = mGame->GetActorManager()->GetStage()->GetTimeText();

	const char* text = 0;
	if (mGame->GetPhase() != Game::FOURTH && mGame->GetContinueFlag())
		/*{
			text = "���̃X�e�[�W�ɐi�݂܂�";
		}
		else*/
	{
		text = "�G���f�B���O�ɐi�݂܂�";
	}

	AddButton("����",
		[this]() {
			mIsChangeAllClear = true;
			ChangeState();
		}
		, 1
			, text
			);

	if (mGame->GetPhase() != Game::FOURTH && mGame->GetContinueFlag())
	{
		AddButton("�^�C�g���ɖ߂�",
			[this]() {
				mIsChangeTitle = true;
				ChangeState();
			}
			, 1
				, "�^�C�g���ɖ߂�܂�"
				);

		AddButton("�Q�[�����I���",
			[this]() {
				new Quit(mGame);
			}
			, 1
				, "�Q�[�����I���܂�"
				);
	}

	if (mGame->GetSoundVolumeManager()->GetBgmFlag())
	{
		if (mGame->GetPhase() != Game::FOURTH)
		{
			playSound(Data.mSound);
			setVolume(Data.mSound, mGame->GetSoundVolumeManager()->GetVolume());
		}
		else
		{
			playSound(Data.mSound2);
			setVolume(Data.mSound2, mGame->GetSoundVolumeManager()->GetVolume());
		}
	}
}

StageClear::~StageClear()
{
	if (mGame->GetPhase() == Game::FOURTH)
	{
		stopSound(Data.mSound2);
	}
	else
	{
		stopSound(Data.mSound);
	}
}

void StageClear::ChangeOption()
{
	mGame->GetActorManager()->ActorClear();
	mGame->GetActorManager()->GetStage()->MapClear();

	if (mIsChangeTitle)
	{
		new Title(mGame);
		mGame->GetRenderer()->GetTransition()->inTrigger();
	}
	else if (mIsChangeAllClear)
	{
		/*if (mGame->GetPhase() != Game::FOURTH && mGame->GetContinueFlag())
		{
			new GamePlay(mGame);
			mGame->GetTransition()->inTrigger();
		}
		else*/
		{
			new AllClear(mGame);
		}
	}

	mGame->SetState(Game::EGameplay);

}

void StageClear::Draw()
{
	UIScreen::Draw();
	textSize(Data.mClearTimeTextSize);
	text((let)"�N���A�^�C�� : " + (let)mClearTimeText.c_str(), width / 2.0f - Data.mClearTimeTextSize * (9 + mClearTimeText.length()) / 4.0f, Data.mTitlePos.y + Data.mClearTimeTextSize * 10.0f);
}
