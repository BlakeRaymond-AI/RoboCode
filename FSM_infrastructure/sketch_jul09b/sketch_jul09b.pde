#include <FiniteStateMachine.h>
#include <phys253.h>       //   ***** from 253 template file
#include <LiquidCrystal.h> //   ***** from 253 template file
#include <Servo253.h>      //   ***** from 253 template file 

#include <io_helper_functions.h>
#include <observer.h>
#include <pins.h>
#include <signal.h>
#include <tape_follower.h>
#include <state_machine.h>
#include <state_controller.h>

void setup() 
{
  LIFTER.enable();
}

int gCount = 0;

void loop() 
{

  OBSERVER.update();
  LIFTER.update();
  robotStateMachine.update();
  STATE_HISTORY.record();

  if(readStart())
  {
    MENU.open();
  }
  if(readStop())
  {
    STATECONTROLLER.open();
  }  

  /*
  ++gCount;
  if(gCount == 100)
  {
    TAPEFOLLOWER.kP = analogRead(6);
    TAPEFOLLOWER.kD = analogRead(7);
    LCD.clear();
    LCD.home();
    LCD.print("P: " + String(TAPEFOLLOWER.kP));
    LCD.setCursor(0,1);
    LCD.print("D: " + String(TAPEFOLLOWER.kD));
    
    gCount = 0;
  }
  */
}

