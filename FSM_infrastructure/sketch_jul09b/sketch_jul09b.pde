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


/* TODO
 	Implement Supervisor or determine it isn't necessary
 	Sort out Menu dependencies via header/source split
 	Add necessary #includes to each header file
 	Clean up followTape()
 	Properly implement errorHandling_TapeLost
 */

void setup() 
{
}

int gCount = 0;

void loop() 
{
  OBSERVER.update();
  robotStateMachine.update();
  STATE_HISTORY.record();

  ++gCount;
  if(gCount == 1000)
  {
    TAPEFOLLOWER.kP = map(analogRead(6), 0, 1023, 0.0, 100.0);
    TAPEFOLLOWER.kD = map(analogRead(7), 0, 1023, 0.0, 100.0);
    LCD.clear();
    LCD.home();
    LCD.print("P: ");
	LCD.print(TAPEFOLLOWER.kP);
	LCD.print(" D: ");
	LCD.print(TAPEFOLLOWER.kD);
    
    gCount = 0;
	
	if(readStart())
	{
		MENU.open();
	}
  }
}

