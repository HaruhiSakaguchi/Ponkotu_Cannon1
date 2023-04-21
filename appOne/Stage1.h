#pragma once
#include "Map.h"
#include "Tama.h"

class Stage1 :
	public Map
{
public:
	Stage1(class Game* game);
	int SetUp()override;
private:
	const char* Maptext1 = "Assets\\All_Map\\BackGround\\Collision.txt";
};

