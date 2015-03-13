#include "statemanager.h"
#include "graphics.h"
#include <stdio.h>

StateManager * StateManager::instance = NULL;

StateManager * StateManager::GetStateManager()
{
	if (instance == NULL)
	{
		instance = new StateManager();
	}
	return instance;
}

StateManager::StateManager(): newState(NULL), currentState(NULL)
{

}

StateManager::~StateManager()
{
	if (newState != NULL)
	{
		State::Unregester(newState);
	}

	if (currentState != NULL)
	{
		State::Unregester(currentState);
	}
}

void StateManager::SwitchState(State* NewState)
{
	if (newState == NewState)
	{
		printf("attempting to switch state to the same state!\n");
		return;
	}
	
	if (newState != NULL)
	{
		printf("two or more new states have been added to the stack in the same frame! only the latest new state will be used!\n");
		State::Unregester(newState);
	}

	newState = NewState;
}

bool StateManager::Update(float timestep)
{
	if (newState != NULL)
	{
		printf("switching state.\n");		
		State::Unregester(currentState);
		currentState = newState;
		newState = NULL;
	}

	if (currentState != NULL)
	{
		return currentState->Update(timestep);
	}

	printf("quiting!\n");
	return false;
}

void StateManager::Draw()
{
	if (currentState != NULL)
	{
		Graphics::GetGraphics()->Clear();
		currentState->Draw();
		Graphics::GetGraphics()->Present();
	}
}
