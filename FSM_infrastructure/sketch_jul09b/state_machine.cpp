#include "WProgram.h"
#include <state_machine.h>

State TravelToDepot 		= State(travelToDepot_Enter, travelToDepot_Update, travelToDepot_Exit);
State FindBlockInDepot 		= State(findBlockInDepot_Enter, findBlockInDepot_Update, findBlockInDepot_Exit);
State FindBlockInBuildArea      = State(findBlockInBuildArea_Enter, findBlockInBuildArea_Update, findBlockInBuildArea_Exit);
State TravelFromDepot 		= State(travelFromDepot_Enter, travelFromDepot_Update, travelFromDepot_Exit);
State DropBlock 		= State(dropBlock_Enter, dropBlock_Update, dropBlock_Exit);
State StackBlocks 		= State(stackBlocks_Enter, stackBlocks_Update, stackBlocks_Exit);

State TestState_DriveStraight          = State(test_DriveStraight);
State TestState_DriveReverse           = State(test_DriveReverse);
State TestState_ForwardAndStop         = State(test_ForwardAndStop);
State TestState_FindBlockInBuildArea   = State(test_FindBlockInBuildArea);
State TestState_FindBlockInDepot       = State(test_FindBlockInDepot);
State TestState_TapeFollow             = State(test_TapeFollow);
State TestState_Rangefinders           = State(test_Rangefinders);


FSM robotStateMachine(TravelToDepot);

char blockCount = 0;
bool leftSide = true;

void leaveBuildArea_Enter()
{
		OBSERVER.leftQRD.enable();
		OBSERVER.rightQRD.enable();
		OBSERVER.leftOutboardQRD.enable();
		OBSERVER.rightOutboardQRD.enable();
}

void leaveBuildArea_Update()
{
		while(OBSERVER.leftOutboardQRD.belowThreshold() && OBSERVER.rightOutboardQRD.belowThreshold())
		{
				DRIVE_SYSTEM.drive(SLOW_MOTOR_SPEED, SLOW_MOTOR_SPEED);
				OBSERVER.update();
		}
		
		if(leftSide)
		{
				while(OBSERVER.leftOutboardQRD.belowThreshold())
				{
						DRIVE_SYSTEM.drive(SLOW_MOTOR_SPEED, MEDIUM_MOTOR_SPEED);
						OBSERVER.update();
				}
				while(OBSERVER.leftQRD.belowThreshold())
				{
						DRIVE_SYSTEM.drive(SLOW_MOTOR_SPEED, SLOW_MOTOR_SPEED);
						OBSERVER.update();
				}
				robotStateMachine.transitionTo(TravelToDepot);
		}
		else
		{
				while(OBSERVER.rightOutboardQRD.belowThreshold())
				{
						DRIVE_SYSTEM.drive(MEDIUM_MOTOR_SPEED, SLOW_MOTOR_SPEED);
						OBSERVER.update();
				}
				while(OBSERVER.rightQRD.belowThreshold())
				{
						DRIVE_SYSTEM.drive(SLOW_MOTOR_SPEED, SLOW_MOTOR_SPEED);
						OBSERVER.update();
				}
				robotStateMachine.transitionTo(TravelToDepot);				
		}
}

void leaveBuildArea_Exit()
{
		OBSERVER.leftQRD.disable();
		OBSERVER.rightQRD.disable();
		OBSERVER.leftOutboardQRD.disable();
		OBSERVER.rightOutboardQRD.disable();
}

void travelToDepot_Enter()
{
	TAPEFOLLOWER.enable();
	if(leftSide)
	{
		TAPEFOLLOWER.turnBias =1;	
	}
	else
	{
		TAPEFOLLOWER.turnBias = -1;
	}
	
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
		
		//search for a block
    while(!RANGEFINDERS.searchForBlockInDepot())
	{
		LCD.clear();
		LCD.home();
		LCD.print("NO BLOCK FOUND");
		delay(3000);
	}
}

