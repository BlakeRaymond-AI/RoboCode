#include "WProgram.h"
#include <state_machine.h>

State TravelToDepot 						= State(travelToDepot_Enter, travelToDepot_Update, travelToDepot_Exit);
State TravelToFirstTurnFromDepot			= State(travelToFirstTurnFromDepot_Enter, travelToFirstTurnFromDepot_Update, travelToFirstTurnFromDepot_Exit);
State TravelFromFirstTurnToBuildArea 		= State(travelFromFirstTurnToBuildArea_Enter, travelFromFirstTurnToBuildArea_Update, travelFromFirstTurnToBuildArea_Exit);
State FindBlock 							= State(findBlock_Enter, findBlock_Update, findBlock_Exit);
State DropBlock								= State(dropBlock_Enter, dropBlock_Update, dropBlock_Exit);
State FindTape								= State(findTape_Enter, findTape_Update, findTape_Exit);

FSM robotStateMachine(TravelToDepot);
StateHistory STATE_HISTORY(robotStateMachine);

void travelToDepot_Enter()
{

}

void travelToDepot_Exit()
{

}

void travelToDepot_Update()
{

}

void findBlock_Enter()
{
	GRIPPER.enable();
}

void findBlock_Update()
{
	GRIPPER.grip();
	
	if(!GRIPPER.switchesClosed()) //back up, turn around, try again
	{
		GRIPPER.open();
		MOVEMENT_CONTROL.backUp();
                MOVEMENT_CONTROL.turnRight(45);
		MOVEMENT_CONTROL.backUp();
		MOVEMENT_CONTROL.turnLeft(45);
		MOVEMENT_CONTROL.forwardToDepot();
	}
	else //got a block
	{
		MOVEMENT_CONTROL.backUp();
		MOVEMENT_CONTROL.turnLeft(90);
		robotStateMachine.transitionTo(FindTape);
	}
}

void findBlock_Exit()
{
	GRIPPER.disable();
}

void findTape_Enter()
{
	TAPEFOLLOWER.enable(); //Enable the outboard QRDs
}

void findTape_Exit()
{
	TAPEFOLLOWER.disable();
}

void findTape_Update()
{
	if(TAPEFOLLOWER.leftOutboardQRD.aboveThreshold())
	{
		MOVEMENT_CONTROL.backUp();
		MOVEMENT_CONTROL.turnLeft(90);
	}
	else if(TAPEFOLLOWER.rightOutboardQRD.aboveThreshold())
	{
		robotStateMachine.transitionTo(TravelToFirstTurnFromDepot);
	}
}

void dropBlock_Enter()
{

}

void dropBlock_Update()
{
	GRIPPER.open();
	MOVEMENT_CONTROL.backUp();
}

void dropBlock_Exit()
{

}

void travelToFirstTurnFromDepot_Enter()
{
	TAPEFOLLOWER.enable();
}

void travelToFirstTurnFromDepot_Update()
{
	if(TAPEFOLLOWER.leftOutboardQRD.aboveThreshold())
	{
		TAPEFOLLOWER.makeHardLeft();
	}
	else
	{
		TAPEFOLLOWER.followTape();
	}
}

void travelToFirstTurnFromDepot_Exit()
{
	TAPEFOLLOWER.disable();
}

void travelFromFirstTurnToBuildArea_Enter()
{
	TAPEFOLLOWER.enable();
}

void travelFromFirstTurnToBuildArea_Update()
{
	if(TAPEFOLLOWER.leftOutboardQRD.aboveThreshold() && TAPEFOLLOWER.rightOutboardQRD.aboveThreshold()) //Reached depot
	{
		robotStateMachine.transitionTo(DropBlock);
	}
	else if(TAPEFOLLOWER.leftOutboardQRD.aboveThreshold()) //Left has reached depot; turn left
	{
		MOVEMENT_CONTROL.stop();
		delay(100);
		while(TAPEFOLLOWER.rightOutboardQRD.belowThreshold())
		{
			MOVEMENT_CONTROL.inchLeft();
			TAPEFOLLOWER.rightOutboardQRD.read();
		}
		MOVEMENT_CONTROL.stop();
	}
	else if(TAPEFOLLOWER.rightOutboardQRD.aboveThreshold()) //Right has reached depot; turn right
	{
		MOVEMENT_CONTROL.stop();
		delay(100);
		while(TAPEFOLLOWER.leftOutboardQRD.belowThreshold())
		{
			MOVEMENT_CONTROL.inchRight();
			TAPEFOLLOWER.leftOutboardQRD.read();
		}
		MOVEMENT_CONTROL.stop();
	}
	else
	{
		TAPEFOLLOWER.followTape();
	}
}

void  travelFromFirstTurnToBuildArea_Exit()
{
	TAPEFOLLOWER.disable();
}
