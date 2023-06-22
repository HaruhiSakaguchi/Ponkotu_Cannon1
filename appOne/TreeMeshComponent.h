#pragma once
#include <vector>
#include "MeshComponent.h"
class TreeMeshComponent :
	public MeshComponent
{
public:
	TreeMeshComponent(class Actor* owner);
	~TreeMeshComponent();
	void SetTree(const char* name);
	void SetDamageTree(const char* name);
	void SetAnim(const char* name);
	void SetAnimId(int id);
	void SetNextAnimId(int id, float morphFrame, float startFrame = 0);
	void Update()override;
	void Draw()override;
private:
	class TREE* mTree;
	class TREE* mNormalTree;
	class TREE* mDamageTree;
	std::vector<const class ANIMATION*> mAnims;
};

