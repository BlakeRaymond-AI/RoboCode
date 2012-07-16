#ifndef SIGNAL_HEADER_GUARD
#define SIGNAL_HEADER_GUARD

struct Signal
{
	Signal(int threshold, int pin)
	:threshold(threshold),
	pin(pin)
	{}
	
	void read()
	{
		reading = analogAverage(pin);
	}
	
	bool aboveThreshold()
	{
		return reading >= threshold;
	}
	
	bool belowThreshold()
	{
		return reading < threshold;
	}
	
	void setThreshold(int newThreshold)
	{
		threshold = newThreshold;
	}	
	
	int reading;
	int threshold;
	int pin;
};

#endif