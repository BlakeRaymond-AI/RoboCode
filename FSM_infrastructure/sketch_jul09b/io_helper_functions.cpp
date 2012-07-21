#include "WProgram.h"
#include <io_helper_functions.h>

int analogAverage(int pin)
{
  int total=0;
  int count=0;

  while(count < 5)
  {
    total += analogRead(pin);
    ++count;
  }

  return total/count;
}

boolean readStart()
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

boolean readStop()
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
