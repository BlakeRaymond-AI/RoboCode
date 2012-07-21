#include <WProgram.h>
#include <menu.h>

Menu::Menu(TapeFollower &tapeFollower)
:tapeFollower(tapeFollower)
{
    items.addElement(MenuItem("Full Speed: ", &tapeFollower.baseSpeed));
    items.addElement(MenuItem("QRDL: ", &tapeFollower.leftQRD.threshold));
    items.addElement(MenuItem("QRDR: ", &tapeFollower.rightQRD.threshold));
}

Menu MENU(TAPEFOLLOWER);

