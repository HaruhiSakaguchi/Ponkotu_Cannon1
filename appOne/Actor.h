#pragma once
#include "VECTOR.h"
#include <vector>
#include "COLOR.h"
#include "ActorManager.h"

class Actor
{
public:
	enum State
	{
		EActive, EPause, EDead
	};

	Actor(class Game* game);
	virtual ~Actor();

	void ProcessInput();
	virtual void ActorInput() {}

	virtual void Update();
	virtual void UpdateActor() {}

	virtual int SetUp() { return 0; }

	//Getters,Setters
	State GetState() const { return mState; }
	void SetState(State state) { mState = state; }

	const VECTOR& GetPosition() const { return mPosition; }
	void SetPosition(const VECTOR& pos) { mPosition = pos; }
	void SetPosition(float x, float y, float z) { mPosition.x = x, mPosition.y = y, mPosition.z = z; }
	const VECTOR& GetInitPosition() { return mInitPosition; }
	void SetInitPosition(const VECTOR& pos) { mInitPosition = pos; }
	const VECTOR& GetRotation() { return mRotation; }
	void SetRotationY(float rotationY) { mRotation.y = rotationY; }
	void SetRotationX(float rotationX) { mRotation.x = rotationX; }
	void SetRotationZ(float rotationZ) { mRotation.z = rotationZ; }
	void SetRotation(const VECTOR& angle) { mRotation.x = angle.x; mRotation.y = angle.y; mRotation.z = angle.z; }
	void SetScale(const VECTOR& scale) { mScale = scale; }
	const VECTOR& GetScale() { return mScale; }

	void AddComponent(class Component*);
	void RemoveComponent(class Component*);
	std::vector<Component*>GetComponents() { return mComponents; }

	void DropItems();
	void DropItems(int num1);
	void DropItems(const VECTOR& pos);

	//計算用関数　派生クラスで呼び出して使用
	float Round(float num);
	void Sort(float* num, int left, int right);
	//あるポジションがマップのエリア内にいるかを判別する
	bool PositionOnMap(const VECTOR& pos, float radius = 0);
	bool PositionOnMapArea0(const VECTOR& pos, float radius = 0);

	void SetDamageMesh(class MeshComponent* mesh) { mDamageMesh = mesh; }
	void SetNormalMesh(class MeshComponent* mesh) { mNormalMesh = mesh; }

public:
	enum ActorsCategory
	{
		Character,
		Weapon,
		Item,
		Object
	};

	void SetCategory(ActorsCategory category) { mCategory = category; }
	ActorsCategory GetCategory() { return mCategory; }
	class Game* GetGame() { return mGame; }
private:
	//Actorの状態
	State mState;
	VECTOR mPosition;
	VECTOR mRotation;
	VECTOR mInitPosition;
	VECTOR mScale;
	ActorsCategory mCategory;
protected:
	class MeshComponent* mNormalMesh;
	class MeshComponent* mDamageMesh;
protected:
	float mMaxPosY;
	float mMinPosY;
private:
	//Gameクラスのpublicメンバにアクセスするポインタ
	class Game* mGame;
	//component配列
	std::vector<class Component*> mComponents;
};