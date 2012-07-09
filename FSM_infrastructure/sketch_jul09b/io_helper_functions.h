#ifndef IO_HELPER_FUNCTIONS
#define IO_HELPER_FUNCTIONS

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
  while(on+off<20)
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
  while(on+off<20)
  {
    if(stopbutton())
      ++on;
    else
      ++off;
  }
  return on > off;
}

#endif
