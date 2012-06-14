#include <phys253.h>       //   ***** from 253 template file
#include <LiquidCrystal.h> //   ***** from 253 template file
#include <Servo253.h>      //   ***** from 253 template file 
#include <TimedAction.h>

enum MOTORS { LEFT_DRIVE_MOTOR = 0, RIGHT_DRIVE_MOTOR = 1 };
enum ANALOG_INPUTS { LEFT_TAPE_QRD = 5, RIGHT_TAPE_QRD = 4 };

const int initialSpeed = 500;
const int initialQRDThreshold = 100;
const int initialProportionalGain = 150;
const int initialDerivGain = 100;
const int intialLRBalance = 512;

class Menu
{
  public:
  Menu()
  {
    names.addElement("Prop: ");
    names.addElement("Deriv: ");
    names.addElement("Speed: ");
    names.addElement("QRD: ");
    names.addElement("L/R Balance: ");   
    
    values.addElement(initialProportionalGain);
    values.addElement(initialDerivGain);
    values.addElement(initialSpeed);
    values.addElement(initialQRDThreshold);
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
      qrdThreshold(initialQRDThreshold),
      leftMotorSpeed(0),
      rightMotorSpeed(0),
      displayAction(100, NULL),
      LRBalance(intialLRBalance)
    {
    
    }
    
    void followTape()
    {
      leftQRD = analogRead(LEFT_TAPE_QRD);
      rightQRD = analogRead(RIGHT_TAPE_QRD);
     
      //Proportional control
      int error = 0;    
      if(leftQRD>qrdThreshold && rightQRD>qrdThreshold) error = 0;
      if(leftQRD>qrdThreshold && rightQRD<qrdThreshold) error = -1;
      if(leftQRD<qrdThreshold && rightQRD>qrdThreshold) error = 1;
      if(leftQRD<qrdThreshold && rightQRD<qrdThreshold)
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
      
      displayAction.check();
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
    int qrdThreshold;
    int LRBalance;
    
    TimedAction displayAction;
    
  private:
    int time;
    int lastTime;
    int error;
    int lastError;
    int leftMotorSpeed;
    int rightMotorSpeed;
    int leftQRD;
    int rightQRD;
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
    TAPEFOLLOWER.qrdThreshold = MENU.values.elementAt(3);
    TAPEFOLLOWER.LRBalance = MENU.values.elementAt(4);
  }
}
TimedAction refreshAction = TimedAction(100, refresh);

void setup()
{
  //Display the motor speeds every 100 milliseconds
  TAPEFOLLOWER.displayAction = TimedAction(100, tapeFollowerDisplay);
}

void loop()
{
  refreshAction.check();
  TAPEFOLLOWER.followTape();
}

