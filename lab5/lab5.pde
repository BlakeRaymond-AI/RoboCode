class TapeFollower
{
public:
	static void adjustProportionalCoeff(int knobReturn)
	{
		proportionalCoeff = (knobReturn / 1023) * maxProportionalCoeff;
	}
	static void adjustDerivativeCoeff(int knobReturn)
	{
		derivativeCoeff = (knobReturn / 1023) * maxDerivativeCoeff;
	}
	static void adjustIntegralCoeff(int knobReturn)
	{
		integralCoeff = (knobReturn / 1023) * maxIntegralCoeff;
	}

	static float derivativeCoeff;
	static float proportionalCoeff;
	static float integralCoeff;

	static const float maxProportionalCoeff;
	static const float maxDerivativeCoeff;
	static const float maxIntegralCoeff;
};
TapeFollower::maxProportionalCoeff = 500;
TapeFollower::maxDerivativeCoeff = 100;
TapeFollower::maxIntegralCoeff = 10;

const int NUM_MENU_ITEMS = 3;
enum MenuItems = { PROPORTIONAL_COEFF, DERIVATIVE_COEFF, INTEGRAL_COEFF };
const int MENU_KNOB = 6;
const int MENU_ADVANCE_PIN = 0; //Don't forget to change the interrupt attachment if you change this
const int MENU_EXIT_PIN = 1;

class Menu
{
	static void advance()
	{
		menuItem += 1;
		if(menuItem > 2) menuItem %= NUM_MENU_ITEMS;
	}

	static void adjust()
	{
		switch(menuItem)
		{
		case PROPORTIONAL_COEFF:
			TapeFollower::adjustProportionalCoeff(knob(MENU_KNOB));
			break;
		case DERIVATIVE_COEFF:
			TapeFollower::adjustDerivativeCoeff(knob(MENU_KNOB));
			break;
		case INTEGRAL_COEFF:
			TapeFollower::adjustIntegralCoeff(knob(MENU_KNOB));
			break;
		default:
			break;
		}
	}

	static void print()
	{
		LCD.clear();
		LCD.home();
		switch(menuItem)
		{
		case PROPORTIONAL_COEFF:
			LCD.println("PROPORTIONAL");
			LCD.print(TapeFollower::derivativeCoeff);
			break;
		case DERIVATIVE_COEFF:
			LCD.println("DERIVATIVE");
			LCD.print(TapeFollower::proportionalCoeff);
			break;
		case INTEGRAL_COEFF:
			LCD.println("INTEGRAL");
			LCD.print(TapeFollower::integralCoeff);
			break;
		default:
			break;
		}
	}
	
	static int menuItem;
};
Menu::menuItem=0;

void openMenuInterruptHandler()
{
	while(!digitalRead(MENU_EXIT_PIN)) //Remain in the menu until EXIT
	{
		Menu::print();
		while(!digitalRead(MENU_ADVANCE_PIN)) //Wait for ADVANCE
		{
			Menu::adjust(); //Read the knob value
			delay(100);
		} 
		Menu::advance(); //Advance the menu selection
	}
	LCD.clear();
	LCD.home();
}

void setup()
{
	attachInterrupt(EXTERNAL_INTERRUPT_0, openMenuInterruptHandler, FALLING);
}

void loop()
{

}
