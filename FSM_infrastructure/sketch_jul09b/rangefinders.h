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
    : cycleCount(0),
    blockThreshold(100),
    edgeThreshold(50),
    maxSearchTime(2000),
    maxEdgeFindingTime(500),
    kPBuildArea(300),
    kPDepot(300),
    error(0),
    lastError(0)
    {}
    
    void display()
    {
        LCD.clear();
        LCD.home();
        LCD.print("L: " + String(OBSERVER.leftRangefinder.reading));
        LCD.print(" R: " + String(OBSERVER.rightRangefinder.reading));
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
    
    bool panLeftUntilEitherHigh()
    {
        DRIVE_SYSTEM.stop();
        long startTime = millis();
        while(OBSERVER.leftRangefinder.belowThreshold(blockThreshold) && OBSERVER.rightRangefinder.belowThreshold(blockThreshold))
        {
            DRIVE_SYSTEM.turnLeft(SLOW_MOTOR_SPEED);
            OBSERVER.update();
            if((millis() - startTime) > maxSearchTime)
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
        while(OBSERVER.leftRangefinder.belowThreshold(blockThreshold) && OBSERVER.rightRangefinder.belowThreshold(blockThreshold))
        {
            DRIVE_SYSTEM.turnRight(SLOW_MOTOR_SPEED);
            OBSERVER.update();
            if((millis() - startTime) > maxSearchTime)
            {
                DRIVE_SYSTEM.stop();
                return false;
            }
        }
        DRIVE_SYSTEM.stop();
        return true;
    }
    
    bool panLeftUntilBothHigh()
    {
        DRIVE_SYSTEM.stop();
        long startTime = millis();
        while(OBSERVER.leftRangefinder.belowThreshold(blockThreshold) || OBSERVER.rightRangefinder.belowThreshold(blockThreshold))
        {
            DRIVE_SYSTEM.turnLeft(SLOW_MOTOR_SPEED);
            OBSERVER.update();
            if((millis() - startTime) > maxSearchTime)
            {
                DRIVE_SYSTEM.stop();
                return false;
            }
        }
        DRIVE_SYSTEM.stop();
        return true;
    }
    
    bool panRightUntilBothHigh()
    {
        DRIVE_SYSTEM.stop();
        long startTime = millis();
        while(OBSERVER.leftRangefinder.belowThreshold(blockThreshold) || OBSERVER.rightRangefinder.belowThreshold(blockThreshold))
        {
            DRIVE_SYSTEM.turnRight(SLOW_MOTOR_SPEED);
            OBSERVER.update();
            if((millis() - startTime) > maxSearchTime)
            {
                DRIVE_SYSTEM.stop();
                return false;
            }
        }
        DRIVE_SYSTEM.stop();
        return true;
    }
 
    bool panRightUntilRightHigh()
    {
        DRIVE_SYSTEM.stop();
        long startTime = millis();
        
        while(OBSERVER.rightRangefinder.belowThreshold(blockThreshold))
        {
            DRIVE_SYSTEM.turnRight(SLOW_MOTOR_SPEED);
            OBSERVER.update();
            if((millis() - startTime) > maxSearchTime)
            {
                DRIVE_SYSTEM.stop();
                return false;
            }
        }
        DRIVE_SYSTEM.stop();
        return true;   
    }
    
    bool panLeftUntilLeftHigh()
    {
        DRIVE_SYSTEM.stop();
        long startTime = millis();
        
        while(OBSERVER.leftRangefinder.belowThreshold(blockThreshold))
        {
            DRIVE_SYSTEM.turnLeft(SLOW_MOTOR_SPEED);
            OBSERVER.update();
            if((millis() - startTime) > maxSearchTime)
            {
                DRIVE_SYSTEM.stop();
                return false;
            }
        }
        DRIVE_SYSTEM.stop();
        return true;       
    }
    
    bool findLeftEdge()
    {
        int startTime = millis();
        
        while(!OBSERVER.leftRangefinder.edgeOff(edgeThreshold))
        {
            if((millis() - startTime) > maxEdgeFindingTime)
                return false;
        }
        
        return true;
    }

    bool findRightEdge()
    {
        int startTime = millis();
        
        while(!OBSERVER.rightRangefinder.edgeOff(edgeThreshold))
        {
            if((millis() - startTime) > maxEdgeFindingTime)
                return false;
        }
        
        return true;
    }
    
    bool searchForBlockInDepot()
    {
        //search for a block
        //find its edges
        //if successful, go get it
        //otherwise keep searching
        if(panRightUntilBothHigh())
        {
            if(findLeftEdge() && findRightEdge())
                return true;
        }
        if(panLeftUntilBothHigh() || panLeftUntilBothHigh())
        {
            if(findLeftEdge() && findRightEdge())
                return true;
        }
        if(panRightUntilBothHigh())
        {
            if(findLeftEdge() && findRightEdge())
                return true;
        }
        
        return false;
    }
  
    bool searchForBlockInBuildArea()
    {
        if(OBSERVER.leftRangefinder.aboveThreshold(blockThreshold) || OBSERVER.rightRangefinder.aboveThreshold(blockThreshold)) //already sees it
        {
            return true;
        }

        //search for the block
        if(panRightUntilRightHigh() || panLeftUntilLeftHigh() || panLeftUntilLeftHigh() || panRightUntilRightHigh())
        {
            //found it
            DRIVE_SYSTEM.stop();
            return true;
        }
        else
        {
            //couldn't find it
            return false;  
        }
    }    
    
    void moveToBlockInBuildArea() //remember to handle the centre bumper externally
    {   
        LCD.clear();
        LCD.home();
        
        if(OBSERVER.leftRangefinder.aboveThreshold(blockThreshold) && OBSERVER.rightRangefinder.belowThreshold(blockThreshold))
        {
            error = -1;
            LCD.print("LEFT");
        }
        
        else if(OBSERVER.leftRangefinder.belowThreshold(blockThreshold) && OBSERVER.rightRangefinder.aboveThreshold(blockThreshold))
        {
            //turn right until both high
            
            error = 1;
            LCD.print("RIGHT");
        }
        
        else if(OBSERVER.leftRangefinder.belowThreshold(blockThreshold) && OBSERVER.rightRangefinder.belowThreshold(blockThreshold))
        {
            if(lastError > 0)
            {
              error = 2;
              LCD.print("RIGHT");
            }
            else if(lastError < 0)
            {
              error = -2;
              LCD.print("LEFT");
            }
            else
            {
              if(panLeftUntilLeftHigh() || panRightUntilRightHigh() || panRightUntilRightHigh() || panLeftUntilLeftHigh())
              {
                DRIVE_SYSTEM.stop();
              }
              else
              {
                LCD.clear();
                LCD.home();
                LCD.print("NOT FOUND");
                delay(250);
              }
            }
        }
        else
        {
           error =0;
           LCD.print("CENTRE");
        }
        
        int correction = kPBuildArea * error;
        
        if(correction > 0) //turn right
        {
          DRIVE_SYSTEM.drive(SLOW_MOTOR_SPEED, SLOW_MOTOR_SPEED - correction);
        }
        else
        {
          DRIVE_SYSTEM.drive(SLOW_MOTOR_SPEED + correction, SLOW_MOTOR_SPEED);          
        }
        
        lastError = error;
    }

    bool moveToBlockInDepot()
    {
        OBSERVER.gripperCentreSwitch.enable();
        if(OBSERVER.gripperCentreSwitch.on())
        {
            DRIVE_SYSTEM.stop();
            LCD.clear();
            LCD.home();
            LCD.print("FOUND");
            delay(100);
            return true;
        }
        else if(OBSERVER.leftBumper.on() || OBSERVER.rightBumper.on())
        {
            DRIVE_SYSTEM.stop();
            LCD.clear();
            LCD.home();
            LCD.print("HIT DEPOT");
            delay(100);
            return true;
        }
        
        if(OBSERVER.leftRangefinder.edgeOff(edgeThreshold))
        {
            error = 1;
        }
        else if(OBSERVER.rightRangefinder.edgeOff(edgeThreshold))
        {
            error = -1;
        }
        else if(OBSERVER.leftRangefinder.aboveThreshold(blockThreshold) && OBSERVER.rightRangefinder.aboveThreshold(blockThreshold))
        {
            error = 0;
        }
        else if(OBSERVER.leftRangefinder.belowThreshold(blockThreshold) && OBSERVER.rightRangefinder.belowThreshold(blockThreshold))
        {
          DRIVE_SYSTEM.stop();
          LCD.clear();
          LCD.home();
          LCD.print("LOST IT");
          delay(1000);
          searchForBlockInDepot();
        }
        
        int correction = kPDepot * error;
        
        if(correction > 0)
        {
          DRIVE_SYSTEM.drive(SLOW_MOTOR_SPEED, SLOW_MOTOR_SPEED - correction);
        }
        else
        {
          DRIVE_SYSTEM.drive(SLOW_MOTOR_SPEED + correction, SLOW_MOTOR_SPEED);
        }
        
        ++cycleCount;
        if(cycleCount == 100)
        {
            display();
            cycleCount = 0;
        }
    }
        
    int blockThreshold;
    int edgeThreshold;
    int maxSearchTime;
    int maxEdgeFindingTime;
    int kPBuildArea;
    int kPDepot;
    int error;
    int lastError;
    bool blockTracked;
    
    int cycleCount;
};

extern Rangefinders RANGEFINDERS;

#endif
