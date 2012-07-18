#include <WProgram.h>
#include <menu.h>

Menu::Menu(TapeFollower &tapeFollower)
{
        items.addElement(MenuItem("Prop: ", &tapeFollower.kP));
	items.addElement(MenuItem("Deriv: ", &tapeFollower.kD));
	items.addElement(MenuItem("Speed: ", &tapeFollower.baseSpeed));
	items.addElement(MenuItem("QRDL: ", &tapeFollower.QRDThresholdL));
	items.addElement(MenuItem("QRDR: ", &tapeFollower.QRDThresholdR));
}

Menu MENU(TAPEFOLLOWER);

