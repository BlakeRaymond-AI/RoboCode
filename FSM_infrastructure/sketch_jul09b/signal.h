#ifndef SIGNAL_HEADER_GUARD
#define SIGNAL_HEADER_GUARD

#include <io_helper_functions.h>

class Signal
{
public:
	Signal(int pin)
	:pin(pin)
	{}
	
	virtual void read() =0;
	
	int pin;
};

class AnalogSignal : public Signal
{
	public:
	AnalogSignal(int pin, int threshold)
	:Signal(pin),
	threshold(threshold)
	{}
	
	virtual void read()
	{
		reading = analogRead(pin);
	}
	
	bool aboveThreshold()
	{
		return reading >= threshold;
	}
	
	bool belowThreshold()
	{
		return reading < threshold;
	}
	
	int reading;
	int threshold;
};

class DigitalSignal : public Signal
{
public:
	DigitalSignal(int pin)
	:Signal(pin)
	{}
	
	virtual void read()
	{
		reading = digitalRead(pin);
	}
	
	boolean on()
	{
		return reading;
	}
	
	boolean off()
	{
		return !reading;
	}
	
	boolean reading;
};

#endif
