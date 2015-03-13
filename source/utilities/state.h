#ifndef STATE
#define STATE

#ifndef NULL
#define NULL 0
#endif

class State
{
public:
	virtual bool Init() = 0;
	virtual bool Update(float timestep=0.02) = 0;
	virtual void Draw() = 0;
	virtual bool CanBeDeleted() { return true; }
	static void Unregester(State* state)
	{
		if (state == NULL)
		{
			return;
		}
		
		if (state->CanBeDeleted())
		{
			delete state;
			state = NULL;
		}
	};
};

#endif