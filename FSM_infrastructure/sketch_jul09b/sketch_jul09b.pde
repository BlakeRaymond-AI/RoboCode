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
  MOVEMENT_CONTROL.enable();
  TAPEFOLLOWER.enable();
  TAPEFOLLOWER.turnBias = 1;
  
  LIFTER.enable();
  LIFTER.setTargetPosition(LOWERED);
  GRIPPER.enable();
}

void loop() 
{
  OBSERVER.update();
  LIFTER.update();
  
  TAPEFOLLOWER.followTape();
  
  if(MOVEMENT_CONTROL.hitDepot())
  {
    motor.stop_all();
      
    GRIPPER.grip();
    if(GRIPPER.switchesClosed())
    {
      LIFTER.setTargetPosition(RAISED);
    }
    else
    {
      GRIPPER.open();
    }
    LIFTER.update();
    delay(2000);
    TAPEFOLLOWER.turnAround();
    TAPEFOLLOWER.turnBias = -1;
  }    
  
  if(GRIPPER.switchesClosed() && TAPEFOLLOWER.rightOutboardQRD.aboveThreshold() && (TAPEFOLLOWER.leftQRD.aboveThreshold() || TAPEFOLLOWER.rightQRD.aboveThreshold()))
  {
    motor.stop(LEFT_DRIVE_MOTOR);
    motor.stop(RIGHT_DRIVE_MOTOR);
    LIFTER.setTargetPosition(LOWERED);
    while(!LIFTER.ready())
    {
      OBSERVER.update();
      LIFTER.update();
    }
    GRIPPER.open();
    TAPEFOLLOWER.turnAround();
  }
  
  
  if(readStart())
  {
    LIFTER.setTargetPosition(LOWERED);
    while(!LIFTER.ready())
    {
      OBSERVER.update();
      LIFTER.update();
    }
  }
  if(readStop())
  {
    LIFTER.setTargetPosition(RAISED);
    while(!LIFTER.ready())
    {
      OBSERVER.update();
      LIFTER.update();
    }
  }
}

