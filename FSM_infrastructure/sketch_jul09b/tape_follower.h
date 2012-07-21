#ifndef TAPE_FOLLOWER_HEADER_GUARD
#define TAPE_FOLLOWER_HEADER_GUARD

#include <FiniteStateMachine.h>
#include <io_helper_functions.h>
#include <signal.h>
#include <observer.h>
#include <pins.h>

enum TAPEFOLLOWING_CONSTANTS 
{
  initialSpeed = 200,
  initialQRDThresholdL = 200,
  initialQRDThresholdR = 200,
  initialProportionalGain = 110,
  initialDerivGain = 60,
  SHARP_TURN_SPEED = 250
};

enum TurningBias 
{
	LEFT,
	RIGHT,
	NONE
};

class TapeFollower
{
public:
  TapeFollower()
  : kP(initialProportionalGain),
    kD(initialDerivGain),
    fullSpeed(initialSpeed),
	leftQRD(LEFT_TAPE_QRD, initialQRDThresholdL),
	rightQRD(RIGHT_TAPE_QRD, initialQRDThresholdR),
	leftOutboardQRD(LEFT_OUTBOARD_QRD, initialQRDThresholdL),
	rightOutboardQRD(RIGHT_OUTBOARD_QRD, initialQRDThresholdR),
    time(0),
    lastTime(0),
    error(0),
    lastError(0),
    count(0),
    turnBias(NONE),
    leftMotorSpeed(0),
    rightMotorSpeed(0)
    {
		
    }
	
	void stop()
	{
		motor.speed(LEFT_DRIVE_MOTOR,0);
		motor.speed(RIGHT_DRIVE_MOTOR,0);
	}
	
	void backUp()
	{
		motor.speed(LEFT_DRIVE_MOTOR,-fullSpeed);
		motor.speed(RIGHT_DRIVE_MOTOR, -fullSpeed);
		delay(1000);
		stop();
	}
	
	void turnAround()
	{
		while(leftOutboardQRD.belowThreshold()) //Turn left until the left outboard QRD hits tape
		{
			motor.speed(LEFT_DRIVE_MOTOR, -fullSpeed);
			motor.speed(RIGHT_DRIVE_MOTOR, fullSpeed);
			leftOutboardQRD.read();
		}
		while(leftQRD.belowThreshold()) //Turn left slower until the left QRD hits tape
		{
			motor.speed(LEFT_DRIVE_MOTOR, -0.5*fullSpeed);
			motor.speed(RIGHT_DRIVE_MOTOR, 0.5*fullSpeed);
			leftQRD.read();
		}
		while(leftQRD.aboveThreshold()) //Inch left until the left QRD is just off the tape (and the robot is straight)
		{
			motor.speed(LEFT_DRIVE_MOTOR, -0.1*fullSpeed);
			motor.speed(RIGHT_DRIVE_MOTOR, 0.1*fullSpeed);
			leftQRD.read();
		}
	}
	
	void enable() //Enable tape following by adding the sensors to the observer
	{
		OBSERVER.addSignal(&leftQRD);
		OBSERVER.addSignal(&rightQRD);
	}
	
	void disable() //Disable tape following by removing the sensors from the observer
	{
		stop();
		OBSERVER.removeSignal(&leftQRD);
		OBSERVER.removeSignal(&rightQRD);
	}
	
	boolean followTapeRightBiased()
	{
		if(rightOutboardQRD.aboveThreshold()) //Outboard has crossed tape; next time both sensors are off, make a hard right
		{
			turnBias = RIGHT;
		}
		return followTape();
	}
	
	boolean followTapeLeftBiased()
	{
		if(leftOutboardQRD.aboveThreshold()) //Outboard has crossed tape; next time both sensors are off, make a hard left
		{
			turnBias = LEFT;
		}
		return followTape();		
	}
	
