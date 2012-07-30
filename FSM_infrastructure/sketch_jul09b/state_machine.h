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

extern State TravelToDepot;
extern State FindBlockInDepot;
extern State FindBlockInBuildArea;
extern State TravelFromDepot;
extern State DropBlock;
extern State StackBlocks;

void travelToDepot_Enter();
void travelToDepot_Update();
void travelToDepot_Exit();

void findBlockInDepot_Enter();
void findBlockInDepot_Update();
void findBlockInDepot_Exit();

void findBlockInBuildArea_Enter();
void findBlockInBuildArea_Update();
void findBlockInBuildArea_Exit();

void travelFromDepot_Enter();
void travelFromDepot_Update();
void travelFromDepot_Exit();

void dropBlock_Enter();
void dropBlock_Update();
void dropBlock_Exit();

void stackBlocks_Enter();
void stackBlocks_Update(); 
void stackBlocks_Exit();

extern FSM robotStateMachine;

#endif
