#pragma once
#include <vector>
#include "VECTOR.h"
#include "COLOR.h"
#include "MATRIX.h"

class Renderer
{
public:
	Renderer(class Game* game);
	~Renderer();

	bool Initialize();
	void Shutdown();
	void Draw();

	void SetCursor();
	void DrawDisplay();

	void AddSprite(class SpriteComponent* sprite);
	void RemoveSprite(class SpriteComponent* sprite);
	void AddMesh(class MeshComponent* mesh);
	void RemoveMesh(class MeshComponent* mesh);
	class SHADER* GetShader() { return mShader; }
	class CONTAINER* GetContainer() { return mContainer; }
	void SetView(class MATRIX& view) { mView = view; }

	void SetDisplayColor(const COLOR& color) { mDisplayColor = color; }
	const COLOR& GetDisplayColor() { return mDisplayColor; }

	class TransitionFade* GetTransition() { return mTransition; }

	struct DATA
	{
		float mAmbient = 0.0f;
		COLOR mDiffuse;
		VECTOR mCamPos;
		VECTOR mLookat;
		VECTOR mUp;
		VECTOR mLightPos;
		float mPerspectiveAngle = 0.0f;
		float mPerspectiveAspect = 0.0f;
		float mPerspectiveNear = 0.0f;//近くのクリッピング平面zNear
		float mPerspectiveFar = 0.0f;//遠くのクリッピング平面zFar
	};
private:
	DATA Data;
private:
	class SHADER* mShader;
	class CONTAINER* mContainer;
	class TransitionFade* mTransition;
	std::vector<class MeshComponent*>mMeshes;

	MATRIX mView;
	MATRIX mProj;
	class Game* mGame;
	std::vector<class SpriteComponent*>mSprites;

	COLOR mDisplayColor;

};

