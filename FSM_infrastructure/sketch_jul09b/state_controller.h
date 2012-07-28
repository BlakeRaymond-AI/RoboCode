#ifndef STATE_CONTROLLER_HEADER_GUARD
#define STATE_CONTROLLER_HEADER_GUARD

#include <state_machine.h>

struct StateControllerItem
{
        StateControllerItem()
        : state(NULL),
        text(NULL)
        {}
        
        StateControllerItem operator=(int zero)
        {
          return *this;
        }
  
	StateControllerItem(State* state, char* text)
	: state(state), 
	text(text)
	{
	
	}
	
	State* state;
	char* text;	
};

class StateController
{
	public:
	StateController()
        {
                states.addElement(StateControllerItem(&TravelToDepot, "TravelToDepot"));
		states.addElement(StateControllerItem(&FindBlock, "FindBlock"));
		states.addElement(StateControllerItem(&FindTape, "FindTape"));
		//states.addElement(StateControllerItem(&TravelFromDepotToFirstTurn, "Depot->FirstTurn"));
		//states.addElement(StateControllerItem(&TravelFromFirstTurnToBuildArea, "FirstTurn->BuildArea"));
		states.addElement(StateControllerItem(&DropBlock, "DropBlock"));	
        }	
		
	void open()
  {
        motor.stop_all();
	delay(1000);

    //Press stop to exit the menu 
    while(!readStop())
    {
      getItem();
      display();
      delay(50);
      //Press start to set the value
      if(readStart())
      {
        set(knob(5));
        LCD.setCursor(0,1);
        LCD.print("Set");
        delay(1000);
      }
    }

    LCD.clear();
	LCD.home();
	int i=5;
	while(i>0)
	{
		LCD.print(i);
		delay(1000);
		--i;
	}
	LCD.clear();
  }

  //Scroll through the menu using one of the knobs  
  void getItem()
  {
    index = knob(6) / 1023.0 * (states.size());
  }

  void display()
  {
    LCD.clear();
    LCD.home();
    LCD.print(states.elementAt(index).text);
  }  

  void set(int value)
  {
    robotStateMachine.transitionTo(*states.get(index).state);
  }

  Vector<StateControllerItem> states;
  int index;
};

extern StateController STATECONTROLLER;

#endif
