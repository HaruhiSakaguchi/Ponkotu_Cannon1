#include "Map.h"

class Stage3 : public Map
{
public:
	Stage3(class Game* game);
	int SetUp()override;
private:
	const char* Maptext3 = "Assets\\All_Map\\Stage3\\Collision.txt";
};