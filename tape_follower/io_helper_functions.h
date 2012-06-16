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

int remap(int origValue, int origLow, int origHigh, int newLow, int newHigh)
{
  return (int)((float)(origValue - origLow) / (origHigh - origLow) * (newHigh - newLow) + newLow);
}

int remapKnob(int origValue, int newLow, int newHigh)
{
  return remap(origValue, 0, 1023, newLow, newHigh);
}

#endif