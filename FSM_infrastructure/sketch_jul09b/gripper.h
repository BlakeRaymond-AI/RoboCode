#ifndef GRIPPER_HEADER_GUARD
#define GRIPPER_HEADER_GUARD

#include <signal.h>
#include <observer.h>
#include <pins.h>

enum GripperPosition
{
  GRIPPER_OPEN = 0,
  GRIPPER_CLOSED = 1,
  GRIPPER_INDETERMINATE = 2
};

class Gripper
{
  public:
	Gripper()
	: positionCounter(0),
	position(GRIPPER_OPEN),
	maxPositionCounter(200000)
	{
	
	}

	void enable()
	{
		OBSERVER.gripperLeftSwitch.enable();
		OBSERVER.gripperRightSwitch.enable();
		OBSERVER.gripperCentreSwitch.enable();
	}
	
	void disable()
	{
		OBSERVER.gripperLeftSwitch.disable();
		OBSERVER.gripperRightSwitch.disable();
		OBSERVER.gripperCentreSwitch.disable();
	}
	
	bool switchesClosed()
	{
		return OBSERVER.gripperLeftSwitch.on() && OBSERVER.gripperRightSwitch.on();
	}
	
	void grip()
	{
		position = GRIPPER_INDETERMINATE;
		while(position != GRIPPER_CLOSED)
		{
			OBSERVER.gripperLeftSwitch.read();
			OBSERVER.gripperRightSwitch.read();
			
			if(switchesClosed())
			{
				delay(500);
				motor.stop(GRIPPER_MOTOR);
				position = GRIPPER_CLOSED;
			}
			else if(positionCounter >= maxPositionCounter)
			{
				motor.stop(GRIPPER_MOTOR);
				position = GRIPPER_CLOSED;
			}
			else
			{
				closeJaw();
				++positionCounter;
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
		positionCounter *= 1.40; //Correction factor to keep one open/close cycle at zero net change
		position = GRIPPER_INDETERMINATE;
		
		while(position != GRIPPER_OPEN)
		{
			if(positionCounter > 0)
			{
				openJaw();
				--positionCounter;
			}
			else
			{
				motor.stop(GRIPPER_MOTOR);
				position = GRIPPER_OPEN;
			}
		}
	}
	
	GripperPosition position;
	unsigned long positionCounter;
        unsigned long maxPositionCounter;
};

extern Gripper GRIPPER;

#endif
