#ifndef GRIPPER_HEADER_GUARD
#define GRIPPER_HEADER_GUARD

#include <signal.h>
#include <observer.h>

const int MAX_MOTOR_SPEED = 1023;

enum GripperState
{
	OPEN,
	CLOSED,
	HALFWAY
}

class Gripper
{
	Gripper()
	: leftClawMicroswitch(GRIPPER_LEFT_CLAW_MICROSWITCH),
	rightClawMicroswitch(GRIPPER_RIGHT_CLAW_MICROSWITCH),
	timeToGrip(0),
	gripStartTime(0)
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
	
	void advanceTime()
	{
		
	}
	
	boolean switchesClosed()
	{
		return leftClawMicroswitch.on() && rightClawMicroswitch.on();
	}
	
	void grip() //Returns whether or not it's gripped a block
	{
		if(switchesClosed())
		{
			motor.stop(GRIPPER_MOTOR);
			state	
		}
		else
		{
			closeJaw();
			++positionCounter;
		}
	}
	
	void closeJaw()
	{
		motor.speed(GRIPPER_MOTOR, MAX_MOTOR_SPEED);
	}
	
	void openJaw()
	{
		motor.speed(GRIPPER_MOTOR_, -MAX_MOTOR_SPEED);
	}
	
	void open()
	{
		gripped = false;
		if(positionCounter > 0)
		{
			openJaw();
			--positionCounter;
		}
		else
		{
			motor.stop(GRIPPER_MOTOR);
			open = true;
		}
	}
	
	DigitalSignal leftClawMicroswitch;
	DigitalSignal rightClawMicroswitch;
	GripperState state;
	unsigned long int positionCounter;
};

#endif