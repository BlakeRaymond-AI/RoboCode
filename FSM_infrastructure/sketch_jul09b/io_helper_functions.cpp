#include "WProgram.h"
#include <io_helper_functions.h>

int analogAverage(int pin)
{
  int total=0;
  int count=0;

  while(count < 3)
  {
    total += analogRead(pin);
    ++count;
  }

  return total/count;
}

bool readStart()
{
  int on=0;
  int off=0;
  while(on+off<5)
  {
    if(startbutton())
      ++on;
    else
      ++off;
  }
  return on > off;
}

bool readStop()
{
  int on=0;
  int off=0;
  while(on+off<5)
  {
    if(stopbutton())
      ++on;
    else
      ++off;
  }
  return on > off;
}
