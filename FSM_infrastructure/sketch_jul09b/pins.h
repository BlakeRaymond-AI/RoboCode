#ifndef PINS_HEADER_GUARD
#define PINS_HEADER_GUARD

enum ANALOG_IN
{
	LEFT_TAPE_QRD = 0,
	LEFT_OUTBOARD_QRD = 1,
	RIGHT_OUTBOARD_QRD = 2,
	RIGHT_TAPE_QRD = 3,
	LEFT_RANGEFINDER = 4,
	RIGHT_RANGEFINDER = 5
};

enum MOTORS
{
	LEFT_DRIVE_MOTOR = 0,
	RIGHT_DRIVE_MOTOR = 1,
    GRIPPER_MOTOR = 2,
    LIFTER_MOTOR = 3
};

enum DIGITAL_IN
{
    GRIPPER_RIGHT_SWITCH = 0, 
    RIGHT_BUMPER = 1,
    LEFT_BUMPER = 12,
	CENTRE_BUMPER = 11,
	GRIPPER_CENTRE_SWITCH = 10,
    SLIDE_TOP_SWITCH = 13,
    GRIPPER_LEFT_SWITCH = 14,
    SLIDE_BOTTOM_SWITCH = 15
};

enum MOTOR_SPEEDS
{
    MAX_MOTOR_SPEED = 1023,
	FAST_MOTOR_SPEED = 512,
	MEDIUM_MOTOR_SPEED = 300,
	SLOW_MOTOR_SPEED = 250
};

#endif
