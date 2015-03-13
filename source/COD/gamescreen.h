#ifndef CODGAMESCREEN
#define CODGAMESCREEN

#include "state.h"
#include "clickresponder.h"
#include "screen.h"
#include "game.h"
#include "highscore.h"
#include "tileresource.h"
#include <vector>
#include "sound.h"

class GameState: public State, public ClickResponder
{
public:

	GameState();

	virtual bool Init();
	virtual bool Update(float timestep=0.02);
	virtual void Draw();

	virtual void Clicked	(ScreenObject & ob, Button button);
	virtual void Released	(ScreenObject & ob, Button button);
	virtual void Hovered	(ScreenObject & ob, Button button);
	virtual void UnHovered	(ScreenObject & ob, Button button);

	void UpdateInput();
	void UpdateRocks();
	void UpdateRock(int x, int y);

	void KillPlayer();
	void MovePlayer(int x, int y);
	void MoveRock(int x, int y);

	void LoadLevel(unsigned int levelnumber);

	void DrawPlayer(Geometry::Square & area);

	Screen screen;

	int playerx;
	int playery;

	int playerRotation;
	bool playerFlipped;

	int numberOfCollectables;
	int numberOfCollectables2;

	float levelTime;

	HighScore highscore;

	bool DisplayHighScore;
	float DisplayHighScoreTime;

	Sound * fallingRockSound;
	Sound * squishSound;
	Sound * chokeSound;

	Game 	game;

	Texture HUDImage;
	Texture collectableNumberImage;
	Texture collectable2NumberImage;
	Texture levelTimeImage;
	Texture newHighScoreImage;
	Texture newHighScoreStatsImage;
};

#endif