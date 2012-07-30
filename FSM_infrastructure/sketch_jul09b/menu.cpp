#include <WProgram.h>
#include <menu.h>

Menu::Menu()
{
    items.addElement(MenuItem("TF_KP: ", &TAPEFOLLOWER.kP, 0, 512));
    items.addElement(MenuItem("TF_KD: ", &TAPEFOLLOWER.kD, 0, 2000));
    items.addElement(MenuItem("TF_SPD: ", &TAPEFOLLOWER.baseSpeed, 0, 1023));
	items.addElement(MenuItem("TF_MaxErr; ", &TAPEFOLLOWER.maxError, 0, 10));
	
    items.addElement(MenuItem("QRDL: ", &TAPEFOLLOWER.leftQRD.threshold, 0, 1023));
    items.addElement(MenuItem("QRDR: ", &TAPEFOLLOWER.rightQRD.threshold, 0, 1023));
    items.addElement(MenuItem("QRDOL: ", &TAPEFOLLOWER.leftOutboardQRD.threshold, 0, 1023));
    items.addElement(MenuItem("QRDOR: ", &TAPEFOLLOWER.rightOutboardQRD.threshold, 0, 1023));
	
	items.addElement(MenuItem("MC_MPDT: ", &MOVEMENT_CONTROL.millisPerDegreeTurn, 0, 20));
	items.addElement(MenuItem("MC_TurnSpd: ", &MOVEMENT_CONTROL.turnSpeed, 0, 512));
	items.addElement(MenuItem("MC_BckupSpd: ", &MOVEMENT_CONTROL.backUpSpeed, 0, 512));
	items.addElement(MenuItem("MC_BckupTm: ", &MOVEMENT_CONTROL.backUpTime, 0, 2000));
	
	items.addElement(MenuItem("RF_GapThrsh: ", &RANGEFINDERS.gapThreshold, 0, 1023));
	items.addElement(MenuItem("RF_EdgThrsh: ", &RANGEFINDERS.edgeThreshold, 0, 512));
	items.addElement(MenuItem("RF_MaxPanTime: ", &RANGEFINDERS.maxPanTime, 0, 10000));
}

Menu MENU();

    int gapThreshold;
    int edgeThreshold;
    int maxPanTime;