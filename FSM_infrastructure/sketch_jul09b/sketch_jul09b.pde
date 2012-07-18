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

void loop() 
{
	OBSERVER.update();
    robotStateMachine.update();
    STATE_HISTORY.record();
	
	if(readStart())
	{
	  MENU.open();
	}
}


