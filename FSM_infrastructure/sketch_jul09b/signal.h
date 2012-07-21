#ifndef SIGNAL_HEADER_GUARD
#define SIGNAL_HEADER_GUARD

#include <io_helper_functions.h>

struct Signal
{
	Signal(int pin, int threshold)
	:threshold(threshold),
	pin(pin)
	{}
	
	void read();
	
	bool aboveThreshold();
	
	bool belowThreshold();
	
	void setThreshold(int newThreshold);
	
	int reading;
	int threshold;
	int pin;
};

#endif
