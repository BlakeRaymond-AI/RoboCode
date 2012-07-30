#ifndef RANGEFINDERS_HEADER_GUARD
#define RANGEFINDERS_HEADER_GUARD

class Rangefinders
{
    public:
    Rangefinders() {}
    
    bool isGap()
    {
        return OBSERVER.leftRangefinder.belowThreshold() && OBSERVER.rightRangefinder.belowThreshold();
    }
    
    void enable()
    {
        OBSERVER.leftRangefinder.enable();
        OBSERVER.rightRangefinder.enable();
    }
    
    void disable()
    {
        OBSERVER.leftRangefinder.disable();
        OBSERVER.rightRangefinder.disable();
    }
    
    void panRightUntilGap()
    {
        DRIVE_SYSTEM.stop();
        
        while(OBSERVER.leftRangefinder.aboveThreshold() || OBSERVER.rightRangefinder.aboveThreshold())
        {
            DRIVE_SYSTEM.turnRight(SLOW_MOTOR_SPEED);
            OBSERVER.update();
        }
        
        DRIVE_SYSTEM.stop();
    }
    
    void panLeftUntilGap()
    {
        DRIVE_SYSTEM.stop();
        
        while(OBSERVER.leftRangefinder.aboveThreshold() || OBSERVER.rightRangefinder.aboveThreshold())
        {
            DRIVE_SYSTEM.turnLeft(SLOW_MOTOR_SPEED);
            OBSERVER.update();
        }
        
        DRIVE_SYSTEM.stop();
    }
    
    void moveToBlockInDepot()
    {
        while(OBSERVER.leftBumper.off() && OBSERVER.rightBumper.off() && OBSERVER.gripperCentreSwitch.off())
        {
            DRIVE_SYSTEM.drive(MEDIUM_MOTOR_SPEED);
            
            if(OBSERVER.leftRangefinder.aboveThreshold())
            {
                panRightUntilGap();
            }
            else if(OBSERVER.rightRangefinder.aboveThreshold())
            {
                panLeftUntilGap();
            }
            
            OBSERVER.update();
        }
    }
};

extern Rangefinders RANGEFINDERS;

#endif