	void makeHardLeft()
	{
		while(leftQRD.belowThreshold())
		{
			motor.speed(LEFT_DRIVE_MOTOR, -SHARP_TURN_SPEED);
			motor.speed(RIGHT_DRIVE_MOTOR, SHARP_TURN_SPEED);
			leftQRD.read();
		}
		
		motor.speed(LEFT_DRIVE_MOTOR, fullSpeed);
		motor.speed(RIGHT_DRIVE_MOTOR, fullSpeed);
		turnBias = NONE;
	}
	
	void makeHardRight()
	{
		while(rightQRD.aboveThreshold())
		{
			motor.speed(LEFT_DRIVE_MOTOR, SHARP_TURN_SPEED);
			motor.speed(RIGHT_DRIVE_MOTOR, -SHARP_TURN_SPEED);
			rightQRD.read();
		}
		
		motor.speed(LEFT_DRIVE_MOTOR, fullSpeed);
		motor.speed(RIGHT_DRIVE_MOTOR, fullSpeed);
		turnBias = NONE;
	}
	
	int getError()
	{
		if(leftQRD.aboveThreshold() && rightQRD.aboveThreshold()) error = 0;
		else if(leftQRD.belowThreshold() && rightQRD.aboveThreshold()) error = 1; //Left off tape, turn right
		else if(leftQRD.aboveThreshold() && rightQRD.belowThreshold()) error = -1; //Right off tape, turn left
		else if(leftQRD.belowThreshold() && rightQRD.belowThreshold()) //Both off tape -- use history or make a hard turn
		{
			if(previousError1>0) 
				error = 3;
			else 
				error = -3;
		}
	}
	
	void limitMotorOutput()
	{
		if(leftMotorSpeed < -MAX_MOTOR_SPEED)
		{
			leftMotorSpeed = -MAX_MOTOR_SPEED;
		}
		else if(leftMotorSpeed > MAX_MOTOR_SPEED)
		{
			leftMotorSpeed = MAX_MOTOR_SPEED;
		}
		
		if(rightMotorSpeed < -MAX_MOTOR_SPEED)
		{
			rightMotorSpeed = -MAX_MOTOR_SPEED;
		}
		else if(rightMotorSpeed > MAX_MOTOR_SPEED)
		{
			rightMotorSpeed = MAX_MOTOR_SPEED;
		}
	}
	
	void driveMotors()
	{	
		limitMotorOutput();
	
		motor.speed(LEFT_DRIVE_MOTOR, leftMotorSpeed);
		motor.speed(RIGHT_DRIVE_MOTOR, rightMotorSpeed);
	}
	
  boolean followTape()
  {
    getError();
    
	correction = previousCorrection + (kP * 0.1) * (error - previousError1) + (kD * 0.1) * (error - 2 * previousError1 + previousError2);
	
	if(correction > 0) //Turn right -- Decrease right motor speed
	{
		leftMotorSpeed = fullSpeed;
		rightMotorSpeed = fullSpeed - correction;
	}
	else //Turn left -- Decrease left motor speed
	{
		leftMotorSpeed = fullSpeed + correction;
		rightMotorSpeed = fullSpeed;
	}
	
	driveMotors();

	++count;
    if(count == 1000)
    {
      display();
      count = 0;
    }	
	
	previousCorrection = correction;
	previousError2 = previousError1;
	previousError1 = error;
    
    return true;
  }

  void display()
  {
	LCD.setCursor(0,1);
    LCD.print("L:" + String(leftMotorSpeed) + " ");
    LCD.print("R:" + String(rightMotorSpeed));
  }
  
  Signal leftQRD;
  Signal rightQRD;
  Signal leftOutboardQRD;
  Signal rightOutboardQRD;

  int error;
  int lastError;
  int count;
  TurningBias turnBias;
  
  int leftMotorSpeed;
  int rightMotorSpeed;
  
  int QRDThresholdL;
  int QRDThresholdR;
  int kP;
  int kD;
  int fullSpeed;

};

extern TapeFollower TAPEFOLLOWER;

#endif

