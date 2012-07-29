#ifndef DRIVE_SYSTEM_HEADER_GUARD
#define DRIVE_SYSTEM_HEADER_GUARD

class ManagedMotor
{
public:
    ManagedMotor(int pin)
    : pin(pin)
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
        
        motor.speed(pin, -lastSpeed);
    }
    
private:
    int pin;
    float correction;
    int speed;
    int lastSpeed;    
};

class DriveSystem
{
public:
    DriveSystem()
    : leftMotor(LEFT_DRIVE_MOTOR),
    rightMotor(RIGHT_DRIVE_MOTOR),
    inertiaCorrectionTime(150)
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
        
        delay(inertiaCorrection * sqrt(sq(leftMotor.lastSpeed) + sq(rightMotor.lastSpeed)));
        
        leftMotor.stopWithoutDelay();
        rightMotor.stopWithoutDelay();        
    }
    
    ManagedMotor leftMotor;
    ManagedMotor rightMotor;
    int inertiaCorrection;
    
private:
};

extern DriveSystem DRIVE_SYSTEM;

#endif

