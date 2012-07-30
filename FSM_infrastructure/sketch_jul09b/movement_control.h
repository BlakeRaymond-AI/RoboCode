#ifndef MOVEMENT_CONTROL_HEADER
#define MOVEMENT_CONTROL_HEADER

#include <pins.h>
#include <signal.h>
#include <tape_follower.h>

enum INITIAL_MOVEMENT_CONTROL_CONSTANTS
{
	initialBackUpSpeed = 512,
	initialTurnSpeed = 512,
	initialBackUpTime = 500,
};

const float initialMillisPerDegreeTurn = 3.5;

class MovementControl
{
	public:
	MovementControl()
	: backUpSpeed(initialBackUpSpeed),
	turnSpeed(initialTurnSpeed),
	backUpTime(initialBackUpTime),
    millisPerDegreeTurn(initialMillisPerDegreeTurn)
	{}

        void enable()
        {
           OBSERVER.leftBumper.enable();
           OBSERVER.rightBumper.enable();
        }
        
        void disable()
        {
           OBSERVER.leftBumper.disable();
           OBSERVER.rightBumper.disable();
        }
	
	void backUp()
	{
		DRIVE_SYSTEM.drive(-backUpSpeed, -backUpSpeed);
		delay(backUpTime);
		
		DRIVE_SYSTEM.stop();
	}
	
	void turnLeft(int degrees)
	{
		int turnTime = degrees * millisPerDegreeTurn;
		
		DRIVE_SYSTEM.turnLeft(turnSpeed);		
		delay(turnTime);
		
		DRIVE_SYSTEM.stop();
	}
	
	void turnRight(int degrees)
	{	
		int turnTime = degrees * millisPerDegreeTurn;
		
		DRIVE_SYSTEM.turnRight(turnSpeed);		
		delay(turnTime);
		
		DRIVE_SYSTEM.stop();
	}
	
	void forwardToTape()
	{
		while(OBSERVER.leftQRD.belowThreshold() && OBSERVER.rightQRD.belowThreshold());	
		{
				OBSERVER.leftOutboardQRD.read();
				OBSERVER.rightOutboardQRD.read();
		
				DRIVE_SYSTEM.drive(MEDIUM_MOTOR_SPEED, MEDIUM_MOTOR_SPEED);
		} 	
		DRIVE_SYSTEM.stop();
	}
	
	void forwardToDepot()
	{
		while(!hitDepot())
		{
				OBSERVER.leftBumper.read();
				OBSERVER.rightBumper.read();
		
				DRIVE_SYSTEM.drive(MEDIUM_MOTOR_SPEED, MEDIUM_MOTOR_SPEED);
		}
		DRIVE_SYSTEM.stop();
	}

		bool hitDepot()
		{
				return OBSERVER.leftBumper.on() || OBSERVER.rightBumper.on();
		}

	int backUpSpeed;
	int turnSpeed;
	float millisPerDegreeTurn;
	int backUpTime;
};

extern MovementControl MOVEMENT_CONTROL;

#endif
