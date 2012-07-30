#ifndef RANGEFINDERS_HEADER_GUARD
#define RANGEFINDERS_HEADER_GUARD

#include <signal.h>
#include <observer.h>
#include <pins.h>
#include <drive_system.h>

class Rangefinders
{
    public:
    Rangefinders()
    : blockTracked(false)
    {}
    
    bool isGap()
    {
        return OBSERVER.leftRangefinder.belowThreshold(gapThreshold) && OBSERVER.rightRangefinder.belowThreshold(gapThreshold);
    }
    
    void enable()
    {
        OBSERVER.leftRangefinder.enable();
        OBSERVER.rightRangefinder.enable();
    }
    
    void reset()
    {
        blockTracked = false;
    }
    
    void disable()
    {
        OBSERVER.leftRangefinder.disable();
        OBSERVER.rightRangefinder.disable();
        reset();
    }
    
    bool panRightUntilGap()
    {
        DRIVE_SYSTEM.stop();
        
        long startTime = millis();
        
        while(!isGap())
        {
            DRIVE_SYSTEM.turnRight(SLOW_MOTOR_SPEED);
            OBSERVER.update();
            
            if((millis() - startTime) > maxPanTime)
            {
                DRIVE_SYSTEM.stop();
                return false;
            }
        }
        
        DRIVE_SYSTEM.stop();
        return true;
    }
    
    bool panLeftUntilGap()
    {
        DRIVE_SYSTEM.stop();
        
        long startTime = millis();
        
        while(!isGap())
        {
            DRIVE_SYSTEM.turnLeft(SLOW_MOTOR_SPEED);
            OBSERVER.update();
            
            if((millis() - startTime) > maxPanTime)
            {
                DRIVE_SYSTEM.stop();
                return false;
            }
        }
        
        DRIVE_SYSTEM.stop();
        return true;
    }
    
    bool panRightUntilEitherHigh()
    {
        DRIVE_SYSTEM.stop();
        long startTime = millis();
        while(OBSERVER.leftRangefinder.belowThreshold(gapThreshold) && OBSERVER.rightRangefinder.belowThreshold(gapThreshold))
        {
            DRIVE_SYSTEM.turnRight(SLOW_MOTOR_SPEED);
            OBSERVER.update();
            if((millis() - startTime) > maxPanTime)
            {
                DRIVE_SYSTEM.stop();
                return false;
            }
        }
        DRIVE_SYSTEM.stop();
        return true;
    }
    
    bool panLeftUntilEitherHigh()
    {
        DRIVE_SYSTEM.stop();
        long startTime = millis();
        while(OBSERVER.leftRangefinder.belowThreshold(gapThreshold) && OBSERVER.rightRangefinder.belowThreshold(gapThreshold))
        {
            DRIVE_SYSTEM.turnLeft(SLOW_MOTOR_SPEED);
            OBSERVER.update();
            if((millis() - startTime) > maxPanTime)
            {
                DRIVE_SYSTEM.stop();
                return false;
            }
        }
        DRIVE_SYSTEM.stop();
        return true;    
    }
    
    void moveToBlockInDepot()
    {
        while(OBSERVER.leftBumper.off() && OBSERVER.rightBumper.off() && OBSERVER.gripperCentreSwitch.off())
        {
            DRIVE_SYSTEM.drive(MEDIUM_MOTOR_SPEED, MEDIUM_MOTOR_SPEED);
            
            if(OBSERVER.leftRangefinder.edgeOn(edgeThreshold))
            {
                panRightUntilGap();
            }
            else if(OBSERVER.rightRangefinder.edgeOn(edgeThreshold))
            {
                panLeftUntilGap();
            }
            
            OBSERVER.update();
        }
    }
    
    void moveToBlockInBuildArea() //remember to handle the centre bumper externally
    {
        if(OBSERVER.leftRangefinder.belowThreshold(gapThreshold) && OBSERVER.rightRangefinder.belowThreshold(gapThreshold)) //doesn't see anything
        {
            if(blockTracked == true)
            {
                //move forward
                DRIVE_SYSTEM.drive(SLOW_MOTOR_SPEED, SLOW_MOTOR_SPEED);
            }
            else
            {
                //search for the block
                if(panRightUntilEitherHigh() == false && panLeftUntilEitherHigh() == false && panLeftUntilEitherHigh() == false && panRightUntilEitherHigh() == false)
                {
                    //couldn't find it; lower the threshold and try again
                    LCD.clear();
                    LCD.home();
                    LCD.print("ERROR");
                    LCD.setCursor(1,0);
                    LCD.print("BLOCK NOT FOUND");
                    delay(1000);
                    gapThreshold -= 25;                    
                }
                else
                {
                    //found it
                    DRIVE_SYSTEM.stop();
                }
            }
        }
        
        else if(OBSERVER.leftRangefinder.aboveThreshold(gapThreshold) && OBSERVER.rightRangefinder.belowThreshold(gapThreshold))
        {
            //turn left until gap
            
            panLeftUntilGap();
            
            blockTracked = true;
        }
        
        else if(OBSERVER.leftRangefinder.belowThreshold(gapThreshold) && OBSERVER.rightRangefinder.aboveThreshold(gapThreshold))
        {
            //turn right until gap
            
            panRightUntilGap();
            
            blockTracked = true;
        }
        
        else if(OBSERVER.leftRangefinder.aboveThreshold(gapThreshold) && OBSERVER.rightRangefinder.aboveThreshold(gapThreshold))
        {
            LCD.clear();
            LCD.home();
            LCD.print("ERROR");
            LCD.setCursor(1,0);
            LCD.print("BOTH SENSORS HIGH");
            delay(3000);
        }
        else
        {
            LCD.clear();
            LCD.home();
            LCD.print("ERROR");
            LCD.setCursor(1,0);
            LCD.print("BAD LOGIC");
            delay(3000);
        }
    }
    
    int gapThreshold;
    int edgeThreshold;
    int maxPanTime;
    bool blockTracked;
};

extern Rangefinders RANGEFINDERS;

#endif
