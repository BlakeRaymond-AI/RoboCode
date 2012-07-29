#ifndef SIGNAL_HEADER_GUARD
#define SIGNAL_HEADER_GUARD

#include <io_helper_functions.h>

class Signal
{
public:
	Signal(int pin)
	:pin(pin),
	enabled(false)
	{}
	
	virtual void read() =0;
	
	void enable()
	{
		enabled = true;
	}
	
	int pin;
	bool enabled;
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
		if(enabled)
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
		if(enabled)
				reading = digitalRead(pin);
	}
	
	bool on()
	{
		return !reading;
	}
	
	bool off()
	{
		return reading;
	}
	
	bool reading;
};

class Rangefinder : public Signal
{
		virtual void read()
		{
				lastReading = reading;
				reading = analogAverage(pin);
		}
		
		int lastReading;
};

#endif
