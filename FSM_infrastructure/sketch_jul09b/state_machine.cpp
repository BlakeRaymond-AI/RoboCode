#include "WProgram.h"
#include <state_machine.h>

State TravelToDepot = State(travelToDepot_Enter, travelToDepot_Update, travelToDepot_Exit);
State FindBlock = State(findBlock_Enter, findBlock_Update, findBlock_Exit);
State TravelFromDepot = State(travelFromDepot_Enter, travelFromDepot_Update, travelFromDepot_Exit);
State DropBlock = State(dropBlock_Enter, dropBlock_Update, dropBlock_Exit);
State StackBlocks = State(stackBlocks_Enter, stackBlocks_Update, stackBlocks_Exit);

FSM robotStateMachine(TravelToDepot);

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
		DRIVE_SYSTEM.stop();
		MOVEMENT_CONTROL.backUp();
		robotStateMachine.transitionTo(FindBlock);
        LCD.clear();
        LCD.print("Finding block");
    }
}

void findBlock_Enter()
{
      MOVEMENT_CONTROL.enable();
		GRIPPER.enable();
		
		//RANGEFINDERS.enable();
		
		MOVEMENT_CONTROL.turnLeft(45);
}

void findBlock_Update()
{
	//pan left and right, looking for a gap

	DRIVE_SYSTEM.turnRight(SLOW_MOTOR_SPEED);
	
	if(//left rangefinder.gap && right rangefinder.gap)
	{
		DRIVE_SYSTEM.stop();
		
		//RANGEFINDERS.moveToBlock();
		
		GRIPPER.grip();
		
		if(GRIPPER.switchesClosed())
		{
				LIFTER.setTargetPosition(LIFTER_RAISED);
				LIFTER.enable();
				LIFTER.update();
				delay(1000);
				robotStateMachine.transitionTo(TravelFromDepot)
		}
		else
		{
				//back up and try again
				MOVEMENT_CONTROL.backUp();
				MOVEMENT_CONTROL.turnLeft(45);
		}
	}
	else if(//bothTapesTried == false)
	{
		//find the other tape, try again
		MOVEMENT_CONTROL.turnRight(45);
		
		TAPEFOLLOWER.enable();
		MOVEMENT_CONTROL.forwardToTape();
		TAPEFOLLOWER.disable();
		
		MOVEMENT_CONTROL.turnLeft(90);
		MOVEMENT_CONTROL.forwardToDepot();
		MOVEMENT_CONTROL.backUp();
		MOVEMENT_CONTROL.turnLeft(45);		
	}
	else //give up
	{
		DRIVE_SYSTEM.stop();
		LCD.clear();
		LCD.home();
		LCD.print("NO BLOCKS FOUND");
		while(true)
		{
				if(readStart())
				{
						robotStateMachine.transitionTo(TravelToDepot);
						break;
				}
				delay(500);
		}
    }
			   
}

void findBlock_Exit()
{
	MOVEMENT_CONTROL.disable();
}

void travelFromDepot_Enter()
{
}

void travelFromDepot_Update()
{
}

void travelFromDepot_Exit()
{
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

