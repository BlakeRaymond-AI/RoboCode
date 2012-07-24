#ifndef STATE_MACHINE_HEADER_GUARD
#define STATE_MACHINE_HEADER_GUARD

#include <FiniteStateMachine.h>
#include <io_helper_functions.h>
#include <tape_follower.h>
#include <state_history.h>
#include <menu.h>

#include <phys253.h>       //   ***** from 253 template file
#include <LiquidCrystal.h> //   ***** from 253 template file
#include <Servo253.h>      //   ***** from 253 template file

void travelToDepot_Enter();
void travelToDepot_Update();
void travelToDepot_Exit();

void travelFromDepot_Enter();
void travelFromDepot_Update();
void travelFromDepot_Exit();

void errorHandling_TapeLost_Enter();
void errorHandling_TapeLost_Update();
void errorHandling_TapeLost_Exit();

void dropBlock_Enter();
void dropBlock_Update();
void dropBlock_Exit();

void idle_Update();

extern State TravelToDepot;
extern State TravelFromDepot;
extern State FindBlock;
extern State DropBlock;

extern FSM robotStateMachine;
extern StateHistory STATE_HISTORY;



#endif
