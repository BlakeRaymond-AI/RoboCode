#include <phys253.h>       //   ***** from 253 template file
#include <LiquidCrystal.h> //   ***** from 253 template file
#include <Servo253.h>      //   ***** from 253 template file 

#include <FiniteStateMachine.h>
#include <Supervisor.h>

int LEFT_BUMPER_PIN = 0;
int RIGHT_BUMPER_PIN = 1;
const int LEFT_TAPE_QRD = 0;
const int RIGHT_TAPE_QRD = 1;
const int LEFT_DRIVE_MOTOR = 0;
const int RIGHT_DRIVE_MOTOR = 1;

enum FSM_STATES 
{
	TRAVEL_TO_DEPOT,
	TRAVEL_FROM_DEPOT,
	DETECT_BLOCK_IN_DEPOT,
	DETECT_BLOCK_IN_BUILD_AREA,
	PLACE_BLOCK_ON_GROUND,
	STACK_BLOCKS,
	PICK_UP_BLOCK
};

FSM_STATES nextState;
boolean operationComplete;
boolean emergencyAbort;

#include <state_transitions.h>

State TravelToDepot 			= State(travelToDepot);
State TravelFromDepot 			= State(travelFromDepot);
State DetectBlockInDepot 		= State(detectBlockInDepot);
State DetectBlockInBuildArea 	= State(detectBlockInBuildArea);
State PlaceBlockOnGround 		= State(placeBlockOnGround);
State StackBlocks 				= State(stackBlocks);
State PickUpBlock 				= State(pickUpBlock);

FSM robotStateMachine = FSM(TravelToDepot);

void setup() 
{

}

void loop() 
{
	operationComplete = false;
    robotStateMachine.update();
	Supervisor.checkRules();
}


