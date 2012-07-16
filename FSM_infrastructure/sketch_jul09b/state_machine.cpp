#include <state_machine.h>

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
	if(!digitalRead(BUMPER))
	{
		TAPEFOLLOWER.followTapeRightBiased();
	}
	else
	{
		TAPEFOLLOWER.stop();
		TAPEFOLLOWER.backUp();
		TAPEFOLLOWER.turnAround();
		robotStateMachine.transitionTo(TravelFromDepot);
	}
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
		TAPEFOLLOWER.followTapeLeftBiased();
	}
	else //Stop
	{
		TAPEFOLLOWER.stop();
		robotStateMachine.transitionTo(Idle);
	}
}

void errorHandling_TapeLost()
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
	
	if(startButton())
	{
		robotStateMachine.transitionTo(TravelToDepot);
	}
}