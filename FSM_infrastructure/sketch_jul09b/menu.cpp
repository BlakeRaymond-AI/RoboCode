#include <WProgram.h>
#include <menu.h>

Menu::Menu()
{
    items.addElement(MenuItem("TF_KP: ", &TAPEFOLLOWER.kP, 0, 512));
    items.addElement(MenuItem("TF_KD: ", &TAPEFOLLOWER.kD, 0, 2000));
    items.addElement(MenuItem("TF_SPD: ", &TAPEFOLLOWER.baseSpeed, 0, 1023));
    items.addElement(MenuItem("TF_MaxErr; ", &TAPEFOLLOWER.maxError, 0, 10));
	
    items.addElement(MenuItem("QRDL: ", &OBSERVER.leftQRD.threshold, 0, 1023));
    items.addElement(MenuItem("QRDR: ", &OBSERVER.rightQRD.threshold, 0, 1023));
    items.addElement(MenuItem("QRDOL: ", &OBSERVER.leftOutboardQRD.threshold, 0, 1023));
    items.addElement(MenuItem("QRDOR: ", &OBSERVER.rightOutboardQRD.threshold, 0, 1023));
	
	items.addElement(MenuItem("MC_MPDT: ", &MOVEMENT_CONTROL.millisPerDegreeTurn, 0, 20));
	items.addElement(MenuItem("MC_TurnSpd: ", &MOVEMENT_CONTROL.turnSpeed, 0, 512));
	items.addElement(MenuItem("MC_BckupSpd: ", &MOVEMENT_CONTROL.backUpSpeed, 0, 512));
	items.addElement(MenuItem("MC_BckupTm: ", &MOVEMENT_CONTROL.backUpTime, 0, 2000));
	
	items.addElement(MenuItem("RF_GapThrsh: ", &RANGEFINDERS.gapThreshold, 0, 1023));
	items.addElement(MenuItem("RF_EdgThrsh: ", &RANGEFINDERS.edgeThreshold, 0, 512));
	items.addElement(MenuItem("RF_MaxPanTime: ", &RANGEFINDERS.maxPanTime, 0, 10000));
	
	items.addElement(MenuItem("DS_LMotorC: ", &DRIVE_SYSTEM.leftMotor.correction, 0.0, 2.0));
	items.addElement(MenuItem("DS_RMotorC: ", &DRIVE_SYSTEM.rightMotor.correction, 0.0, 2.0));
	items.addElement(MenuItem("DS_InertiaC: ", &DRIVE_SYSTEM.inertiaCorrection, 0.0, 10.0));
}

Menu MENU;
