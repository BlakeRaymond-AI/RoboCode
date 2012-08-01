#ifndef STATE_CONTROLLER
#define STATE_CONTROLLER

#include <state_machine.h>

class StateMenuItem
{
  public:
    StateMenuItem(State state, char* text)
    : state(state), text(text) {}
    
    	StateMenuItem()
	:text(""),
	state(TravelToDepot)
        {}
        
	const StateMenuItem& operator=(const StateMenuItem& rhs)
	{
	  text = rhs.text;
	  state = rhs.state;
	  return rhs;
	}
	
	int operator=(int rhs)
	{
	  text = "";
	  state = TravelToDepot;
	  return rhs;
	}
    
    State state;
    char* text;
};

class StateController
{
  public:
    StateController() 
    {
      items.addElement(StateMenuItem(TestState_DriveStraight, "Drive Straight"));
      items.addElement(StateMenuItem(TestState_DriveReverse, "Drive Reverse"));
      items.addElement(StateMenuItem(TestState_ForwardAndStop, "Fwd and Stop"));
      items.addElement(StateMenuItem(TestState_FindBlockInBuildArea, "Find Block:BuildArea"));
      items.addElement(StateMenuItem(TestState_FindBlockInDepot, "Find Block:Depot"));
      items.addElement(StateMenuItem(TestState_TapeFollow, "Tape Follow"));
	  items.addElement(StateMenuItem(TestState_Rangefinders, "Rangefinders"));
    }
  
    void open()
    {
        DRIVE_SYSTEM.stop();
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
				  set();
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
    }

  //Scroll through the menu using one of the knobs  
  void getItem()
  {
    index = knob(6) / 1023.0 * (items.size());
  }

  void display()
  {
    LCD.clear();
    LCD.home();
    LCD.print(items.elementAt(index).text);
  }  

  void set()
  {
     robotStateMachine.transitionTo((State&)items.get(index).state);
  }

  Vector<StateMenuItem> items;
  int index;
};

extern StateController STATECONTROLLER;

#endif
