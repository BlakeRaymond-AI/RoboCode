#include <phys253.h>       //   ***** from 253 template file
#include <LiquidCrystal.h> //   ***** from 253 template file
#include <Servo253.h>      //   ***** from 253 template file 

enum MOTORS { LEFT_DRIVE_MOTOR = 0, RIGHT_DRIVE_MOTOR = 1 };
enum ANALOG_INPUTS { LEFT_TAPE_QRD = 5, RIGHT_TAPE_QRD = 4 };

const int initialSpeed = 400;
const int initialQRDThresholdL = 200;
const int initialQRDThresholdR = 200;
const int initialProportionalGain = 480;
const int initialDerivGain = 230;
const int intialLRBalance = 512;

int analogAverage(int pin)
{
  int total=0;
  int count=0;
  
  while(count < 5)
  {
    total += analogRead(pin);
    ++count;
  }
  
  return total/count;
}

class Menu
{
  public:
  Menu()
  {
    names.addElement("Prop: ");
    names.addElement("Deriv: ");
    names.addElement("Speed: ");
    names.addElement("QRDL: ");
    names.addElement("QRDR: ");
    names.addElement("L/R Balance: ");   
    
    values.addElement(initialProportionalGain);
    values.addElement(initialDerivGain);
    values.addElement(initialSpeed);
    values.addElement(initialQRDThresholdL);
    values.addElement(initialQRDThresholdR);
    values.addElement(intialLRBalance);  
}
  
  void open()
  {
    motor.stop(LEFT_DRIVE_MOTOR);
    motor.stop(RIGHT_DRIVE_MOTOR);
    
    //Press stop to exit the menu 
    while(!readStop())
    {
      getItem();
      display();
      delay(50);
      //Press start to set the value
      if(readStart())
      {
        set(knob(5));
        LCD.setCursor(0,1);
        LCD.print("Set");
        delay(1000);
      }
    }
    
    LCD.clear();    
  }
  
  //Scroll through the menu using one of the knobs  
  void getItem()
  {
    index = knob(6) / 1023.0 * (names.size()); 
  }
  
  void display()
  {
    LCD.clear();
    LCD.home();
    LCD.print(names.elementAt(index));
    LCD.print(knob(5));
  }  
  
  void set(int value)
  {
    values.setElementAt(value, index);
  }

  Vector<char*> names;  
  Vector<int> values;
  int index;
};

Menu MENU;

class TapeFollower
{
  public:
    TapeFollower()
    : kP(initialProportionalGain),
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

boolean readStart()
{
  int on=0;
  int off=0;
  while(on+off<20)
  {
    if(startbutton())
      ++on;
    else
      ++off;
  }
  return on > off;
}

boolean readStop()
{
  int on=0;
  int off=0;
  while(on+off<20)
  {
    if(stopbutton())
      ++on;
    else
      ++off;
  }
  return on > off;
}

int remap(int origValue, int origLow, int origHigh, int newLow, int newHigh)
{
  return (int)((float)(origValue - origLow) / (origHigh - origLow) * (newHigh - newLow) + newLow);
}

int remapKnob(int origValue, int newLow, int newHigh)
{
  return remap(origValue, 0, 1023, newLow, newHigh);
}

void refresh()
{
  if(readStart())
  {
    while(readStart()); //Wait for the start button to stop being pressed
    MENU.open();
    
    TAPEFOLLOWER.kP = MENU.values.elementAt(0) / 1023.0 * 300;
    TAPEFOLLOWER.kD = MENU.values.elementAt(1) / 1023.0 * 300;
    TAPEFOLLOWER.speed = MENU.values.elementAt(2);
    TAPEFOLLOWER.qrdThresholdL = MENU.values.elementAt(3);
    TAPEFOLLOWER.qrdThresholdR = MENU.values.elementAt(4);
    TAPEFOLLOWER.LRBalance = MENU.values.elementAt(5);
  }
}

void setup()
{

}

void loop()
{
  int i=0;
  
  while(1)
  {
    ++i;
    TAPEFOLLOWER.followTape();
    
    if(i == 100)
    {
      refresh();
      tapeFollowerDisplay();
      i=0;
    }
  }
}

