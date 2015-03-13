#include "simplegame.h"
#include "window.h"
#include "graphics.h"
#include "audio.h"
#include "texture.h"
#include "area.h"
#include "point.h"
#include "sound.h"
#include "music.h"
#include "font.h"
#include "splashscreen.h"

SimpleGame::SimpleGame()
{
	stateManager = StateManager::GetStateManager();
	Graphics::CreateGraphics( Window::CreateWindow("Caves Of Doom", 1024, 720)->GetSDLWindow() );
	Audio::CreateAudio();
	Init();	
}

SimpleGame::~SimpleGame()
{
	delete stateManager;
	Graphics::DestroyGraphics();
	Window::DestroyWindow();
	Audio::DestroyAudio();
}

static Texture *t;
static Sound *s;
static Music *m;

bool SimpleGame::Init()
{
	stateManager->SwitchState( new SplashScreenState() );
	return true;
}

bool SimpleGame::Update(float timestep)
{
	if (!Window::CheckInput())
	{
		return false;
	}

	return stateManager->Update(timestep);
}

void SimpleGame::Draw()
{
	stateManager->Draw();
}
