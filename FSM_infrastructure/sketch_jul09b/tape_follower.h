#ifndef TAPE_FOLLOWER_HEADER_GUARD
#define TAPE_FOLLOWER_HEADER_GUARD

#include <FiniteStateMachine.h>
#include <io_helper_functions.h>
#include <signal.h>
#include <observer.h>
#include <pins.h>

enum TAPEFOLLOWING_CONSTANTS 
{
  initialSpeed = 225,
  initialQRDThreshold = 450,
  initialProportionalGain = 175,
  initialDerivGain = 175,
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
    baseSpeed(initialSpeed),
	leftQRD(LEFT_TAPE_QRD, initialQRDThreshold),
	rightQRD(RIGHT_TAPE_QRD, initialQRDThreshold),
	leftOutboardQRD(LEFT_OUTBOARD_QRD, initialQRDThreshold),
	rightOutboardQRD(RIGHT_OUTBOARD_QRD, initialQRDThreshold),
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
		motor.speed(LEFT_DRIVE_MOTOR,-baseSpeed);
		motor.speed(RIGHT_DRIVE_MOTOR, -baseSpeed);
		delay(1000);
		stop();
	}
	
	void turnAround()
	{
		while(leftOutboardQRD.belowThreshold()) //Turn left until the left outboard QRD hits tape
		{
			motor.speed(LEFT_DRIVE_MOTOR, -baseSpeed);
			motor.speed(RIGHT_DRIVE_MOTOR, baseSpeed);
			leftOutboardQRD.read();
		}
		while(leftQRD.belowThreshold()) //Turn left slower until the left QRD hits tape
		{
			motor.speed(LEFT_DRIVE_MOTOR, -0.5*baseSpeed);
			motor.speed(RIGHT_DRIVE_MOTOR, 0.5*baseSpeed);
			leftQRD.read();
		}
		while(leftQRD.aboveThreshold()) //Inch left until the left QRD is just off the tape (and the robot is straight)
		{
			motor.speed(LEFT_DRIVE_MOTOR, -0.1*baseSpeed);
			motor.speed(RIGHT_DRIVE_MOTOR, 0.1*baseSpeed);
			leftQRD.read();
		}
	}
	
	void enable() //Enable tape following by adding the sensors to the observer
	{
		OBSERVER.addSignal(&leftQRD);
		OBSERVER.addSignal(&rightQRD);
		OBSERVER.addSignal(&leftOutboardQRD);
		OBSERVER.addSignal(&rightOutboardQRD);
	}
	
	void disable() //Disable tape following by removing the sensors from the observer
	{
		stop();
		OBSERVER.removeSignal(&leftQRD);
		OBSERVER.removeSignal(&rightQRD);
		OBSERVER.removeSignal(&leftOutboardQRD);
		OBSERVER.removeSignal(&rightOutboardQRD);
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
		
		motor.speed(LEFT_DRIVE_MOTOR, baseSpeed);
		motor.speed(RIGHT_DRIVE_MOTOR, baseSpeed);
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
		
		motor.speed(LEFT_DRIVE_MOTOR, baseSpeed);
		motor.speed(RIGHT_DRIVE_MOTOR, baseSpeed);
		turnBias = NONE;
	}
	
  boolean followTape()
  {
    //Proportional control
    byte error = 0;    
    if(leftQRD.aboveThreshold() && rightQRD.aboveThreshold()) error = 0; //Both on tape
    else if(leftQRD.belowThreshold() && rightQRD.aboveThreshold()) error = 1; //Left off tape, turn right
    else if(leftQRD.aboveThreshold() && rightQRD.belowThreshold()) error = -1; //Right off tape, turn left
    else //if(leftQRD.aboveThreshold() && rightQRD.aboveThreshold()) //Both off tape -- use hisory or make a hard turn
    {
		if(turnBias == NONE)
		{
		  if(lastError>0) error = 3; //Right on tape last; turn right
		  else if(lastError<0) error = -3; //Left on tape last; turn left
		  else
		  {
			return false; //Not successful
		  }
		}
		else if(turnBias == LEFT)
		{
			makeHardLeft();
		}
		else if(turnBias == RIGHT)
		{
			makeHardRight();
		}		
    }
    int proportional=kP*error;

    //Derivative estimation
    if(error != lastError)
    {
      lastTime=time;
      time=0;
    }      
    int derivative=kD*(error-lastError)/((float)(time+lastTime));

    int correction = proportional + derivative;
    
    leftMotorSpeed = baseSpeed+correction;
    rightMotorSpeed = baseSpeed-correction;

    motor.speed(LEFT_DRIVE_MOTOR, leftMotorSpeed);
    motor.speed(RIGHT_DRIVE_MOTOR, rightMotorSpeed);
    lastError = error;
    ++time;
    ++count;

    if(count == 100)
    {
      display();
      count = 0;
    }
    
    return true;
  }

  void display()
  {
    LCD.clear();
    LCD.home();
    LCD.print("L:" + String(leftMotorSpeed) + " ");
    LCD.print("R:" + String(rightMotorSpeed));
    LCD.setCursor(0,1);
    LCD.print("L:" + String(leftQRD.reading) + " ");
    LCD.print("R:" + String(rightQRD.reading));
  }
  
  Signal leftQRD;
  Signal rightQRD;
  Signal leftOutboardQRD;
  Signal rightOutboardQRD;

  int time;
  int lastTime;
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
  int baseSpeed;

};

extern TapeFollower TAPEFOLLOWER;

#endif

