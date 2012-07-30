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

class Rangefinder : public AnalogSignal
{
		public:
		Rangefinder(int pin, int threshold)
		: AnalogSignal(pin, threshold)
		{
		
		}
		
		virtual void read()
		{
				if(enabled)
				{
						lastReading = reading;
						reading = analogAverage(pin);
				}				
		}
		
		bool edgeDetected(int &edgeThreshold)
		{
				return abs(reading - lastReading) > edgeThreshold;
		}
		
		bool edgeOff(int &edgeThreshold)
		{
				return (lastReading - reading) > edgeThreshold;
		}
		
		bool edgeOn(int &edgeThreshold)
		{
				return (reading - lastReading) > edgeThreshold;
		}
		
		bool belowThreshold(int &threshold)
		{
				return reading < threshold;
		}
		
		bool aboveThreshold(int &threshold)
		{
				return reading >= threshold;
		}
		
		int lastReading;
};

#endif
