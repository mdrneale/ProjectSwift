#ifndef CODGAME
#define CODGAME

#include "level.h"
#include "square.h"
#include "tileresource.h"
#include <vector>

class Game
{
public:

	Game();

	void Draw();
	void Draw(Geometry::Square area);
	void DrawWithoutPlayer();
	
	void Load(const char * filename);
	void Save(const char * filename);

	void LoadLevel(int levelnumber);
	void SaveLevel();

	int currentlevel;

	Level 	level;
	std::vector<std::string> levelfiles;
};

#endif