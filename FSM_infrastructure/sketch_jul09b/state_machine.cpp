#include "WProgram.h"
#include <state_machine.h>

State TravelToDepot 		= State(travelToDepot_Enter, travelToDepot_Update, travelToDepot_Exit);
State FindBlockInDepot 		= State(findBlockInDepot_Enter, findBlockInDepot_Update, findBlockInDepot_Exit);
State FindBlockInBuildArea  = State(findBlockInBuildArea_Enter, findBlockInBuildArea_Update, findBlockInBuildArea_Exit);
State TravelFromDepot 		= State(travelFromDepot_Enter, travelFromDepot_Update, travelFromDepot_Exit);
State DropBlock 			= State(dropBlock_Enter, dropBlock_Update, dropBlock_Exit);
State StackBlocks 			= State(stackBlocks_Enter, stackBlocks_Update, stackBlocks_Exit);

FSM robotStateMachine(TravelToDepot);

char blockCount = 0;

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
	
    if(OBSERVER.leftBumper.on() || OBSERVER.rightBumper.on())
    {
		DRIVE_SYSTEM.stop();
		MOVEMENT_CONTROL.backUp();
		robotStateMachine.transitionTo(FindBlockInDepot);
    }
}

void findBlockInDepot_Enter()
{
      MOVEMENT_CONTROL.enable();
		GRIPPER.enable();
		
		RANGEFINDERS.enable();
}

void findBlockInDepot_Update()
{
	//pan left and right, looking for a gap

	if(RANGEFINDERS.panLeftUntilGap() || RANGEFINDERS.panRightUntilGap() || RANGEFINDERS.panRightUntilGap() || RANGEFINDERS.panLeftUntilGap())
	{
		DRIVE_SYSTEM.stop();
		
		RANGEFINDERS.moveToBlockInDepot();
		
		GRIPPER.grip();
		
		if(GRIPPER.switchesClosed())
		{
				LIFTER.setTargetPosition(LIFTER_RAISED);
				LIFTER.enable();
				LIFTER.update();
				delay(1000);
				robotStateMachine.transitionTo(TravelFromDepot);
		}
		else
		{
				//back up and try again
				GRIPPER.open();
				MOVEMENT_CONTROL.backUp();
		}
	}
}

void findBlockInDepot_Exit()
{
	MOVEMENT_CONTROL.disable();
}

void travelFromDepot_Enter()
{
		TAPEFOLLOWER.enable();
		TAPEFOLLOWER.turnAround();
}

void travelFromDepot_Update()
{
		TAPEFOLLOWER.followTape();

		if(blockCount == 0)
		{
				if(OBSERVER.leftOutboardQRD.aboveThreshold() && OBSERVER.rightOutboardQRD.aboveThreshold())
				{
						DRIVE_SYSTEM.drive(SLOW_MOTOR_SPEED, SLOW_MOTOR_SPEED);
						delay(1000);
						DRIVE_SYSTEM.stop();
						
						robotStateMachine.transitionTo(DropBlock);
				}
		}
		else //second or higher block
		{
			robotStateMachine.transitionTo(FindBlockInBuildArea);
		}
}

void travelFromDepot_Exit()
{
		TAPEFOLLOWER.disable();
}

void findBlockInBuildArea_Enter()
{
		RANGEFINDERS.enable();
}

void findBlockInBuildArea_Update()
{
		RANGEFINDERS.moveToBlockInBuildArea();
		
		if(OBSERVER.centreBumper.on())
		{
				DRIVE_SYSTEM.stop();
				
				//back up
				DRIVE_SYSTEM.drive(-SLOW_MOTOR_SPEED, -SLOW_MOTOR_SPEED);
				delay(MOVEMENT_CONTROL.backUpTime);
				DRIVE_SYSTEM.stop();				
				
				//drop the block
				robotStateMachine.transitionTo(DropBlock);
		}
}

void findBlockInBuildArea_Exit()
{
		RANGEFINDERS.disable();
}

void dropBlock_Enter()
{

}

void dropBlock_Update()
{
		LIFTER.setTargetPosition(LIFTER_LOWERED);
		LIFTER.update();
		while(!LIFTER.ready())
		{
				OBSERVER.update();
				LIFTER.update();
		}
		GRIPPER.open();
		++blockCount;
		
		//back up
		MOVEMENT_CONTROL.backUp();
		
		TAPEFOLLOWER.turnAround();
		
		robotStateMachine.transitionTo(TravelToDepot);
}

void dropBlock_Exit()
{

}

void stackBlocks_Enter()
{

}

void stackBlocks_Update()
{

}

void stackBlocks_Exit()
{

}
