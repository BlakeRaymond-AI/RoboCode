#ifndef MENU_HEADER_GUARD
#define MENU_HEADER_GUARD

#include <io_helper_functions.h>
#include <tape_follower.h>

struct MenuItem
{
	MenuItem(char* displayText, int* valuePtr)
	:displayText(displayText),
	valuePtr(valuePtr)
	{}

        MenuItem()
        :displayText(""),
        valuePtr(NULL)
        {}
        
        const MenuItem& operator=(const MenuItem& rhs)
        {
          displayText = rhs.displayText;
          valuePtr = rhs.valuePtr;
          return rhs;
        }
        
        int operator=(int rhs)
        {
          displayText = "";
          valuePtr = NULL;
          return rhs;
        }

        char* getDisplayText() const
        { 
          return displayText;
        }
	
	char* displayText;
	int* valuePtr;
};

class Menu
{
public:
  Menu(TapeFollower &tapeFollower);

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
    //LCD.print(items.elementAt(index).displayText);
    LCD.print(knob(5));
  }  

  void set(int value)
  {
    *(items.elementAt(index).valuePtr) = value;
  }

  Vector<MenuItem> items;
  int index;
};

extern Menu MENU;

#endif
