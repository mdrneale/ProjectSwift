#ifndef BASEGAME
#define BASEGAME

class GameWrapper;

class BaseGame
{
public:
	virtual bool Init() = 0;
	virtual bool Update(float timestep=0.02) = 0;
	virtual void Draw() = 0;
};

#endif