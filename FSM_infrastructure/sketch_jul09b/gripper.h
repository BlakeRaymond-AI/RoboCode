#ifndef GRIPPER_HEADER_GUARD
#define GRIPPER_HEADER_GUARD

#include <signal.h>
#include <observer.h>

const int MAX_MOTOR_SPEED = 1023;

class Gripper
{
	Gripper()
	: leftClawMicroswitch(GRIPPER_LEFT_SWITCH),
	rightClawMicroswitch(GRIPPER_RIGHT_SWITCH),
	positionCounter(0)
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
	
	boolean switchesClosed()
	{
		return leftClawMicroswitch.on() && rightClawMicroswitch.on();
	}
	
	void grip()
	{
		if(switchesClosed())
		{
			delay(500);
			motor.stop(GRIPPER_MOTOR);
			closed = true;
		}
		else if(positionCounter >= MAX_POS_COUNTER)
		{
			motor.stop(GRIPPER_MOTOR);
			closed = true;
		}
		else
		{
			closeJaw();
			++positionCounter;
			closed = false;
		}
	}
	
	void closeJaw();
	{
		motor.speed(GRIPPER_MOTOR, MAX_MOTOR_SPEED);
	}
	
	void openJaw();
	{
		motor.speed(GRIPPER_MOTOR_, -MAX_MOTOR_SPEED);
	}
	
	void open()
	{
		if(positionCounter > 0)
		{
			openJaw();
			--positionCounter;
			open = false;
		}
		else
		{
			motor.stop(GRIPPER_MOTOR);
			open = true;
		}
	}
	
	DigitalSignal leftClawMicroswitch;
	DigitalSignal rightClawMicroswitch;
	long int positionCounter;
	boolean closed;
	boolean open;
};

#endif