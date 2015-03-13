#ifndef STATEMANAGER
#define STATEMANAGER
#include "state.h"

class StateManager
{
	public:
		static StateManager * GetStateManager();
		virtual ~StateManager();

		void SwitchState(State* NewState);
		bool Update(float timestep=0.02);
		void Draw();

	private:
		StateManager();
		static StateManager * instance;
		State* currentState;
		State* newState;
};

#endif