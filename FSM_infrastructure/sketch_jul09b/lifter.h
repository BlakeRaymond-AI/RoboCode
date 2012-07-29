#ifndef LIFTER_HEADER_GUARD
#define LIFTER_HEADER_GUARD

#include <signal.h>
#include <pins.h>
#include <observer.h>

enum LifterPosition
{
	LIFTER_LOWERED =0,
	LIFTER_RAISED =1,
	LIFTER_INDETERMINATE =2
};

class Lifter
{
	public:
		Lifter()
		: currentPosition(LIFTER_LOWERED),
		targetPosition(LIFTER_LOWERED)
		{
		
		}
		
		void enable()
		{
			OBSERVER.bottomSwitch.enable();
			OBSERVER.topSwitch.enable();
		}
		
		void disable()
		{
			OBSERVER.bottomSwitch.disable();
			OBSERVER.topSwitch.disable();
		}		
		
		void setTargetPosition(LifterPosition target)
		{
			targetPosition = target;
		}
		
		void update()
		{
			updatePosition();
			
			if(targetPosition == LIFTER_RAISED && currentPosition != LIFTER_RAISED) //Higher
			{
				raiseStage();
			}
			else if(targetPosition == LIFTER_LOWERED && currentPosition != LIFTER_LOWERED)
			{
				lowerStage();
			}
			else // targetPosition == currentPosition
			{
				stopStage();
			}
		}
		
		bool ready()
		{
			return currentPosition == targetPosition;
		}
		
		LifterPosition currentPosition;
		LifterPosition targetPosition;
		
	private:		
		void raiseStage()
		{
			motor.speed(LIFTER_MOTOR, MAX_MOTOR_SPEED);
		}
		
		void lowerStage()
		{
			motor.speed(LIFTER_MOTOR, -MAX_MOTOR_SPEED);
		}
		
		void stopStage()
		{
			motor.stop(LIFTER_MOTOR);
		}
		
		void updatePosition()
		{
			if(bottomSwitch.on())
			{
				currentPosition = LIFTER_LOWERED;
			}
			else if(topSwitch.on())
			{
				currentPosition = LIFTER_RAISED;
			}
			else
			{
				currentPosition = LIFTER_INDETERMINATE;
			}
		}	
};

extern Lifter LIFTER;

#endif