void findBlockInDepot_Update()
{
		if(!RANGEFINDERS.moveToBlockInDepot())
		{
				MOVEMENT_CONTROL.backUp();
				while(!RANGEFINDERS.searchForBlockInDepot())
				{
						LCD.clear();
						LCD.home();
						LCD.print("NO BLOCK FOUND");
						delay(3000);
				}
		}
		
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

void findBlockInDepot_Exit()
{
	MOVEMENT_CONTROL.disable();
	RANGEFINDERS.disable();
}

void travelFromDepot_Enter()
{
		TAPEFOLLOWER.enable();
		if(leftSide)
		{
				TAPEFOLLOWER.turnBias = -1;
		}
		else
		{
				TAPEFOLLOWER.turnBias = 1;
		}
		
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
						delay(1500);
						
						DRIVE_SYSTEM.stop();
						
						robotStateMachine.transitionTo(DropBlock);						
				}
		}
		else if(leftSide)
		{
				if(OBSERVER.leftOutboardQRD.aboveThreshold())
				{
						DRIVE_SYSTEM.stop();
						robotStateMachine.transitionTo(FindBlockInBuildArea);
				}
		}
		else
		{
				if(OBSERVER.rightOutboardQRD.aboveThreshold())
				{
						DRIVE_SYSTEM.stop();
						robotStateMachine.transitionTo(FindBlockInBuildArea);
				}
		}
}

void travelFromDepot_Exit()
{
		TAPEFOLLOWER.disable();
}

void findBlockInBuildArea_Enter()
{
		RANGEFINDERS.enable();
		OBSERVER.centreBumper.enable();
		
		while(!RANGEFINDERS.searchForBlockInBuildArea())
		{
				LCD.clear();
				LCD.home();
				LCD.print("NO BLOCK FOUND");
				delay(3000);
		}
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
		OBSERVER.centreBumper.disable();
}

void dropBlock_Enter()
{
		LIFTER.enable();
		GRIPPER.enable();
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
		LIFTER.disable();
		GRIPPER.disable();
}

void stackBlocks_Enter()
{
		RANGEFINDERS.enable();
		OBSERVER.centreBumper.enable();
}

void stackBlocks_Update()
{
		if(blockCount == 0)
		{
				robotStateMachine.transitionTo(TravelToDepot);
				return;
		}
		
		if(OBSERVER.centreBumper.on())
		{
				DRIVE_SYSTEM.stop();
				GRIPPER.open();
				--blockCount;
				
				//back up
				DRIVE_SYSTEM.drive(-SLOW_MOTOR_SPEED, -SLOW_MOTOR_SPEED);
				delay(500);
				DRIVE_SYSTEM.stop();
				
				LIFTER.setTargetPosition(LIFTER_LOWERED);
				LIFTER.update();
				while(!LIFTER.ready())
				{
						OBSERVER.update();
						LIFTER.update();
				}
		}
		else if(OBSERVER.gripperCentreSwitch.on())
		{
				GRIPPER.grip();
				
				LIFTER.setTargetPosition(LIFTER_RAISED);
				LIFTER.update();
				while(!LIFTER.ready())
				{
						OBSERVER.update();
						LIFTER.update();
				}
		}
		else
		{
				RANGEFINDERS.moveToBlockInBuildArea();
		}
		
}

void stackBlocks_Exit()
{
		RANGEFINDERS.disable();
		OBSERVER.centreBumper.disable();
}

void test_DriveStraight()
{ 
    DRIVE_SYSTEM.drive(MEDIUM_MOTOR_SPEED, MEDIUM_MOTOR_SPEED);
} 

void test_DriveReverse()
{
    DRIVE_SYSTEM.drive(-MEDIUM_MOTOR_SPEED, -MEDIUM_MOTOR_SPEED);
}

void test_ForwardAndStop()
{
    DRIVE_SYSTEM.drive(MEDIUM_MOTOR_SPEED, MEDIUM_MOTOR_SPEED);
    delay(random(250, 2000));
    DRIVE_SYSTEM.stop();
    delay(2000);
}

bool blockFound = false;

void test_FindBlockInBuildArea()
{
    RANGEFINDERS.enable();
    RANGEFINDERS.moveToBlockInBuildArea();
}

void test_FindBlockInDepot()
{
  RANGEFINDERS.enable();
  RANGEFINDERS.moveToBlockInDepot();
}

void test_TapeFollow()
{  
    TAPEFOLLOWER.enable();
    TAPEFOLLOWER.followTape();
}

void test_Rangefinders()
{
		int left = analogRead(LEFT_RANGEFINDER);
		int right = analogRead(RIGHT_RANGEFINDER);
		
		LCD.clear();
		LCD.home();
		LCD.print("L: " + String(left));
		LCD.setCursor(0,1);
		LCD.print("R: " + String(right));
		delay(100);
}
