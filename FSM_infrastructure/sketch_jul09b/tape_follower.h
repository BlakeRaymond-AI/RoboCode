#ifndef TAPE_FOLLOWER_HEADER_GUARD
#define TAPE_FOLLOWER_HEADER_GUARD

#include <FiniteStateMachine.h>
#include <io_helper_functions.h>
#include <signal.h>
#include <observer.h>
#include <pins.h>
#include <drive_system.h>

enum TAPEFOLLOWING_CONSTANTS 
{
  initialSpeed = 300,
  initialProportionalGain = 210,
  initialDerivGain = 400,
  initialMaxError = 3
};

class TapeFollower
{
public:
  TapeFollower()
  : kP(initialProportionalGain),
    kD(initialDerivGain),
    kI(0),
    baseSpeed(initialSpeed),
    error(0),
    previousError1(0),
    previousError2(0),
    correction(0),
    previousCorrection(0),
    count(0),
    leftMotorSpeed(0),
    rightMotorSpeed(0),
    maxError(initialMaxError),
    turnBias(1)
    {
		
    }
	
	void turnAround()
	{
	  DRIVE_SYSTEM.drive(-baseSpeed, -baseSpeed);
	  delay(500);
	  DRIVE_SYSTEM.stop();
                
	  while(OBSERVER.rightOutboardQRD.belowThreshold()) //Turn left until the left outboard QRD hits tape
	  {
		  DRIVE_SYSTEM.turnLeft(baseSpeed);
		  OBSERVER.rightOutboardQRD.read();
	  }
          DRIVE_SYSTEM.stop();
          
	  while(OBSERVER.rightQRD.belowThreshold()) //Turn left slower until the left QRD hits tape
	  {
		  DRIVE_SYSTEM.turnLeft(275);
		  OBSERVER.rightQRD.read();
	  }
          DRIVE_SYSTEM.stop();

	  while(OBSERVER.rightQRD.aboveThreshold()) //Inch left until the left QRD is just off the tape (and the robot is straight)
	  {
		  DRIVE_SYSTEM.turnLeft(240);
		  OBSERVER.rightQRD.read();
	  }
          DRIVE_SYSTEM.stop();
	}
	
	void enable() //Enable tape following by adding the sensors to the observer
	{
		OBSERVER.leftQRD.enable();
		OBSERVER.rightQRD.enable();
		OBSERVER.leftOutboardQRD.enable();
		OBSERVER.rightOutboardQRD.enable();
	}
	
	void disable() //Disable tape following by removing the sensors from the observer
	{
		DRIVE_SYSTEM.stop();
		OBSERVER.leftQRD.disable();
		OBSERVER.rightQRD.disable();
		OBSERVER.leftOutboardQRD.disable();
		OBSERVER.rightOutboardQRD.disable();
	}
	
int getError()
	{
		if(OBSERVER.leftQRD.aboveThreshold() && OBSERVER.rightQRD.aboveThreshold()) error = 0;
		else if(OBSERVER.leftQRD.belowThreshold() && OBSERVER.rightQRD.aboveThreshold()) error = 1; //Left off tape, turn right
		else if(OBSERVER.leftQRD.aboveThreshold() && OBSERVER.rightQRD.belowThreshold()) error = -1; //Right off tape, turn left
		else if(OBSERVER.leftQRD.belowThreshold() && OBSERVER.rightQRD.belowThreshold()) //Both off tape -- use history or make a hard turn
		{
			if(previousError1>0) 
				error = maxError;
			else if(previousError1<0)
				error = -maxError;
			else
			  {
				  error = maxError * turnBias;
			  }
				
		}
	}

  void followTape()
  {
      enable();
    getError();
    
	correction = previousCorrection + kP * (error - previousError1) + kD * (error - 2 * previousError1 + previousError2) + kI * (error + previousError1) / 2.0;

	if(correction > 0) //Turn right -- Decrease right motor speed
	{
		leftMotorSpeed = baseSpeed;
		rightMotorSpeed = baseSpeed - correction;
	}
	else //Turn left -- Decrease left motor speed
	{
		leftMotorSpeed = baseSpeed + correction;
		rightMotorSpeed = baseSpeed;
	}

	DRIVE_SYSTEM.drive(leftMotorSpeed, rightMotorSpeed);

	++count;

	if(count == 1000)
	{
	  display();
	  count = 0;
	}	

	previousCorrection = correction;
	previousError2 = previousError1;
	previousError1 = error;
  }

  void display()
  {
    LCD.clear();
    LCD.home();
    LCD.print("L:" + String(OBSERVER.leftOutboardQRD.reading) + " ");
    LCD.print("R:" + String(OBSERVER.rightOutboardQRD.reading));
    LCD.setCursor(0,1);
    LCD.print("L:" + String(OBSERVER.leftQRD.reading) + " ");
    LCD.print("R:" + String(OBSERVER.rightQRD.reading));
  }
  
  char error;
  char previousError1;
  char previousError2;
  
  int correction;  
  int previousCorrection;
  
  int maxError;
  int count;
  
  char turnBias;
  
  int leftMotorSpeed;
  int rightMotorSpeed;
  
  int kP;
  int kD;
  int kI;
  int baseSpeed;
};

extern TapeFollower TAPEFOLLOWER;

#endif

