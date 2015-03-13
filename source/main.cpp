#include "simplegame.h"
#include "timer.h"

int main()
{
	SimpleGame sg;
	Timer t;
	for(;;)
	{
		if (t.Time()>0.02f)
		{
			if (!sg.Update(t.Time()))
			{
				break;
			}
			t.Reset();
		}
		sg.Draw();
	}
	
	return 0;
};

