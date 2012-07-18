#include "WProgram.h"
#include <state_history.h>

void StateHistory::record()
{
	if(stateMachine.isInState(*(history.lastElement())))
		return;
	history.addElement(&(stateMachine.getCurrentState()));
}

void StateHistory::rollback()
{
	history.removeElementAt(history.size()-1);
	stateMachine.transitionTo(*(history.lastElement()));
}
