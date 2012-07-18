#include "WProgram.h"
#include <signal.h>

	void Signal::read()
	{
		reading = analogRead(pin);
	}
	
	bool Signal::aboveThreshold()
	{
		return reading >= threshold;
	}
	
	bool Signal::belowThreshold()
	{
		return reading < threshold;
	}
	
	void Signal::setThreshold(int newThreshold)
	{
		threshold = newThreshold;
	}	
	
	void TimedSignal::read()
	{
		int time = millis();
		if((time - lastRefreshTime) > refreshRate)
		{
			lastRefreshTime = time;
			reading = analogRead(pin);
		}
	}
