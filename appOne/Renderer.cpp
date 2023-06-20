#include "Renderer.h"
#include "Game.h"
#include "graphic.h"
#include "window.h"
#include "SpriteComponent.h"
#include "Actor.h"
#include "TransitionFade.h"
#include "SHADER/LAMBERT.h"
#include "CONTAINER/CONTAINER.h"
#include "MeshComponent.h"

Renderer::Renderer(class Game* game)
	:mGame(game)
{
	Initialize();
}
Renderer::~Renderer()
{
	Shutdown();
}

bool Renderer::Initialize()
{
	Data = mGame->GetAllData()->rendererData;

	mShader = new LAMBERT;
	setRasterizerCullBack();
	mProj.pers(Data.mPerspectiveAngle, Data.mPerspectiveAspect, Data.mPerspectiveNear, Data.mPerspectiveFar);

	mContainer = new CONTAINER("Assets\\Assets.txt");

	return true;
}

void Renderer::Shutdown()
{
	delete mShader;
	delete mContainer;
}

void Renderer::Draw()
{
	//SetCursor();

	mShader->setProjView(mProj * mView);
	mShader->setLightPosition(Data.mLightPos);
	mShader->setLightAmbient(Data.mAmbient);
	mShader->setDiffuse(Data.mDiffuse);

	for (auto mesh : mMeshes)
	{
		setTextureSamplerWrap();
		if (mesh->GetOwner()->GetState() == Actor::EActive)
		{
			mesh->Draw();
		}
	}

	for (auto sprite : mSprites)
	{
		if (sprite->GetOwner()->GetState() == Actor::EActive)
		{
			sprite->Draw();
		}
	}

	if (mGame->GetStage())
	{
		mGame->GetStage()->Draw();
	}

	if (mGame->GetState() == Game::EPaused)
	{
		DrawDisplay();
	}

	for (auto ui : mGame->GetUIStack())
	{
		if (ui->GetState() == UIScreen::EActive)
		{
			ui->Draw();
		}
	}

	if (mGame->GetTransition())
	{
		mGame->GetTransition()->draw();
	}

	if (mGame->GetCurState() && mGame->GetState() != Game::EQuit)
	{
		textSize(30);
		fill(0, 0, 0);
		print((int)mGame->GetCurState()->GetState());
	}

}

void Renderer::AddSprite(SpriteComponent* sprite)
{
	// ソート済み配列の挿入場所を探す
	// (自分より順番の大きい最初の要素を探す)
	int myDrawOrder = sprite->GetDrawOrder();
	auto iter = mSprites.begin();
	for (; iter != mSprites.end(); ++iter)
	{
		if (myDrawOrder < (*iter)->GetDrawOrder())
		{
			break;
		}
	}

	// 探し出した要素の前に自分を挿入
	mSprites.insert(iter, sprite);
}

void Renderer::RemoveSprite(SpriteComponent* sprite)
{
	// swapしてpop_backできない。swapしてしまうと順番が崩れるため
	auto iter = std::find(mSprites.begin(), mSprites.end(), sprite);
	mSprites.erase(iter);
}

void Renderer::AddMesh(MeshComponent* mesh)
{
	mMeshes.emplace_back(mesh);
}

void Renderer::RemoveMesh(MeshComponent* mesh)
{
	// swapしてpop_backできない。swapしてしまうと順番が崩れるため
	auto iter = std::find(mMeshes.begin(), mMeshes.end(), mesh);
	mMeshes.erase(iter);
}

void Renderer::SetCursor()
{
	//カーソルの状態をゲームステートに合わせて切り替える
	//Uiを前に出すため、スプライトよりも前に表示する

	showCursor();
}

void Renderer::DrawDisplay()
{
	//画面に貼り付ける透明な四角形
	//ダメージやゲームステートによって色を変える
	rectMode(CORNER);
	noStroke();
	if (mGame->GetDisplayColor().a != 0)
	{
		fill(mGame->GetDisplayColor());
		rect(0, 0, width, height);
	}
}