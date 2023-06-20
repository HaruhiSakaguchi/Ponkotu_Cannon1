#include "Actor.h"
#include "Game.h"
#include "Component.h"
#include "Map.h"
#include "ItemComponent.h"
#include "Items.h"
#include "Container.h"
#include <sstream>

Actor::Actor(Game* game)
	: mGame(game)
	, mPosition(0.0f, 0.0f, 0.0f)
	, mRotation(0.0f, 0.0f, 0.0f)
	, mInitPosition(0.0f, 0.0f, 0.0f)
	, mScale(1.0f, 1.0f, 1.0f)
	, mState(EActive)
	, mCategory(Object)
	, mMinPosY(-45.0f)
	, mMaxPosY(15.0f)
{
	mGame->GetActorManager()->AddActor(this);
}

Actor::~Actor()
{
	mGame->GetActorManager()->RemoveActor(this);

	while (!mComponents.empty())
	{
		delete mComponents.back();
	}
}

void Actor::ProcessInput()
{
	if (mState == EActive)
	{
		for (auto comp : mComponents)
		{
			comp->ProcessInput();
		}

		ActorInput();
	}
}

void Actor::Update()
{
	if (mState == EActive)
	{
		UpdateActor();

		for (auto component : mComponents)
		{
			component->Update();
		}
	}
}

bool Actor::PositionOnMap(const VECTOR& pos, float radius)
{
	int cnt = 0;
	if (GetGame()->GetStage())
	{
		for (int i = 0; i < mGame->GetStage()->GetAreaNum(); i++)
		{
			if ((pos.x - radius <= mGame->GetStage()->GetMaxX()[i] && pos.x + radius >= mGame->GetStage()->GetMinX()[i] && pos.z - radius <= mGame->GetStage()->GetMaxZ()[i] && pos.z + radius >= mGame->GetStage()->GetMinZ()[i]) && (pos.y >= 0 && pos.y <= mMaxPosY))
			{
				cnt++;
			}
		}
	}

	if (cnt != 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Actor::PositionOnMapArea0(const VECTOR& pos, float radius)
{
	int cnt = 0;
	if (GetGame()->GetStage() && ((pos.x - radius <= mGame->GetStage()->GetMaxX()[0] && pos.x + radius >= mGame->GetStage()->GetMinX()[0] && pos.z - radius <= mGame->GetStage()->GetMaxZ()[0] && pos.z + radius >= mGame->GetStage()->GetMinZ()[0]) && (pos.y >= 0 && pos.y <= mMaxPosY)))
	{
		return true;
	}
	else
	{
		return false;
	}
}


void Actor::AddComponent(Component* component)
{
	// �\�[�g�ςݔz��̑}���ꏊ��T��
	// (������菇�Ԃ̑傫���ŏ��̗v�f��T��)
	int myOrder = component->GetUpdateOrder();
	auto iter = mComponents.begin();
	for (; iter != mComponents.end(); ++iter)
	{
		if (myOrder < (*iter)->GetUpdateOrder())
		{
			break;
		}
	}

	// �T���o�����v�f�̑O�Ɏ�����}��
	mComponents.insert(iter, component);
}

void Actor::RemoveComponent(Component* component)
{
	auto iter = std::find(mComponents.begin(), mComponents.end(), component);
	if (iter != mComponents.end())
	{
		mComponents.erase(iter);
	}
}

//�A�N�^�[�̃|�C���g����h���b�v
void Actor::DropItems()
{
	int num = rand() % 5;
	VECTOR pos = GetPosition();
	Recovery* r = nullptr;
	PowerUp* p = nullptr;
	SpeedUp* s = nullptr;
	RapidFire* f = nullptr;
	Barrier* b = nullptr;
	switch (num)
	{
	case 0:
		r = new Recovery(GetGame());
		r->SetPosition(pos);
		break;
	case 1:
		p = new PowerUp(GetGame());
		p->SetPosition(pos);
		break;
	case 2:
		s = new SpeedUp(GetGame());
		s->SetPosition(pos);
		break;
	case 3:
		f = new RapidFire(GetGame());
		f->SetPosition(pos);
		break;
	case 4:
		b = new Barrier(GetGame());
		b->SetPosition(pos);
		break;
	default:
		break;
	}
}

//�A�C�e�����w�肵�āA�A�N�^�[�̃|�C���g����h���b�v
void Actor::DropItems(int num)
{
	VECTOR pos = GetPosition();
	class Item* i = nullptr;
	switch (num)
	{
	case 1:
		i = new Recovery(GetGame());
		break;
	case 2:
		i = new PowerUp(GetGame());
		break;
	case 3:
		i = new SpeedUp(GetGame());
		break;
	case 4:
		i = new RapidFire(GetGame());
		break;
	case 5:
		i = new Barrier(GetGame());
		break;
	default:
		break;
	}

	if (i != nullptr)
	{
		i->SetPosition(pos);
	}
}


//�|�C���g���w�肵�ăA�C�e�����h���b�v
void Actor::DropItems(const VECTOR& pos)
{
	int num = rand() % 5;
	class Item* i = nullptr;
	switch (num)
	{
	case 0:
		i = new Recovery(GetGame());
		break;
	case 1:
		i = new PowerUp(GetGame());
		break;
	case 2:
		i = new SpeedUp(GetGame());
		break;
	case 3:
		i = new RapidFire(GetGame());
		break;
	case 4:
		i = new Barrier(GetGame());
		break;
	default:
		break;
	}

	if (i != nullptr)
	{
		i->SetPosition(pos);
		std::ostringstream oss;
		oss << i->GetName() << "�A�C�e�����h���b�v�B";
		GetGame()->GetStage()->GetLog()->AddText(oss.str());
	}

}

float Actor::Round(float num)
{
	return float(round(num));
}

void Actor::Sort(float* num, int left, int right)
{
	if (left >= right)
	{
		return;
	}
	float p = num[(left + right) / 2];//�s�|�b�g  left��right�̒��Ԓn�_�������؂�ɔz��𕪊�����
	int l = left, r = right; float tmp;
	while (l <= r)
	{
		while (num[l] < p) { l++; }
		while (num[r] > p) { r--; }
		if (l <= r)
		{
			tmp = num[l]; num[l] = num[r]; num[r] = tmp;
			l++; r--;
		}
	}
	Sort(num, left, r);//�s�|�b�g��荶�����\�[�g
	Sort(num, l, right);//�s�|�b�g���E�����\�[�g
}