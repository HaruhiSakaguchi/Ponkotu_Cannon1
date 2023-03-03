#include "Map.h"

class Stage2 : public Map
{
public:
	Stage2(class Game* game);
	int SetUp()override;
private:
	const char* Maptext2 = "Assets\\All_Map\\Stage2\\Collision.txt";
};