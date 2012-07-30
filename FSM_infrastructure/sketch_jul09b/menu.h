#ifndef MENU_HEADER_GUARD
#define MENU_HEADER_GUARD

#include <io_helper_functions.h>
#include <tape_follower.h>
#include <movement_control.h>
#include <rangefinders.h>

class MenuItem
{
public:
	MenuItem(char* displayText, float* valuePtr, float minValue, float maxValue)
	:displayText(displayText),
	valuePtrFloat(valuePtr),
	valuePtrInt(NULL),
	minValue(minValue),
	maxValue(maxValue)
	{}
	
	MenuItem(char* displayText, int* valuePtr, int minValue, int maxValue)
	:displayText(displayText),
	valuePtrInt(valuePtr),
	valuePtrFloat(NULL),
	minValue(minValue),
	maxValue(maxValue)
	{}

	MenuItem()
	:displayText(""),
	valuePtrInt(NULL),
	valuePtrFloat(NULL),
	minValue(0),
	maxValue(0)
	{}
        
	const MenuItem& operator=(const MenuItem& rhs)
	{
	  displayText = rhs.displayText;
	  valuePtrInt = rhs.valuePtrInt;
	  valuePtrFloat = rhs.valuePtrFloat;
	  return rhs;
	}
	
	int operator=(int rhs)
	{
	  displayText = "";
	  valuePtrInt = NULL;
	  valuePtrFloat = NULL;
	  return rhs;
	}
	
	void setValue(int value) const
	{
		if(valuePtrInt)
		{
			*valuePtrInt = mapKnob(value);
		}
		else if(valuePtrFloat)
		{
			*valuePtrFloat = mapKnob(value);
		}
	}
	
	float mapKnob(int value) const
	{
		return map(value, 0, 1023, minValue, maxValue);
	}
	
	char* displayText;
	
private:
	
	float* valuePtrFloat;
	int* valuePtrInt;

	float minValue;
	float maxValue;
};

class Menu
{
public:
  Menu();

  void open()
  {
	motor.stop_all();
	delay(500);

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
	LCD.clear();
	LCD.home();
	int i=3;
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
    LCD.print(items.elementAt(index).displayText);
    LCD.print(items.elementAt(index).mapKnob(knob(5)));
  }  

  void set(int value)
  {
    items.elementAt(index).setValue(value);
  }

  Vector<MenuItem> items;
  int index;
};

extern Menu MENU;

#endif
