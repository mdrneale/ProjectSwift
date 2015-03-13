#ifndef CODLEVELEDITOR
#define CODLEVELEDITOR

#include "state.h"
#include "clickresponder.h"
#include "screen.h"
#include "game.h"
#include "tileresource.h"
#include "texture.h"

class LevelEditorState: public State, public ClickResponder
{
public:

	LevelEditorState();

	virtual bool Init();
	virtual bool Update(float timestep=0.02);
	virtual void Draw();

	virtual void Clicked	(ScreenObject & ob, Button button);
	virtual void Released	(ScreenObject & ob, Button button);
	virtual void Hovered	(ScreenObject & ob, Button button);
	virtual void UnHovered	(ScreenObject & ob, Button button);

	void Load(const char * filename);
	void Save(const char * filename);

	Screen screen;

	int currenttile;

	Game 	game;

	Texture BGImage;

};

#endif