#include <WProgram.h>
#include <menu.h>

Menu::Menu(TapeFollower &tapeFollower)
:tapeFollower(tapeFollower)
{
    items.addElement(MenuItem("Full Speed: ", &tapeFollower.fullSpeed, 0, 1023));
    items.addElement(MenuItem("QRDL: ", &tapeFollower.leftQRD.threshold, 0, 1023));
    items.addElement(MenuItem("QRDR: ", &tapeFollower.rightQRD.threshold, 0, 1023));
	items.addElement(MenuItem("Max Error: ", &tapeFollower.maxError, 1, 10));
}

Menu MENU(TAPEFOLLOWER);

