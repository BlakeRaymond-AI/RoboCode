#ifndef STATE_HISTORY_HEADER_GUARD
#define STATE_HISTORY_HEADER_GUARD

#include <FiniteStateMachine.h>

class StateHistory
{
	public:
		StateHistory(FSM &stateMachine)
                :stateMachine(stateMachine)
                {}		
		void record();		
		void rollback();
		
	private:
		Vector<State*> history;
                FSM&  stateMachine;
};

extern StateHistory STATE_HISTORY;

#endif
