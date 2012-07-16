#ifndef MENU_HEADER_GUARD
#define MENU_HEADER_GUARD

#include <io_helper_functions.h>

struct MenuItem
{
	MenuItem(char* displayText, int* valuePtr)
	:displayText(displayText),
	valuePtr(valuePtr)
	{}
	
	char* displayText;
	int* valuePtr;
};

class Menu
{
public:
  Menu()
  {
    items.addElement(MenuItem("Prop: ", &TAPEFOLLOWER.kP));
	items.addElement(MenuItem("Deriv: ", &TAPEFOLLOWER.kD));
	items.addElement(MenuItem("Speed: ", &TAPEFOLLOWER.baseSpeed));
	items.addElement(MenuItem("QRD: ", &TAPEFOLLOWER.QRDThreshold));
  }

  void open()
  {
    motor.stop(LEFT_DRIVE_MOTOR);
    motor.stop(RIGHT_DRIVE_MOTOR);

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
    LCD.print(items.elementAt(index).displayText);
    LCD.print(knob(5));
  }  

  void set(int value)
  {
    &items.elementAt(index).valuePtr = value;
  }

  Vector<MenuItem> items;
  int index;
};

Menu MENU;

#endif
