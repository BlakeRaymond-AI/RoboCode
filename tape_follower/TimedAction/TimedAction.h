#ifndef TIMEDACTION_H
#define TIMEDACTION_H

class TimedAction
{
  public:
    TimedAction(unsigned long interval, void (*function)());
    TimedAction(unsigned long prev, unsigned long interval, void (*function)());

    void reset();
    void disable();
    void enable();
    void check();

    void setInterval(unsigned long interval);

  private:
    bool active;
    unsigned long previous;
    unsigned long interval;

    void (*execute)();

};

#endif
// TIMEDACTION_H