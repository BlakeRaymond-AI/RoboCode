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
	initialBackUpTime = 500,
	initialMinMotorSpeed = 250
};

const float initialMillisPerDegreeTurn = 3.5;
const float initialLRBalance = 1.15;

class MovementControl
{
	public:
	MovementControl()
	: leftBumper(LEFT_BUMPER),
    rightBumper(RIGHT_BUMPER),
	backUpSpeed(initialBackUpSpeed),
	turnSpeed(initialTurnSpeed),
	inchSpeed(initialInchSpeed),
	backUpTime(initialBackUpTime),
    millisPerDegreeTurn(initialMillisPerDegreeTurn),
    LRBalance(initialLRBalance)
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
		motor.speed(RIGHT_DRIVE_MOTOR, -backUpSpeed*LRBalance);
		delay(backUpTime);
		
		motor.speed(LEFT_DRIVE_MOTOR, backUpSpeed);
		motor.speed(RIGHT_DRIVE_MOTOR, backUpSpeed*LRBalance);
		delay(inertiaCorrection);
		
		motor.stop(LEFT_DRIVE_MOTOR);
		motor.stop(RIGHT_DRIVE_MOTOR);
	}
	
	void turnLeft(int degrees)
	{
		int turnTime = degrees * millisPerDegreeTurn;
		motor.speed(LEFT_DRIVE_MOTOR, -turnSpeed);
		motor.speed(RIGHT_DRIVE_MOTOR, turnSpeed*LRBalance);
		delay(turnTime);
		
		motor.speed(LEFT_DRIVE_MOTOR, turnSpeed);
		motor.speed(RIGHT_DRIVE_MOTOR, -turnSpeed*LRBalance);
		delay(inertiaCorrection);
		
		motor.stop(LEFT_DRIVE_MOTOR);
		motor.stop(RIGHT_DRIVE_MOTOR);
	}
	
	void turnRight(int degrees)
	{	
		int turnTime = degrees * millisPerDegreeTurn;
		motor.speed(LEFT_DRIVE_MOTOR, turnSpeed);
		motor.speed(RIGHT_DRIVE_MOTOR, -turnSpeed*LRBalance);
		delay(turnTime);
		
		motor.speed(LEFT_DRIVE_MOTOR, -turnSpeed);
		motor.speed(RIGHT_DRIVE_MOTOR, turnSpeed*LRBalance);
		delay(inertiaCorrection);
		
		motor.stop(LEFT_DRIVE_MOTOR);
		motor.stop(RIGHT_DRIVE_MOTOR);
	}
	
	void inchLeft()
	{
		motor.speed(LEFT_DRIVE_MOTOR, -inchSpeed);
		motor.speed(RIGHT_DRIVE_MOTOR, inchSpeed*LRBalance);
	}
	
	void inchRight()
	{
		motor.speed(LEFT_DRIVE_MOTOR, inchSpeed);
		motor.speed(RIGHT_DRIVE_MOTOR, -inchSpeed*LRBalance);
	}
	
	void forwardToTape()
	{
		do
		{
		TAPEFOLLOWER.leftOutboardQRD.read();
		TAPEFOLLOWER.rightOutboardQRD.read();
		
		motor.speed(LEFT_DRIVE_MOTOR, TAPEFOLLOWER.baseSpeed);
		motor.speed(RIGHT_DRIVE_MOTOR, TAPEFOLLOWER.baseSpeed*LRBalance);
		} while(TAPEFOLLOWER.leftQRD.belowThreshold() && TAPEFOLLOWER.rightQRD.belowThreshold());		
		stop();
	}
	
	void forwardToDepot()
	{
		do
		{
		leftBumper.read();
		rightBumper.read();
		
		motor.speed(LEFT_DRIVE_MOTOR, TAPEFOLLOWER.baseSpeed);
		motor.speed(RIGHT_DRIVE_MOTOR, TAPEFOLLOWER.baseSpeed*LRBalance);
		} while(leftBumper.off() && rightBumper.off()); //NO switch closes on contact, bringing digital signal down
	}

	void stop()
	{
		motor.stop(LEFT_DRIVE_MOTOR);
		motor.stop(RIGHT_DRIVE_MOTOR);
	}

        bool hitDepot()
        {
          return leftBumper.on() || rightBumper.on();
        }

	int backUpSpeed;
	int turnSpeed;
	float millisPerDegreeTurn;
	int inchSpeed;
	int backUpTime;
	DigitalSignal leftBumper;
        DigitalSignal rightBumper;
        float LRBalance;
        int inertiaCorrection;
	int minMotorSpeed;
};

extern MovementControl MOVEMENT_CONTROL;

#endif
