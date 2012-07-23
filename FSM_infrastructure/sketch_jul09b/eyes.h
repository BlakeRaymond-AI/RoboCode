#ifndef EYES_HEADER_GUARD
#define EYES_HEADER_GUARD

#include <pins.h>

FloatTable RANGEFINDER_DISTANCES = FloatTable
{
	
};

const float FLOAT_PRECISION = 1E-8;
const float DISTANCE_BETWEEN_EYES = 150.0; //mm
const int SERVO_RESPONSE_TIME = 250; //ms

class Rangefinder
{
public:
	Rangefinder(int pin)
	: rangefinder(pin, 0),
	distance(0.0)
	{}

	void computeDistance()
	{
		distance = RANGEFINDER_DISTANCES[rangefinder.reading];
		if(abs(currentDistance - distance) > FLOAT_PRECISION) //i.e. currentDistance != distance
		{
			lastDistance = currentDistance;
			currentDistance = distance;
		}
	}
	
	void update()
	{
		rangefinder.update();
		computeDistance();
	}
	
	float currentDistance;
	float lastDistance;
	Signal rangefinder;
};

class Eyes
{
public:
	Eyes()
	: leftRf(LEFT_RANGEFINDER),
	rightRf(RIGHT_RANGEFINDER)
	{}
	
	void updateServos()
	{
		RCServo1.write(leftRfAngle); //TODO determine the angle-to-angle correction
		RCServo2.write(rightRfAngle);
	}
	
	void point_right()
	{
		rightRfAngle = 180;
	}
	
	void point_left()
	{
		leftRfAngle = 180;
	}
	
	void point_straight()
	{
		leftRfAngle = 90;
		rightRfAngle = 90;
	}
	
	void compute_dY()
	{
		dY = ( leftRf.currentDistance * sin(leftRfAngle) + rightRf.currentDistance * sin(rightRfAngle) ) / 2.0;
	}
	
	void computeObjectWidth()
	{
		width = DISTANCE_BETWEEN_EYES - leftRf.currentDistance * cos(leftRfAngle) - rightRf.currentDistance * cos(rightRfAngle);
	}
	
	void refreshRangefinders()
	{
		leftRf.update();
		rightRf.update();
	}
	
	void panLeftRf()
	{
		leftRfAngle += 5*leftRfPanningDirection;
		if(leftRfAngle == 0 || leftRfAngle == 90)
			leftRfPanningDirection *= -1;
	}
	
	void panRightRf()
	{
		rightRfAngle += 5*rightRfPanningDirection;
		if(rightRfAngle == 0 || rightRfAngle == 90)
			rightRfPanningDirection *= -1;
	}	
	
	void track()
	{
		//TODO switch to PID control
		while(leftRf.rangefinder.aboveThreshold() || rightRf.rangefinder.aboveThreshold())
		{
			if(leftRf.rangefinder.aboveThreshold())
			{
				panLeftRf();
			}
			if(rightRf.rangefinder.aboveThreshold())
			{
				panRightRf();
			}
			
			updateServos();
			delay(SERVO_RESPONSE_TIME);
			refreshRangefinders();
		}
	}
	
private:
	int leftRfAngle;
	int rightRfAngle;
	
	int leftRfPanningDirection;
	int rightRfPanningDirection;
	
	Rangefinder leftRf;
	Rangefinder rightRf;
	
	float dY;
	float objectWidth;
};

#endif