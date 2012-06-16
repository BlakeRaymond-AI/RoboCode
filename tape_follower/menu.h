#ifndef MENU_HEADER_GUARD
#define MENU_HEADER_GUARD

#include <io_helper_functions.h>

class Menu
{
public:
  Menu()
  {
    names.addElement("Prop: ");
    names.addElement("Deriv: ");
    names.addElement("Speed: ");
    names.addElement("QRDL: ");
    names.addElement("QRDR: ");
    names.addElement("L/R Balance: ");   

    values.addElement(initialProportionalGain);
    values.addElement(initialDerivGain);
    values.addElement(initialSpeed);
    values.addElement(initialQRDThresholdL);
    values.addElement(initialQRDThresholdR);
    values.addElement(intialLRBalance);
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
    index = knob(6) / 1023.0 * (names.size());
  }

  void display()
  {
    LCD.clear();
    LCD.home();
    LCD.print(names.elementAt(index));
    LCD.print(knob(5));
  }  

  void set(int value)
  {
    values.setElementAt(value, index);
  }

  Vector<char*> names;  
  Vector<int> values;
  int index;
};

Menu MENU;

#endif
