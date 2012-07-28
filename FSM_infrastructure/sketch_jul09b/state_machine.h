#ifndef STATE_MACHINE_HEADER_GUARD
#define STATE_MACHINE_HEADER_GUARD

#include <FiniteStateMachine.h>
#include <io_helper_functions.h>
#include <tape_follower.h>
#include <state_history.h>
#include <menu.h>
#include <movement_control.h>
#include <gripper.h>
#include <lifter.h>

#include <phys253.h>       //   ***** from 253 template file
#include <LiquidCrystal.h> //   ***** from 253 template file
#include <Servo253.h>      //   ***** from 253 template file

void travelToDepot_Enter();
void travelToDepot_Update();
void travelToDepot_Exit();

void dropBlock_Enter();
void dropBlock_Update();
void dropBlock_Exit();

void findBlock_Enter();
void findBlock_Update();
void findBlock_Exit();

void findTape_Enter();
void findTape_Update();
void findTape_Exit();

void dropBlock_Enter();
void dropBlock_Exit();
void dropBlock_Update();

void findBlockInBuildArea_Enter();
void findBlockInBuildArea_Update();
void findBlockInBuildArea_Exit();

/*
void travelFromDepotToFirstTurn_Enter();
void travelFromDepotToFirstTurn_Update();
void travelFromDepotToFirstTurn_Exit();

void travelFromFirstTurnToBuildArea_Enter();
void travelFromFirstTurnToBuildArea_Update();
void travelFromFirstTurnToBuildArea_Exit();
*/

extern State TravelToDepot;
//extern State TravelFromDepotToFirstTurn;
//extern State TravelFromFirstTurnToBuildArea;
extern State FindBlock;
extern State DropBlock;
extern State FindTape;
extern State FindBlockInBuildArea;

extern FSM robotStateMachine;
extern StateHistory STATE_HISTORY;



#endif
