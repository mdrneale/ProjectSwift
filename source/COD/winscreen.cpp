#include "winscreen.h"
#include "mainmenu.h"
#include "texture.h"
#include "window.h"
#include "statemanager.h"

bool WinScreenState::Init()
{
	currentTime = 0;
	winImage.SetTexture("assets/views/winscreen.png");
}


bool WinScreenState::Update(float timestep)
{
	currentTime += timestep;
	if (currentTime >= displayTime || Window::mouse.leftClicker == Mouse::CLICKED)
	{
		StateManager::GetStateManager()->SwitchState( MainMenuState::GetMainMenu() );
	}
	return true;
}

void WinScreenState::Draw()
{
	winImage.RenderTexture(0,0,1.422,1);
}
