#ifndef CODSPLASHSCREEN
#define CODSPLASHSCREEN

#include "state.h"
#include "texture.h"

class SplashScreenState: public State
{
public:
	SplashScreenState()
	{
		Init();
	}
	virtual bool Init();
	virtual bool Update(float timestep=0.02);
	virtual void Draw();

	static const float displayTime = 2.5f;
	float 		currentTime;
	Texture 	splashImage;
};

#endif