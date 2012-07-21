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
}

Menu MENU(TAPEFOLLOWER);

