#ifndef STATE_MACHINE_HEADER_GUARD
#define STATE_MACHINE_HEADER_GUARD

void travelToDepot_Enter();
void travelToDepot_Update();
void travelToDepot_Exit();

void travelFromDepot_Enter();
void travelFromDepot_Update();
void travelFromDepot_Exit();

void errorHandling_TapeLost_Enter();
void errorHandling_TapeLost_Update();
void errorHandling_TapeLost_Exit();

void idle_Update();

State TravelToDepot 			= State(travelToDepot_Enter, travelToDepot_Update, travelToDepot_Exit);
State TravelFromDepot			= State(travelFromDepot_Enter, travelFromDepot_Update, travelFromDepot_Exit);
State Error_TapeLost			= State(errorHandling_TapeLost_Enter, errorHandling_TapeLost_Update, errorHandling_TapeLost_Exit);
State Idle						= State(idle_Update);

FSM robotStateMachine = FSM(TravelToDepot);

#endif