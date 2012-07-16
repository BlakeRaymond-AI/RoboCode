#ifndef STATE_HISTORY_HEADER_GUARD
#define STATE_HISTORY_HEADER_GUARD

class StateHistory
{
	public:
		StateHistory() {}
		
		void record()
		{
			if(robotStateMachine.isInState(history.get(history.size()-1)))
				return;
			history.addElement(robotStateMachine.getCurrentState());
		}
		
		void rollback()
		{
			history.removeElementAt(history.size()-1);
			robotStateMachine.transitionTo(history.get(history.size()-1));
		}
		
	private:
		Vector<State> history;
};

StateHistory STATE_HISTORY;

#endif