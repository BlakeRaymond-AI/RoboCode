#ifndef DRIVE_SYSTEM_HEADER_GUARD
#define DRIVE_SYSTEM_HEADER_GUARD

#include <pins.h>
#include <phys253.h>       //   ***** from 253 template file

class ManagedMotor
{
public:
    ManagedMotor(int pin, float correction)
    : pin(pin),
    correction(correction),
    speed(0),
    lastSpeed(0)
    {
        
    }
    
    void drive(int newSpeed)
    {
        lastSpeed = speed;
        speed = newSpeed*correction;
        
        if(speed < -MAX_MOTOR_SPEED)
            speed = -MAX_MOTOR_SPEED;
        else if(speed > MAX_MOTOR_SPEED)
            speed = MAX_MOTOR_SPEED;
            
        motor.speed(pin, speed);
    }
    
    void stopWithoutDelay()
    {
        lastSpeed = speed;
        speed = 0;
        
        motor.speed(pin, speed);
    }
    
    int pin;
    float correction;
    int speed;
    int lastSpeed;    
};

class DriveSystem
{
public:
    DriveSystem()
    : leftMotor(LEFT_DRIVE_MOTOR, 1.0),
    rightMotor(RIGHT_DRIVE_MOTOR, 1.4),
    inertiaCorrection(1)
    {
        
    }
    
    void drive(int leftSpeed, int rightSpeed)
    {
        leftMotor.drive(leftSpeed);
        rightMotor.drive(rightSpeed);
    }
    
    void turnRight(int speed)
    {
        drive(speed, -speed);
    }
    
    void turnLeft(int speed)
    {
        drive(-speed, speed);
    }
    
    void stop()
    {
        leftMotor.stopWithoutDelay();      
        rightMotor.stopWithoutDelay();      
        
        delay(inertiaCorrection * max(abs(leftMotor.lastSpeed), abs(rightMotor.lastSpeed)));
        
        leftMotor.stopWithoutDelay();
        rightMotor.stopWithoutDelay();   
    }
    
    ManagedMotor leftMotor;
    ManagedMotor rightMotor;
    float inertiaCorrection;
    
private:
};

extern DriveSystem DRIVE_SYSTEM;

#endif

