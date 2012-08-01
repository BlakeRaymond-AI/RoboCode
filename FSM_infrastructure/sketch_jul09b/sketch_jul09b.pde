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
  //MOVEMENT_CONTROL.enable();
  TAPEFOLLOWER.enable();
  TAPEFOLLOWER.turnBias = 1;
  
  //LIFTER.enable();
  //LIFTER.setTargetPosition(LIFTER_LOWERED);
  //GRIPPER.enable();
  
  //RANGEFINDERS.enable();
}

int gCount =0;

void loop() 
{
  RANGEFINDERS.enable();
  OBSERVER.update();
  //LIFTER.update();
  robotStateMachine.update();
  
  if(readStart())
  {
     MENU.open();
  }
  if(readStop())
  {
    STATECONTROLLER.open();
  }
}

