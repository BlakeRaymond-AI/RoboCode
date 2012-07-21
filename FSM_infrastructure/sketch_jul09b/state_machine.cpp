#include "WProgram.h"
#include <state_machine.h>

State TravelToDepot 			= State(travelToDepot_Enter, travelToDepot_Update, travelToDepot_Exit);
State TravelFromDepot			= State(travelFromDepot_Enter, travelFromDepot_Update, travelFromDepot_Exit);
State Error_TapeLost			= State(errorHandling_TapeLost_Enter, errorHandling_TapeLost_Update, errorHandling_TapeLost_Exit);
State Idle						= State(idle_Update);

FSM robotStateMachine(TravelToDepot);
StateHistory STATE_HISTORY(robotStateMachine);

void travelToDepot_Enter()
{
	TAPEFOLLOWER.enable();
}

void travelToDepot_Exit()
{
	TAPEFOLLOWER.disable();
}

void travelToDepot_Update()
{
	//if(!digitalRead(BUMPER))
	//{
		if(TAPEFOLLOWER.followTapeRightBiased())
                  return;
                else 
                  robotStateMachine.transitionTo(Error_TapeLost);
	//}
	//else
	//{
	//	TAPEFOLLOWER.stop();
	//	TAPEFOLLOWER.backUp();
	//	TAPEFOLLOWER.turnAround();
	//	robotStateMachine.transitionTo(TravelFromDepot);
	//}
}

void travelFromDepot_Enter()
{
	TAPEFOLLOWER.enable();
}

void travelFromDepot_Exit()
{
	TAPEFOLLOWER.disable();
}

void travelFromDepot_Update()
{
	if(TAPEFOLLOWER.leftOutboardQRD.aboveThreshold() && TAPEFOLLOWER.rightOutboardQRD.aboveThreshold()) //Go until both sensors hit tape
	{
		if(TAPEFOLLOWER.followTapeLeftBiased())
                  return;
                else
                  robotStateMachine.transitionTo(Error_TapeLost);
	}
	else //Stop
	{
		TAPEFOLLOWER.stop();
		robotStateMachine.transitionTo(Idle);
	}
}

void errorHandling_TapeLost_Enter()
{
	//TODO
	TAPEFOLLOWER.stop();
	
	LCD.clear();
	LCD.home();
	LCD.print("---ERROR---");
	LCD.setCursor(0,1);
	LCD.print("---TAPE LOST---");
	
	while(!readStart())
	{
		delay(100);
	}	
	STATE_HISTORY.rollback();
}

void errorHandling_TapeLost_Exit()
{
}

void errorHandling_TapeLost_Update()
{
}

void idle_Update()
{
	LCD.clear();
	LCD.home();

	LCD.print("N");
	for(int i=0; i<5; ++i)
	{
		LCD.print("YAN");
		delay(500);
	}
	
	LCD.setCursor(0,1);
	
	LCD.print("N");
	for(int i=0; i<5; ++i)
	{
		LCD.print("YAN");
		delay(500);
	}
	
	if(readStart())
	{
		robotStateMachine.transitionTo(TravelToDepot);
	}
}
