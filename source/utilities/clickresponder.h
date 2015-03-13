#ifndef CLICKRESPONDER
#define CLICKRESPONDER
#include "screenobject.h"

class ClickResponder
{
public:
	enum Button 
	{  
		LEFT,
		RIGHT,
		MIDDLE
	};
	virtual void Clicked	(ScreenObject & ob, Button button) = 0;
	virtual void Released	(ScreenObject & ob, Button button) = 0;
	virtual void Hovered	(ScreenObject & ob, Button button) = 0;
	virtual void UnHovered	(ScreenObject & ob, Button button) = 0;
};

#endif