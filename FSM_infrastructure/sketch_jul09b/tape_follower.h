#ifndef TAPE_FOLLOWER
#define TAPE_FOLLOWER

#include <io_helper_functions.h>

enum INITIAL_TAPEFOLLOWING_CONSTANTS {
  initialSpeed = 400,
  initialQRDThresholdL = 200,
  initialQRDThresholdR = 200,
  initialProportionalGain = 480,
  initialDerivGain = 230,
  intialLRBalance = 512
};

class TapeFollower
{
public:
  TapeFollower()
  : 
    kP(initialProportionalGain),
    kD(initialDerivGain),
    speed(initialSpeed),
    time(0),
    lastTime(0),
    error(0),
    lastError(0),
    qrdThresholdL(initialQRDThresholdL),
    qrdThresholdR(initialQRDThresholdR),
    leftMotorSpeed(0),
    rightMotorSpeed(0),
    LRBalance(intialLRBalance),
    count(0)
    {
	
    }

  void followTape()
  {
    leftQRD = analogAverage(LEFT_TAPE_QRD);
    rightQRD = analogAverage(RIGHT_TAPE_QRD);

    //Proportional control
    int error = 0;    
    if(leftQRD>qrdThresholdL && rightQRD>qrdThresholdR) error = 0;
    if(leftQRD>qrdThresholdL && rightQRD<qrdThresholdR) error = -1;
    if(leftQRD<qrdThresholdL && rightQRD>qrdThresholdR) error = 1;
    if(leftQRD<qrdThresholdL && rightQRD<qrdThresholdR)
    {
      if(lastError>0) error = 3;
      else error = -3;
    }
    int proportional=kP*error;

    //Derivative control estimation
    if(error != lastError)
    {
      lastTime=time;
      time=0;
    }      
    int derivative=kD*(error-lastError)/((float)(time+lastTime));

    int correction = proportional + derivative;

    leftMotorSpeed = speed+correction;
    rightMotorSpeed = speed-correction;

    motor.speed(LEFT_DRIVE_MOTOR, leftMotorSpeed * (LRBalance/512.0));
    motor.speed(RIGHT_DRIVE_MOTOR, rightMotorSpeed);
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

  int kP;
  int kD;
  int speed;
  int qrdThresholdL;
  int qrdThresholdR;
  int LRBalance;

private:
  int time;
  int lastTime;
  int error;
  int lastError;
  int leftMotorSpeed;
  int rightMotorSpeed;
  int leftQRD;
  int rightQRD;
  int count;
};

TapeFollower TAPEFOLLOWER;

void tapeFollowerDisplay()
{
  TAPEFOLLOWER.display();
}

#endif

