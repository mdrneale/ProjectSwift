#ifndef SIMPLEGAME
#define SIMPLEGAME

#include "basegame.h"
#include "statemanager.h"

class SimpleGame: public BaseGame
{
public:
	SimpleGame();
	~SimpleGame();

	virtual bool Init();
	virtual bool Update(float timestep=0.02);
	virtual void Draw();

	StateManager* stateManager;
};

#endif