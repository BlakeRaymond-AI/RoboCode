#ifndef LIFTER_HEADER_GUARD
#define LIFTER_HEADER_GUARD

#include <signal.h>
#include <pins.h>
#include <observer.h>

enum LifterPosition
{
	LOWERED,
	STACKING,
	RAISED
};

class Lifter
{
	public:
		Lifter()
		:bottomSwitch(LIFTER_SWITCH_BOT),
		stackingSwitch(LIFTER_SWITCH_MID),
		topSwitch(LIFTER_SWITCH_TOP),
                currentPosition(STACKING),
                targetPosition(LOWERED)
		{
		
		}
		
		void enable()
		{
			OBSERVER.addSignal((Signal*)&bottomSwitch);
			OBSERVER.addSignal((Signal*)&stackingSwitch);
			OBSERVER.addSignal((Signal*)&topSwitch);
		}
		
		void disable()
		{
			OBSERVER.removeSignal((Signal*)&bottomSwitch);
			OBSERVER.removeSignal((Signal*)&stackingSwitch);
			OBSERVER.removeSignal((Signal*)&topSwitch);
		}		
		
		void setTargetPosition(LifterPosition target)
		{
			targetPosition = target;
		}
		
		void update()
		{
			updatePosition();
			
			if(targetPosition > currentPosition) //Higher
			{
				raiseStage();
			}
			else if(targetPosition < currentPosition)
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
		
		DigitalSignal bottomSwitch;
		DigitalSignal stackingSwitch;
		DigitalSignal topSwitch;
		
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
				currentPosition = LOWERED;
			}
			else if(stackingSwitch.on())
			{
				currentPosition = STACKING;
			}
			else if(topSwitch.on())
			{
				currentPosition = RAISED;
			}
		}
	
};

extern Lifter LIFTER;

#endif
