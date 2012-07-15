#include <tape_follower.h>

boolean isBumperHit(int bumperPin)
{
	return digitalRead(bumperPin);
}

void bumperHit(int &dummy) 
{
	operationComplete = true;
}

void travelToDepot() 
{
	TapeFollower tapeFollower; //TODO extend class for particular tapefollowing tasks
	SupervisedRule<int> leftBumperHit(LEFT_BUMPER_PIN, isBumperHit, bumperHit); //TODO make pins const int again and use a dummy variable
	SupervisedRule<int> rightBumperHit(RIGHT_BUMPER_PIN, isBumperHit, bumperHit);
	Supervisor.addRule(leftBumperHit);
	Supervisor.addRule(rightBumperHit);
	
	while(!operationComplete && !emergencyAbort)
	{
		tapeFollower.followTape();
		Supervisor.checkRules();
	}
	if(emergencyAbort)
	{
	
	}
	else //Successfully reached depot
	{
		robotStateMachine.transitionTo(DetectBlockInDepot); //TODO consider a state change immediately here if circular references can be avoided
	}	
}

void travelFromDepot() {}
void detectBlockInDepot() {}
void detectBlockInBuildArea() {}
void placeBlockOnGround() {}
void stackBlocks() {} 
void pickUpBlock() {}

