#ifndef MOVEMENT_CONTROL_HEADER
#define MOVEMENT_CONTROL_HEADER

enum INITIAL_MOVEMENT_CONTROL_CONSTANTS
{
	initialBackUpSpeed = 512,
	initialTurnSpeed = 512
};

const float initialMillisPerDegreeTurn = 1000.0/360.0;

class MovementControl
{
	public:
	MovementControl()
	: (leftBumper(LEFT_BUMPER)),
	rightBumper(RIGHT_BUMPER)
	{}
	
	void backUp(int millis)
	{
		motor.speed(LEFT_DRIVE_MOTOR, -backUpSpeed);
		motor.speed(RIGHT_DRIVE_MOTOR, -backUpSpeed);
		delay(millis);
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
	
	void forwardToTape()
	{
		do
		{
		TAPEFOLLOWER.leftOutboardQRD.read();
		TAPEFOLLOWER.rightOutboardQRD.read();
		
		motor.speed(LEFT_DRIVE_MOTOR, TAPEFOLLOWER.fullSpeed);
		motor.speed(RIGHT_DRIVE_MOTOR, TAPEFOLLOWER.fullSpeed);
		} while(TAPEFOLLOWER.leftOutboardQRD.belowThreshold() && TAPEFOLLOWER.rightOutboardQRD.belowThreshold());		
	}
	
	void forwardToDepot()
	{
		do
		{
		leftBumper.read();
		rightBumper.read();
		
		motor.speed(LEFT_DRIVE_MOTOR, TAPEFOLLOWER.fullSpeed);
		motor.speed(RIGHT_DRIVE_MOTOR, TAPEFOLLOWER.fullSpeed);
		} while(leftBumper.off() && rightBumper.off());
	}

	int backUpSpeed;
	int turnSpeed;
	float millisPerDegreeTurn;
	DigitalSignal leftBumper;
	DigitalSignal rightBumper;
};

extern MovementControl MOVEMENT_CONTROL;

#endif