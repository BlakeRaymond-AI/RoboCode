#ifndef STATE_HISTORY_HEADER_GUARD
#define STATE_HISTORY_HEADER_GUARD

#include <FiniteStateMachine.h>
#include <phys253.h>       //   ***** from 253 template file
#include <LiquidCrystal.h> //   ***** from 253 template file
#include <Servo253.h>      //   ***** from 253 template file 


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

#endif
