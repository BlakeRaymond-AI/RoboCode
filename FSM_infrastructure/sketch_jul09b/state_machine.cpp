#include "WProgram.h"
#include <state_machine.h>

State TravelToDepot 				= State(travelToDepot_Enter, travelToDepot_Update, travelToDepot_Exit);
//State TravelFromDepotToFirstTurn		= State(travelFromDepotToFirstTurn_Enter, travelFromDepotToFirstTurn_Update, travelFromDepotToFirstTurn_Exit);
//State TravelFromFirstTurnToBuildArea 		= State(travelFromFirstTurnToBuildArea_Enter, travelFromFirstTurnToBuildArea_Update, travelFromFirstTurnToBuildArea_Exit);
State FindBlock 				= State(findBlock_Enter, findBlock_Update, findBlock_Exit);
State DropBlock					= State(dropBlock_Enter, dropBlock_Update, dropBlock_Exit);
State FindTape					= State(findTape_Enter, findTape_Update, findTape_Exit);
State FindBlockInBuildArea 		= State(findBlockInBuildArea_Enter, findBlockInBuildArea_Update, findBlockInBuildArea_Exit);

FSM robotStateMachine(TravelToDepot);
StateHistory STATE_HISTORY(robotStateMachine);

void travelToDepot_Enter()
{
	TAPEFOLLOWER.enable();
	TAPEFOLLOWER.turnBias =1;
    MOVEMENT_CONTROL.enable();
}

void travelToDepot_Exit()
{
	TAPEFOLLOWER.disable();
    MOVEMENT_CONTROL.disable();
}

void travelToDepot_Update()
{
    TAPEFOLLOWER.followTape();
    if(MOVEMENT_CONTROL.leftBumper.on() || MOVEMENT_CONTROL.rightBumper.on())
    {
		robotStateMachine.transitionTo(FindBlock);
        LCD.clear();
        LCD.print("Finding block");
    }
}

void findBlock_Enter()
{
      MOVEMENT_CONTROL.enable();
	//GRIPPER.enable();
}

void findBlock_Update()
{
	//GRIPPER.grip();
	
	if(/*!GRIPPER.switchesClosed()*/true) //back up, turn around, try again
	{
		//GRIPPER.open();
		MOVEMENT_CONTROL.backUp();
        MOVEMENT_CONTROL.turnRight(45);
		MOVEMENT_CONTROL.backUp();
		MOVEMENT_CONTROL.turnLeft(45);
		MOVEMENT_CONTROL.forwardToDepot();
        motor.stop(LEFT_DRIVE_MOTOR);
        motor.stop(RIGHT_DRIVE_MOTOR);
        delay(3000);
	}
	else //got a block
	{
		LIFTER.setTargetPosition(RAISED);
		LIFTER.update();
		MOVEMENT_CONTROL.backUp();
		MOVEMENT_CONTROL.turnLeft(90);
		robotStateMachine.transitionTo(FindTape);
	}
}

void findBlock_Exit()
{
	MOVEMENT_CONTROL.disable();
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
	if(TAPEFOLLOWER.leftOutboardQRD.aboveThreshold() || TAPEFOLLOWER.rightOutboardQRD.aboveThreshold())
	{
		MOVEMENT_CONTROL.backUp();
		MOVEMENT_CONTROL.turnLeft(45);
		MOVEMENT_CONTROL.forwardToTape();
		//robotStateMachine.transitionTo(TravelFromDepotToFirstTurn);
	}
	else 
	{
		motor.speed(LEFT_DRIVE_MOTOR, TAPEFOLLOWER.baseSpeed);
		motor.speed(RIGHT_DRIVE_MOTOR, TAPEFOLLOWER.baseSpeed);
	}
}

void dropBlock_Enter()
{

}

void dropBlock_Update()
{
	if(LIFTER.ready())
	{
		GRIPPER.open();
		MOVEMENT_CONTROL.backUp();
		robotStateMachine.transitionTo(TravelToDepot);
	}
}

void dropBlock_Exit()
{

}

void findBlockInBuildArea_Enter()
{
	//if(centreBumper.on())
	//{
		motor.stop(LEFT_DRIVE_MOTOR);
		motor.stop(RIGHT_DRIVE_MOTOR);
	//}
	//else
//	{
		motor.speed(LEFT_DRIVE_MOTOR, MOVEMENT_CONTROL.minMotorSpeed);
		motor.speed(RIGHT_DRIVE_MOTOR, MOVEMENT_CONTROL.minMotorSpeed);
//	}
}

void findBlockInBuildArea_Update()
{

}

void findBlockInBuildArea_Exit()
{

}



