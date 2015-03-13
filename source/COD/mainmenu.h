#ifndef CODMAINMENU
#define CODMAINMENU

#include "state.h"
#include "screen.h"
#include "texture.h"
#include "clickresponder.h"
#include "sound.h"
#include "music.h"

class MainMenuState: public State, public ClickResponder
{
public:
	static MainMenuState * GetMainMenu();
	static MainMenuState * instance;

	MainMenuState();
	virtual ~MainMenuState();
	virtual bool Init();
	virtual bool Update(float timestep=0.02);
	virtual void Draw();

	virtual void Clicked	(ScreenObject & ob, Button button);
	virtual void Released	(ScreenObject & ob, Button button);
	virtual void Hovered	(ScreenObject & ob, Button button);
	virtual void UnHovered	(ScreenObject & ob, Button button);

	virtual bool CanBeDeleted() { return false; }

	Screen screen;
	Texture bg;
	bool running;
	Sound * quicksound;
	Music * backgroundMusic;
};

#endif