#include <WProgram.h>
#include <menu.h>

Menu::Menu(TapeFollower &tapeFollower)
:tapeFollower(tapeFollower)
{
    items.addElement(MenuItem("Prop: ", &tapeFollower.kP));
    items.addElement(MenuItem("Deriv: ", &tapeFollower.kD));
    items.addElement(MenuItem("Speed: ", &tapeFollower.baseSpeed));
    items.addElement(MenuItem("QRDL: ", &tapeFollower.leftQRD.threshold));
    items.addElement(MenuItem("QRDR: ", &tapeFollower.rightQRD.threshold));
	items.addElement(MenuItem("MPDT: ", &MOVEMENT_CONTROL.millisPerDegreeTurn));
	items.addElement(MenuItem("Turn Spd: ", &MOVEMENT_CONTROL.turnSpeed));
	items.addElement(MenuItem("Bckup Spd: ", &MOVEMENT_CONTROL.backUpSpeed));
}

Menu MENU(TAPEFOLLOWER);

