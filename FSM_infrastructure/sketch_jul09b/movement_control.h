#ifndef MOVEMENT_CONTROL_HEADER
#define MOVEMENT_CONTROL_HEADER

#include <pins.h>
#include <signal.h>
#include <tape_follower.h>

enum INITIAL_MOVEMENT_CONTROL_CONSTANTS
{
	initialBackUpSpeed = 512,
	initialTurnSpeed = 512,
	initialInchSpeed = 100,
	initialBackUpTime = 500
};

const float initialMillisPerDegreeTurn = 1000.0/360.0;

class MovementControl
{
	public:
	MovementControl()
	: leftBumper(LEFT_BUMPER),
        rightBumper(RIGHT_BUMPER),
	backUpSpeed(initialBackUpSpeed),
	turnSpeed(initialTurnSpeed),
	inchSpeed(initialInchSpeed),
	backUpTime(initialBackUpTime)
	{}

        void enable()
        {
           OBSERVER.addSignal((Signal*)&leftBumper);
           OBSERVER.addSignal((Signal*)&rightBumper);
        }
        
        void disable()
        {
           OBSERVER.removeSignal((Signal*)&leftBumper);
           OBSERVER.removeSignal((Signal*)&rightBumper);
        }
	
	void backUp()
	{
		motor.speed(LEFT_DRIVE_MOTOR, -backUpSpeed);
		motor.speed(RIGHT_DRIVE_MOTOR, -backUpSpeed);
		delay(backUpTime);
		motor.stop(LEFT_DRIVE_MOTOR);
		motor.stop(RIGHT_DRIVE_MOTOR);
	}
	
	void turnLeft(int degrees)
	{
		int turnTime = degrees * millisPerDegreeTurn;
		motor.speed(LEFT_DRIVE_MOTOR, -turnSpeed);
		motor.speed(RIGHT_DRIVE_MOTOR, turnSpeed);
		delay(turnTime);
		motor.stop(LEFT_DRIVE_MOTOR);
		motor.stop(RIGHT_DRIVE_MOTOR);
	}
	
	void turnRight(int degrees)
	{
		int turnTime = degrees * millisPerDegreeTurn;
		motor.speed(LEFT_DRIVE_MOTOR, turnSpeed);
		motor.speed(RIGHT_DRIVE_MOTOR, -turnSpeed);
		delay(turnTime);
		motor.stop(LEFT_DRIVE_MOTOR);
		motor.stop(RIGHT_DRIVE_MOTOR);
	}
	
	void inchLeft()
	{
		motor.speed(LEFT_DRIVE_MOTOR, -inchSpeed);
		motor.speed(RIGHT_DRIVE_MOTOR, inchSpeed);
	}
	
	void inchRight()
	{
		motor.speed(LEFT_DRIVE_MOTOR, inchSpeed);
		motor.speed(RIGHT_DRIVE_MOTOR, -inchSpeed);
	}
	
	void forwardToTape()
	{
		do
		{
		TAPEFOLLOWER.leftOutboardQRD.read();
		TAPEFOLLOWER.rightOutboardQRD.read();
		
		motor.speed(LEFT_DRIVE_MOTOR, TAPEFOLLOWER.baseSpeed);
		motor.speed(RIGHT_DRIVE_MOTOR, TAPEFOLLOWER.baseSpeed);
		} while(TAPEFOLLOWER.leftOutboardQRD.belowThreshold() && TAPEFOLLOWER.rightOutboardQRD.belowThreshold());		
	}
	
	void forwardToDepot()
	{
		do
		{
		leftBumper.read();
		rightBumper.read();
		
		motor.speed(LEFT_DRIVE_MOTOR, TAPEFOLLOWER.baseSpeed);
		motor.speed(RIGHT_DRIVE_MOTOR, TAPEFOLLOWER.baseSpeed);
		} while(leftBumper.on() && rightBumper.on()); //NO switch closes on contact, bringing digital signal down
	}

        void stop()
        {
                motor.stop(LEFT_DRIVE_MOTOR);
                motor.stop(RIGHT_DRIVE_MOTOR);
        }

	int backUpSpeed;
	int turnSpeed;
	float millisPerDegreeTurn;
	int inchSpeed;
	int backUpTime;
	DigitalSignal leftBumper;
        DigitalSignal rightBumper;
};

extern MovementControl MOVEMENT_CONTROL;

#endif
