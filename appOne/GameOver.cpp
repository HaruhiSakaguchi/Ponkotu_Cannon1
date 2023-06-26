#include "GameOver.h"
#include "Title.h"
#include "Quit.h"
#include <vector>
#include "GamePlay.h"
#include "Reconfirm.h"
#include "Game.h"
#include "TransitionFade.h"
#include "Container.h"

GameOver::GameOver(Game* game)
	: UIMainState(game)
	, mTime(0.0f)
	, mIsRetryFlag(false)
	, mIsChangeTitleFlag(false)
{
	Data = mGame->GetAllData()->overData;

	mTitle = Data.mTitle;
	mTitleColor = Data.mTitleColor;
	mTextSize = Data.mTitleTextSize;
	mTitlePos = Data.mTitlePos;
	mButtonPos = Data.mButtonPos;
	mState = State::EGameOver;

	mGame->GetRenderer()->GetTransition()->inTrigger();

	AddButton("�^�C�g���ɖ߂�",
		[this]() {
			mIsChangeTitleFlag = true;
			ChangeState();
		}
		, 1
			, "�^�C�g���ɖ߂�܂�"
			);

	AddButton("���g���C",
		[this]() {
			mIsRetryFlag = true;
			ChangeState();
		}
		, 1
			, "������x�X�e�[�W����蒼���܂�"
			, VECTOR2(0.0f, 0.0f)
			, Data.mRetryButtonPosOffset
			);

	AddButton("�Q�[�����I���",
		[this]() {
			new Quit(mGame);
		}
		, 1
			, "�Q�[�����I���܂�"
			);

	if (mGame->GetSoundVolumeManager()->GetBgmFlag())
	{
		playSound(Data.mSound);
		setVolume(Data.mSound, mGame->GetSoundVolumeManager()->GetVolume());
	}
}

void GameOver::Update()
{
	setVolume(Data.mSound, mGame->GetSoundVolumeManager()->GetVolume());
	mTime += Data.mTitlePosAdvSpeed;
	VECTOR2 prePos = mTitlePos;
	VECTOR2 pos = VECTOR2(mTitlePos.x, mTitlePos.y + sinf(mTime) * Data.mMaxOffsetPosY);
	mTitlePos = prePos + (pos - prePos) * Data.mTitlePosAdvSpeed;
	UIState::Update();
}

void GameOver::ChangeOption()
{
	if (mIsRetryFlag && !mIsChangeTitleFlag)
	{
		mGame->GetActorManager()->ActorClear();
		new GamePlay(mGame);
		mGame->SetState(Game::EGameplay);
	}
	else if (!mIsRetryFlag && mIsChangeTitleFlag)
	{
		mGame->GetActorManager()->ActorClear();
		new Title(mGame);
		mGame->SetState(Game::EGameplay);
	}
}


