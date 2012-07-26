#include <WProgram.h>
#include <menu.h>

Menu::Menu(TapeFollower &tapeFollower)
:tapeFollower(tapeFollower)
{
    items.addElement(MenuItem("Prop: ", &tapeFollower.kP, 0, 512));
    items.addElement(MenuItem("Deriv: ", &tapeFollower.kD, 0, 512));
    items.addElement(MenuItem("Speed: ", &tapeFollower.baseSpeed, 0, 1023));
    items.addElement(MenuItem("QRDL: ", &tapeFollower.leftQRD.threshold, 0, 1023));
    items.addElement(MenuItem("QRDR: ", &tapeFollower.rightQRD.threshold, 0, 1023));
    items.addElement(MenuItem("QRDOL: ", &tapeFollower.leftOutboardQRD.threshold, 0, 1023));
    items.addElement(MenuItem("QRDOR: ", &tapeFollower.rightOutboardQRD.threshold, 0, 1023));
    items.addElement(MenuItem("MaxErr; ", &tapeFollower.maxError, 0, 10));
	items.addElement(MenuItem("MPDT: ", &MOVEMENT_CONTROL.millisPerDegreeTurn, 0, 20));
	items.addElement(MenuItem("Turn Spd: ", &MOVEMENT_CONTROL.turnSpeed, 0, 512));
	items.addElement(MenuItem("Bckup Spd: ", &MOVEMENT_CONTROL.backUpSpeed, 0, 512));
	items.addElement(MenuItem("Inch Spd: ", &MOVEMENT_CONTROL.inchSpeed, 0, 512));
	items.addElement(MenuItem("Bckup Tm: ", &MOVEMENT_CONTROL.backUpTime, 0, 2000));
}

Menu MENU(TAPEFOLLOWER);

