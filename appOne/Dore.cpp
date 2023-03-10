#include "Dore.h"
#include "TreeMeshComponent.h"

Dore::Dore(class Game* game)
	:Actor(game)
{
	SetUp();
}

int Dore::SetUp()
{
	TreeMeshComponent* tc = new TreeMeshComponent(this);
	tc->SetTree("Dore");
	return 1;
}

void Dore::UpdateActor()
{

}