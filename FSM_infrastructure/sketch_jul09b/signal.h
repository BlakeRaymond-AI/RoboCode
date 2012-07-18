#ifndef SIGNAL_HEADER_GUARD
#define SIGNAL_HEADER_GUARD

#include <io_helper_functions.h>

class Signal
{
public:
	Signal(int threshold, int pin)
	:threshold(threshold),
	pin(pin)
	{}
	
	virtual void read();
	
	bool aboveThreshold();
	
	bool belowThreshold();
	
	void setThreshold(int newThreshold);
	
	int reading;
	int threshold;
	int pin;
};

class TimedSignal : public Signal
{
public:
	TimedSignal(int threshold, int pin, int refreshRate)
	: refreshRate(refreshRate), Signal(threshold, pin)
	{}
	
	virtual void read();
	
	void setRefreshRate(int newRefreshRate)
	{
		refreshRate = newRefreshRate;
	}
	
	int refreshRate;
	int lastRefreshTime;
};

#endif
