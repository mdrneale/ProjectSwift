#ifndef CODWINSCREEN
#define CODWINSCREEN

#include "state.h"
#include "texture.h"

class WinScreenState: public State
{
public:
	WinScreenState()
	{
		Init();
	}
	virtual bool Init();
	virtual bool Update(float timestep=0.02);
	virtual void Draw();

	static const float displayTime = 15.0f;
	float 		currentTime;
	Texture 	winImage;
};

#endif