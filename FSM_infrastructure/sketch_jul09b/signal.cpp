#include "WProgram.h"
#include <signal.h>

	void AnalogSignal::read()
	{
		reading = analogRead(pin);
	}
	
	bool AnalogSignal::aboveThreshold()
	{
		return reading >= threshold;
	}
	
	bool AnalogSignal::belowThreshold()
	{
		return reading < threshold;
	}
	
	void AnalogSignal::setThreshold(int newThreshold)
	{
		threshold = newThreshold;
	}	
