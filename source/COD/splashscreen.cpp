#include "splashscreen.h"
#include "mainmenu.h"
#include "texture.h"
#include "window.h"
#include "statemanager.h"

bool SplashScreenState::Init()
{
	currentTime = 0;
	splashImage.SetTexture("assets/views/splashscreen.png");
}


bool SplashScreenState::Update(float timestep)
{
	currentTime += timestep;
	if (currentTime >= displayTime)
	{
		StateManager::GetStateManager()->SwitchState( MainMenuState::GetMainMenu() );
	}
	return true;
}

void SplashScreenState::Draw()
{
	splashImage.RenderTexture(0,0,1.422,1);
}
