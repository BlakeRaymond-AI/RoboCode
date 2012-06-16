#include <phys253.h>       //   ***** from 253 template file
#include <LiquidCrystal.h> //   ***** from 253 template file
#include <Servo253.h>      //   ***** from 253 template file 

enum MOTORS { LEFT_DRIVE_MOTOR = 0, RIGHT_DRIVE_MOTOR = 1 };
enum ANALOG_INPUTS { LEFT_TAPE_QRD = 5, RIGHT_TAPE_QRD = 4 };

#include <io_helper_functions.h>
#include <tape_follower.h>
#include <menu.h>

void refresh()
{
  if(readStart())
  {
    while(readStart()); //Wait for the start button to stop being pressed
    MENU.open();

    TAPEFOLLOWER.kP = MENU.values.elementAt(0) / 1023.0 * 300;
    TAPEFOLLOWER.kD = MENU.values.elementAt(1) / 1023.0 * 300;
    TAPEFOLLOWER.speed = MENU.values.elementAt(2);
    TAPEFOLLOWER.qrdThresholdL = MENU.values.elementAt(3);
    TAPEFOLLOWER.qrdThresholdR = MENU.values.elementAt(4);
    TAPEFOLLOWER.LRBalance = MENU.values.elementAt(5);
  }
}

void setup()
{
}

void loop()
{
  int i=0;

  while(1)
  {
    ++i;
    TAPEFOLLOWER.followTape();

    if(i == 100)
    {
      refresh();
      tapeFollowerDisplay();
      i=0;
    }
  }
}

