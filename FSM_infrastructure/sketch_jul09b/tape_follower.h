#ifndef TAPE_FOLLOWER_HEADER_GUARD
#define TAPE_FOLLOWER_HEADER_GUARD

#include <FiniteStateMachine.h>
#include <io_helper_functions.h>
#include <signal.h>
#include <observer.h>
#include <pins.h>

enum TAPEFOLLOWING_CONSTANTS 
{
  initialSpeed = 300,
  initialQRDThresholdL = 500,
  initialQRDThresholdR = 200,
  initialQRDThresholdOL = 200,
  initialQRDThresholdOR = 200,
  initialProportionalGain = 210,
  initialDerivGain = 400,
  initialMaxError = 3,
  SHARP_TURN_SPEED = 250
};

class TapeFollower
{
public:
  TapeFollower()
  : kP(initialProportionalGain),
    kD(initialDerivGain),
    kI(0),
    baseSpeed(initialSpeed),
    leftQRD(LEFT_TAPE_QRD, initialQRDThresholdL),
    rightQRD(RIGHT_TAPE_QRD, initialQRDThresholdR),
    leftOutboardQRD(LEFT_OUTBOARD_QRD, initialQRDThresholdOL),
    rightOutboardQRD(RIGHT_OUTBOARD_QRD, initialQRDThresholdOR),
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
	
	void stop()
	{
		motor.speed(LEFT_DRIVE_MOTOR,0);
		motor.speed(RIGHT_DRIVE_MOTOR,0);
	}	
	
	void turnAround()
	{
                motor.speed(LEFT_DRIVE_MOTOR, -baseSpeed);
                motor.speed(RIGHT_DRIVE_MOTOR, -baseSpeed);
                delay(500);
                motor.speed(LEFT_DRIVE_MOTOR, baseSpeed);
                motor.speed(RIGHT_DRIVE_MOTOR, baseSpeed);
                delay(100);
                
		while(rightOutboardQRD.belowThreshold()) //Turn left until the left outboard QRD hits tape
		{
			motor.speed(LEFT_DRIVE_MOTOR, baseSpeed);
			motor.speed(RIGHT_DRIVE_MOTOR, -baseSpeed);
			rightOutboardQRD.read();
		}
		while(rightQRD.belowThreshold()) //Turn left slower until the left QRD hits tape
		{
			motor.speed(LEFT_DRIVE_MOTOR, 275);
			motor.speed(RIGHT_DRIVE_MOTOR, -275);
			rightQRD.read();
		}
		while(rightQRD.aboveThreshold()) //Inch left until the left QRD is just off the tape (and the robot is straight)
		{
			motor.speed(LEFT_DRIVE_MOTOR, 240);
			motor.speed(RIGHT_DRIVE_MOTOR, -240);
			rightQRD.read();
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
	
	void calibrate()
	{
		motor.speed(LEFT_DRIVE_MOTOR, baseSpeed);
		motor.speed(RIGHT_DRIVE_MOTOR, -baseSpeed);
		
		Vector<int> qrdL;
		Vector<int> qrdR;
		
		long start = millis();
		while((millis() - start) < 2000)
		{
			qrdL.addElement(leftQRD.reading);
			qrdR.addElement(rightQRD.reading);
			OBSERVER.update();
			delay(25);
		}
		
		int qrdLMax=0;
		int qrdRMax=0;
		int qrdLMin=0;
		int qrdRMin=0;
		
		for(int i=0; i<qrdL.size(); ++i)
		{
			if(qrdL.get(i) > qrdLMax)
				qrdLMax = qrdL.get(i);
			if(qrdL.get(i) < qrdLMin)
				qrdLMin = qrdL.get(i);
			
			if(qrdR.get(i) > qrdRMax)
				qrdRMax = qrdR.get(i);
			if(qrdR.get(i) < qrdRMin)
				qrdRMin = qrdR.get(i);
		}
		
		leftQRD.threshold = (qrdLMax + qrdLMin) / 2;
		rightQRD.threshold = (qrdRMax + qrdRMin) / 2;
		
		motor.stop_all();
	}
	
int getError()
	{
		if(leftQRD.aboveThreshold() && rightQRD.aboveThreshold()) error = 0;
		else if(leftQRD.belowThreshold() && rightQRD.aboveThreshold()) error = 1; //Left off tape, turn right
		else if(leftQRD.aboveThreshold() && rightQRD.belowThreshold()) error = -1; //Right off tape, turn left
		else if(leftQRD.belowThreshold() && rightQRD.belowThreshold()) //Both off tape -- use history or make a hard turn
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

  void followTape()
  {
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
  }

  void display()
  {
    LCD.clear();
    LCD.home();
    LCD.print("L:" + String(leftOutboardQRD.reading) + " ");
    LCD.print("R:" + String(rightOutboardQRD.reading));
    LCD.setCursor(0,1);
    LCD.print("L:" + String(leftQRD.reading) + " ");
    LCD.print("R:" + String(rightQRD.reading));
  }
  
  AnalogSignal leftQRD;
  AnalogSignal rightQRD;
  AnalogSignal leftOutboardQRD;
  AnalogSignal rightOutboardQRD;

  unsigned long time;
  unsigned long lastTime;
  char error;
  char previousError1;
  char previousError2;
  int previousCorrection;
  int correction;
  int maxError;
  int count; 
  
  char turnBias;
  
  int leftMotorSpeed;
  int rightMotorSpeed;
  
  int QRDThresholdL;
  int QRDThresholdR;
  int QRDThresholdOL;
  int QRDThresholdOR;
  int kP;
  int kD;
  int kI;
  int baseSpeed;
};

extern TapeFollower TAPEFOLLOWER;

#endif

