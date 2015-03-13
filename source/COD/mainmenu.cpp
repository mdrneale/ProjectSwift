#include "mainmenu.h"
#include "window.h"
#include "graphics.h"
#include "sound.h"

#include <stdio.h>
#include "statemanager.h"
#include "leveleditor.h"
#include "gamescreen.h"

MainMenuState * MainMenuState::instance = NULL;

MainMenuState * MainMenuState::GetMainMenu()
{
	if (instance==NULL)
	{
		instance = new MainMenuState();
	}
	return instance;
}

MainMenuState::MainMenuState()
{
	Init();
}

MainMenuState::~MainMenuState()
{
	delete quicksound;
	delete backgroundMusic;
}

bool MainMenuState::Init()
{
	running = true;
	screen = Screen("assets/views/mainmenu.json");
	screen.SetClickResponder(this);

	bg.SetTexture("assets/views/mainmenu.png");

	quicksound = new Sound("assets/sounds/click.wav");
	Graphics::GetGraphics()->SetDrawColour(1,0.5,1);

	backgroundMusic = new Music("assets/sounds/music.ogg");
	backgroundMusic->Play();
}

void MainMenuState::Clicked(ScreenObject & ob, Button button)
{
	quicksound->Play();
	if (ob.name=="exit")
	{
		running=false;
	}
	else if (ob.name=="edit")
	{
		StateManager::GetStateManager()->SwitchState( new LevelEditorState() );
	}
	else if (ob.name=="start")
	{
		StateManager::GetStateManager()->SwitchState( new GameState() );
	}
	printf("clicked on %s! %f, %f\n", ob.name.c_str(), Window::mouse.currentPosition.x, Window::mouse.currentPosition.y);
}
void MainMenuState::Released	(ScreenObject & ob, Button button){}
void MainMenuState::Hovered		(ScreenObject & ob, Button button){}
void MainMenuState::UnHovered	(ScreenObject & ob, Button button){}

bool MainMenuState::Update(float timestep)
{
	screen.Update(timestep);
	return running;
}

void MainMenuState::Draw()
{
	bg.RenderTexture(0,0,1.422,1);
	screen.Draw();
}
