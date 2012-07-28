#ifndef GRIPPER_HEADER_GUARD
#define GRIPPER_HEADER_GUARD

#include <signal.h>
#include <observer.h>
#include <pins.h>

class Gripper
{
  public:
	Gripper()
	: leftClawMicroswitch(GRIPPER_LEFT_SWITCH),
	rightClawMicroswitch(GRIPPER_RIGHT_SWITCH),
	positionCounter(0),
        isClosed(false),
        isOpen(false),
        maxPositionCounter(200000)
	{
	
	}

	void enable()
	{
		OBSERVER.addSignal((Signal*)&leftClawMicroswitch);
		OBSERVER.addSignal((Signal*)&rightClawMicroswitch);
	}
	
	void disable()
	{
		OBSERVER.removeSignal((Signal*)&leftClawMicroswitch);
		OBSERVER.removeSignal((Signal*)&rightClawMicroswitch);
	}
	
	bool switchesClosed()
	{
		return leftClawMicroswitch.on() && rightClawMicroswitch.on();
	}
	
	void grip()
	{
                isOpen = false;
		while(!isClosed)
		{
			leftClawMicroswitch.read();
			rightClawMicroswitch.read();
			
			if(switchesClosed())
			{
				delay(500);
				motor.stop(GRIPPER_MOTOR);
				isClosed = true;
			}
			else if(positionCounter >= maxPositionCounter)
			{
				motor.stop(GRIPPER_MOTOR);
				isClosed = true;
			}
			else
			{
				closeJaw();
				++positionCounter;
				isClosed = false;
			}
		}
	}
	
	void closeJaw()
	{
		motor.speed(GRIPPER_MOTOR, MAX_MOTOR_SPEED);
	}
	
	void openJaw()
	{
		motor.speed(GRIPPER_MOTOR, -MAX_MOTOR_SPEED);
	}
	
	void open()
	{
                positionCounter *= 1.40;
                isClosed = false;
		while(!isOpen)
		{
			if(positionCounter > 0)
			{
				openJaw();
				--positionCounter;
				isOpen = false;
			}
			else
			{
				motor.stop(GRIPPER_MOTOR);
				isOpen = true;
			}
		}
	}
	
	DigitalSignal leftClawMicroswitch;
	DigitalSignal rightClawMicroswitch;
	unsigned long positionCounter;
	boolean isClosed;
	boolean isOpen;
        unsigned long maxPositionCounter;
};

extern Gripper GRIPPER;

#endif
