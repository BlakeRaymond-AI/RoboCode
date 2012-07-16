#ifndef TAPE_FOLLOWER_HEADER_GUARD
#define TAPE_FOLLOWER_HEADER_GUARD

#include <io_helper_functions.h>

enum INITIAL_TAPEFOLLOWING_CONSTANTS 
{
  initialSpeed = 400,
  initialQRDThreshold = 200
  initialProportionalGain = 480,
  initialDerivGain = 230
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
  : 
    kP(initialProportionalGain),
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
	turnBias(NONE)
    {
		
    }
	
	void stop()
	{
		motors.speed(LEFT_DRIVE_MOTOR,0);
		motors.speed(RIGHT_DRIVE_MOTOR,0);
	}
	
	void backUp()
	{
		motors.speed(LEFT_DRIVE_MOTOR,-baseSpeed);
		motors.speed(RIGHT_DRIVE_MOTOR, -baseSpeed);
		delay(1000);
		stop();
	}
	
	void turnAround()
	{
		while(leftOutboardQRD.aboveThreshold()) //Turn left until the left outboard QRD hits tape
		{
			motors.speed(LEFT_DRIVE_MOTOR, -baseSpeed);
			motors.speed(RIGHT_DRIVE_MOTOR, baseSpeed);
			leftOutboardQRD.read();
		}
		while(leftQRD.aboveThreshold()) //Turn left slower until the left QRD hits tape
		{
			motors.speed(LEFT_DRIVE_MOTOR, -0.5*baseSpeed);
			motors.speed(RIGHT_DRIVE_MOTOR, 0.5*baseSpeed);
			leftQRD.read();
		}
		while(leftQRD.belowThreshold()) //Inch left until the left QRD is just off the tape (and the robot is straight)
		{
			motors.speed(LEFT_DRIVE_MOTOR, -0.1*baseSpeed);
			motors.speed(RIGHT_DRIVE_MOTOR, 0.1*baseSpeed);
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
	
	void followTapeRightBiased()
	{
		if(rightOutboardQRD.belowThreshold()) //Outboard has crossed tape; next time both sensors are off, make a hard right
		{
			turnBias = RIGHT;
		}
		followTape();
	}
	
	void followTapeLeftBiased()
	{
		if(leftOutboardQRD.belowThreshold()) //Outboard has crossed tape; next time both sensors are off, make a hard left
		{
			turnBias = LEFT;
		}
		followTape();		
	}
	
	void makeHardLeft()
	{
		while(leftQRD.aboveThreshold())
		{
			motor.speed(LEFT_DRIVE_MOTOR, -SHARP_TURN_SPEED);
			motor.speed(RIGHT_DRIVE_MOTOR, SHARP_TURN_SPEED);
			leftQRD.read();
		}
		
		motor.speed(LEFT_DRIVE_MOTOR, speed);
		motor.speed(RIGHT_DRIVE_MOTOR, speed);
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
		
		motor.speed(LEFT_DRIVE_MOTOR, speed);
		motor.speed(RIGHT_DRIVE_MOTOR, speed);
		turnBias = NONE;
	}
	
	void errorTapeLost()
	{
		robotStateMachine.transitionTo(Error_TapeLost);
	}

  void followTape()
  {
    //Proportional control
    byte error = 0;    
    if(leftQRD.belowThreshold() && rightQRD.belowThreshold()) error = 0; //Both on tape
    else if(leftQRD.aboveThreshold() && rightQRD.belowThreshold()) error = 1; //Left off tape, turn right
    else if(leftQRD.belowThreshold() && rightQRD.aboveThreshold()) error = -1; //Right off tape, turn left
    else //if(leftQRD.aboveThreshold() && rightQRD.aboveThreshold()) //Both off tape -- use hisory or make a hard turn
    {
		if(turnBias == NONE)
		{
		  if(lastError>0) error = 3; //Right on tape last; turn right
		  else if(lastError<0) error = -3; //Left on tape last; turn left
		  else
		  {
			errorTapeLost();
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

    int correction = proportional + derivative + turn;

    motor.speed(LEFT_DRIVE_MOTOR, speed+correction);
    motor.speed(RIGHT_DRIVE_MOTOR, speed-correction);
    lastError = error;
    ++time;
    ++count;

    if(count == 100)
    {
      display();
      count = 0;
    }
  }

  void display()
  {
    LCD.clear();
    LCD.home();
    LCD.print("L:" + String(leftMotorSpeed) + " ");
    LCD.print("R:" + String(rightMotorSpeed));
    LCD.setCursor(0,1);
    LCD.print("L:" + String(leftQRD) + " ");
    LCD.print("R:" + String(rightQRD));
  }

private:
  int time;
  int lastTime;
  int error;
  int lastError;
  int count;
  TurningBias turnBias;
  
  int QRDThreshold;
  int kP;
  int kD;
  int baseSpeed;

  Signal leftQRD;
  Signal rightQRD;
  Signal leftOutboardQRD;
  Signal rightOutboardQRD;
  
  friend class Menu;
};

TapeFollower TAPEFOLLOWER;

#endif

