#ifndef OBSERVER_HEADER_GUARD
#define OBSERVER_HEADER_GUARD

#include <signal.h>
#include <pins.h>

enum QRD_THRESHOLDS
{
  initialQRDThresholdL = 500,
  initialQRDThresholdR = 200,
  initialQRDThresholdOL = 200,
  initialQRDThresholdOR = 200,
};
  
class Observer
{
	public:
		Observer()
		: leftQRD(LEFT_TAPE_QRD, initialQRDThresholdL),
		rightQRD(RIGHT_TAPE_QRD, initialQRDThresholdR),
		leftOutboardQRD(LEFT_OUTBOARD_QRD, initialQRDThresholdOL),
		rightOutboardQRD(RIGHT_OUTBOARD_QRD, initialQRDThresholdOR),
		gripperLeftSwitch(GRIPPER_LEFT_SWITCH),
		gripperRightSwitch(GRIPPER_RIGHT_SWITCH),
		gripperCentreSwitch(GRIPPER_CENTRE_SWITCH),
		leftBumper(LEFT_BUMPER),
		rightBumper(RIGHT_BUMPER),
		centreBumper(CENTRE_BUMPER),
		slideTopSwitch(SLIDE_TOP_SWITCH),
		slideBottomSwitch(SLIDE_BOTTOM_SWITCH)
		{}
	
	void update()
	{
		leftQRD.read();
		rightQRD.read();
		leftOutboardQRD.read();
		rightOutboardQRD.read();
		gripperLeftSwitch.read();
		gripperRightSwitch.read();
		gripperCentreSwitch.read();
		leftBumper.read();
		rightBumper.read();
		centreBumper.read();
		slideTopSwitch.read();
		slideBottomSwitch.read();
	}
	
    AnalogSignal leftQRD;
    AnalogSignal rightQRD;
    AnalogSignal leftOutboardQRD;
    AnalogSignal rightOutboardQRD;
    DigitalSignal gripperLeftSwitch;
    DigitalSignal gripperRightSwitch;
    DigitalSignal gripperCentreSwitch;
    DigitalSignal leftBumper;
    DigitalSignal rightBumper;
    DigitalSignal centreBumper;
    DigitalSignal slideTopSwitch;
    DigitalSignal slideBottomSwitch;
};

extern Observer OBSERVER;

#endif
