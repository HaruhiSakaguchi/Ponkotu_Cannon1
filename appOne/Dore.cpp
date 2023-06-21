#include "Dore.h"
#include "TreeMeshComponent.h"
#include "graphic.h"

Dore::Dore(class Game* game)
	: Actor(game)
	, mCloseComplete(true)
	, mOpenComplete(false)
	, mBeginCloseFlag(false)
	, mBeginOpenFlag(false)
	, mIsRotate(false)
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
	if (mIsRotate)
	{
		if (mBeginOpenFlag && mCloseComplete)
		{
			OpenDore();
		}
		if (mBeginCloseFlag && mOpenComplete)
		{
			CloseDore();
		}
	}
}

bool Dore::OpenDore()        
{
	if (-3.1415926f / 2 <= GetRotation().x)
	{
		SetRotationX(GetRotation().x - 0.017f);
		return false;
	}
	else
	{
		SetRotationX(-3.14159260f / 2);
		mBeginOpenFlag = false;
		mOpenComplete = true;
		mCloseComplete = false;
		return true;
	}
}

bool Dore::CloseDore()
{                                                      
	if (0.0f >= GetRotation().x)
	{
		SetRotationX(GetRotation().x + 0.017f);
		return false;
	}
	else
	{
		SetRotationX(0.0f);
		mBeginCloseFlag = false;
		mCloseComplete = true;
		mOpenComplete = false;
		mIsRotate = false;
		return true;
	}
}
