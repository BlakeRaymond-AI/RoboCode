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
extern State FindBlock;
extern State TravelFromDepot;
extern State DropBlock;
extern State StackBlocks;

extern FSM robotStateMachine;

#endif
