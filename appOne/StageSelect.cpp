#include "StageSelect.h"
#include "window.h"
#include "Reconfirm.h"
#include "Game.h"

StageSelect::StageSelect(class Game* game)
	:UIState(game)
{
	Data = mGame->GetAllData()->stageSelectData;
	//�e�p�[�c�ʒu����

	mTitle = Data.mTitle;
	mTitleColor = Data.mTitleColor;
	mTextSize = Data.mTextSize;
	mBackPos = Data.mBackPos;
	mTitlePos = Data.mTitlePos;
	mButtonPos = Data.mButtonPos;

	mBackImg = mGame->GetAllData()->mDialog2;

	mGame->SetScene(Game::ETitle);

	AddButton("�X�e�[�W1",
		[this]() {
			mGame->SetPhase(Game::FIRST);
	        new Reconfirm(this, mGame, "�w�X�e�[�W�P�x����n�߂܂����H");
		}
		, 1
			, "�w���܁x���o�ꂷ��t���b�g�ȃX�e�[�W�ł�"
			, VECTOR2(0, 0)
			, Data.mStage1GuideOffsetPos
			);

	AddButton("�X�e�[�W2",
		[this]() {
			mGame->SetPhase(Game::SECOND);
			new Reconfirm(this, mGame, "�w�X�e�[�W�Q�x����n�߂܂����H");
		}
		, 1
			, "�w�T�e���C�g�x���o�ꂷ��t���b�g�ȃX�e�[�W�ł�"
			, VECTOR2(0, 0)
			, Data.mStage2GuideOffsetPos
			);

	AddButton("�X�e�[�W3",
		[this]() {
			mGame->SetPhase(Game::THIRD);
			new Reconfirm(this, mGame, "�w�X�e�[�W�R�x����n�߂܂����H");
		}
		, 1
			, "���ׂĂ̓G���o�ꂷ��A���፷�̂���X�e�[�W�ł�"
			, VECTOR2(0, 0)
			, Data.mStage3GuideOffsetPos
			);

	AddButton("�߂�",
		[this]() {
			CloseMe();
		}
	);
}


