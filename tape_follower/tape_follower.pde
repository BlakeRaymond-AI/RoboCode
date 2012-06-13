#include <phys253.h>       //   ***** from 253 template file
#include <LiquidCrystal.h> //   ***** from 253 template file
#include <Servo253.h>      //   ***** from 253 template file 
#include <TimedAction.h>

enum MOTORS { LEFT_DRIVE_MOTOR = 1, RIGHT_DRIVE_MOTOR = 2 };
enum ANALOG_INPUTS { LEFT_TAPE_QRD = 5, RIGHT_TAPE_QRD = 4 };

class Menu
{
  public:
  Menu()
  {
    names.addElement("Prop: ");
    names.addElement("Deriv: ");
    names.addElement("Speed: ");
    names.addElement("QRD: ");
   
    values.addElement(0);
    values.addElement(0);
    values.addElement(0);
    values.addElement(0);
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
    index = knob(6) / 1023.0 * names.size(); 
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
    values.setElementAt(index, value);
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
    : kP(50),
      kD(10),
      speed(250),
      time(0),
      lastTime(0),
      error(0),
      lastError(0),
      qrdThreshold(100),
      leftMotorSpeed(0),
      rightMotorSpeed(0),
      displayAction(100, NULL)
    {
      
    }
    
    void followTape()
    {
      int leftQRD = analogRead(LEFT_TAPE_QRD);
      int rightQRD = analogRead(RIGHT_TAPE_QRD);
     
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
      
      leftMotorSpeed = speed-correction;
      rightMotorSpeed = speed+correction;
  
      motor.speed(LEFT_DRIVE_MOTOR, leftMotorSpeed);
      motor.speed(RIGHT_DRIVE_MOTOR, rightMotorSpeed);
      lastError = error;
      ++time;
      
      displayAction.check();
    }
    
    void display()
    {
      LCD.clear();
      LCD.home();
      LCD.print("Left: " + String(leftMotorSpeed));
      LCD.setCursor(0,1);
      LCD.print("Right: " + String(rightMotorSpeed));
    }
    
    int kP;
    int kD;
    int speed;
    int qrdThreshold;
    
    TimedAction displayAction;
    
  private:
    int time;
    int lastTime;
    int error;
    int lastError;
    int leftMotorSpeed;
    int rightMotorSpeed;
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
    MENU.open();
    
    TAPEFOLLOWER.kP = remapKnob(MENU.values.elementAt(0), 0, 300);
    TAPEFOLLOWER.kD = remapKnob(MENU.values.elementAt(1), 0, 300);
    TAPEFOLLOWER.speed = MENU.values.elementAt(2);
    TAPEFOLLOWER.qrdThreshold = MENU.values.elementAt(3);
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

