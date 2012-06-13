#include <phys253.h>
#include <LiquidCrystal.h>
#include <Servo253.h>

const int LEFTQRDPIN = 4;
const int RIGHTQRDPIN = 5;
const int LEFTMOTOR = 0;
const int RIGHTMOTOR = 1;
const int TAPE_THRESHOLD = 100;
const int BASELINE_SPEED = 250;

class TapeFollower
{
public:
        TapeFollower()
          :maxProportionalCoeff(500),
  		maxDerivativeCoeff(100),
  	  maxIntegralCoeff(10),
          derivativeCoeff(1.0),
          proportionalCoeff(1.0),
          integralCoeff(1.0),
          errorCurrent(0),
          errorPrev(0),
          errorCurrentDuration(0),
          errorPrevDuration(0),
          leftSensor(0),
          rightSensor(0)
        {}

	void adjustProportionalCoeff(int knobReturn)
	{
		proportionalCoeff = (knobReturn / 1023.0) * maxProportionalCoeff;
	}

	void adjustDerivativeCoeff(int knobReturn)
	{
		derivativeCoeff = (knobReturn / 1023.0) * maxDerivativeCoeff;
	}

	void adjustIntegralCoeff(int knobReturn)
	{
		integralCoeff = (knobReturn / 1023.0) * maxIntegralCoeff;
	}

        void driveLeftMotor(int speed)
        {
          motor.speed(LEFTMOTOR, BASELINE_SPEED + speed);
        }
        
        void driveRightMotor(int speed)
        {
          motor.speed(RIGHTMOTOR, BASELINE_SPEED + speed);
        }
        
        void updateSensors()
        {
          leftSensor = analogRead(LEFTQRDPIN);
          rightSensor = analogRead(RIGHTQRDPIN);
        }
        
        int computeSpeed()
        {
          int proportionalSpeed=0;
          int derivativeSpeed=0;
          int integralSpeed=0;
          
          if(onTape(leftSensor) && onTape(rightSensor))
            errorCurrent = 0;
          else if(onTape(leftSensor) && !onTape(rightSensor))
            errorCurrent = -1;
          else if(!onTape(leftSensor) && onTape(rightSensor))
            errorCurrent = 1;
          else //Both off tape
            //Refer to history
          {
            if(errorPrev > 0) //Right sensor was the last on the tape; needs to turn right
              errorCurrent = 3;
            else //Left sensor was the last on the tape; needs to turn left
              errorCurrent = -3;            
          }
          
          if(errorCurrent != errorPrev) //Derivative approximation
          {
             int changeInError = errorCurrent - errorPrev;
             
             errorPrevDuration = errorCurrentDuration;
             errorCurrentDuration =0;
             
             derivativeSpeed = derivativeCoeff*(changeInError)/(errorCurrentDuration + errorPrevDuration);
          }
          
          proportionalSpeed = proportionalCoeff*errorCurrent;
          ++errorCurrentDuration;
          
          return proportionalSpeed + derivativeSpeed + integralSpeed;
        }
        
        void followTape()
        {
          updateSensors();
          int speed = computeSpeed();
          LCD.print("L ");
          LCD.print(BASELINE_SPEED - speed);
          LCD.print("R ");
          LCD.print(BASELINE_SPEED + speed);
          driveLeftMotor(-1*speed);
          driveRightMotor(speed);
        }
        
        bool onTape(int sensorValue)
        {
          return sensorValue > TAPE_THRESHOLD;
        }
        
        int leftSensor;
        int rightSensor;
        
        int errorCurrent;
        int errorPrev;      
        int errorCurrentDuration; 
        int errorPrevDuration;
                
	float derivativeCoeff;
	float proportionalCoeff;
	float integralCoeff;

	const float maxProportionalCoeff;
	const float maxDerivativeCoeff;
	const float maxIntegralCoeff;
};
TapeFollower TAPEFOLLOWER;

const int NUM_MENU_ITEMS = 3;
enum MenuItems { PROPORTIONAL_COEFF, DERIVATIVE_COEFF, INTEGRAL_COEFF };
const int MENU_KNOB = 6;

class Menu
{
  public:
	void advance()
	{
		menuItem += 1;
                if(menuItem == 3) menuItem = 0;
	}

	void adjust()
	{
		switch(menuItem)
		{
		case PROPORTIONAL_COEFF:
			TAPEFOLLOWER.adjustProportionalCoeff(knob(MENU_KNOB));
			break;
		case DERIVATIVE_COEFF:
			TAPEFOLLOWER.adjustDerivativeCoeff(knob(MENU_KNOB));
			break;
		case INTEGRAL_COEFF:
			TAPEFOLLOWER.adjustIntegralCoeff(knob(MENU_KNOB));
			break;
		default:
			break;
		}
	}

	void print()
	{
		LCD.clear();
		LCD.home();
		switch(menuItem)
		{
		case PROPORTIONAL_COEFF:
			LCD.print("P ");
			LCD.print(TAPEFOLLOWER.proportionalCoeff);
			break;
		case DERIVATIVE_COEFF:
			LCD.print("D ");
			LCD.print(TAPEFOLLOWER.derivativeCoeff);
			break;
		case INTEGRAL_COEFF:
			LCD.print("I ");
			LCD.print(TAPEFOLLOWER.integralCoeff);
			break;
		default:
			break;
		}
	}

        void init()
        {
          menuItem=0;
        }
	
	int menuItem;
};
Menu MENU;

void setup()
{
	//attachInterrupt(INT0, openMenuInterruptHandler, FALLING);
        MENU.init();
}

void loop()
{
  while(!startbutton())
  {
    LCD.clear();
    LCD.home();
    TAPEFOLLOWER.followTape();
    TAPEFOLLOWER.adjustProportionalCoeff(knob(6));
    TAPEFOLLOWER.adjustDerivativeCoeff(knob(7));
    LCD.setCursor(0,1);
    LCD.print("P ");
    LCD.print(TAPEFOLLOWER.proportionalCoeff);
    LCD.print(" D ");
    LCD.print(TAPEFOLLOWER.derivativeCoeff);
    delay(10);
  }
  
  /*
  delay(200);
  while(!stopbutton())
  {
	while(!startbutton()) //Wait for ADVANCE
	{
                MENU.print();
		MENU.adjust(); //Read the knob value
		delay(100);
                if(stopbutton()) break;
	} 
        delay(250);
	MENU.advance(); //Advance the menu selection
  }
  */
  
}
