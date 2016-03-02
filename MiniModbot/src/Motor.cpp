/*
 * Motor.cpp
 *
 *  Created on: Feb 6, 2016
 *      Author: Claire
 */

#include "Motor.h"
#include "Encoder.h"
#include "GLOBAL_DEFINES.h"

#ifndef NULL
#define NULL   ((void *) 0)
#endif


mraa::Pwm* PWM_MOTOR = NULL;
mraa::Gpio* DIRECTION_MOTOR = NULL;
mraa::Gpio* BRAKE_MOTOR = NULL;
int pwm;
int MOTOR_1_PIN;
int MOTOR_2_PIN;
int ENCODER_PIN;
float TARGET_SPEED;
Encoder* encoder;

int oldDirection = -1;


Motor::Motor() {
	// TODO Auto-generated constructor stub

}

Motor::Motor(int pin1, int pin2, int enc_pin, float target) {
	MOTOR_1_PIN = pin1;
	MOTOR_2_PIN = pin2;
	ENCODER_PIN = enc_pin;
	TARGET_SPEED = target;
	encoder = new Encoder(ENCODER_PIN);
}

Motor::~Motor() {
	// TODO Auto-generated destructor stub
}

int Motor::motor_drive(float duty){
	/* Set speed between minimum and maximum (set in motor_drive.h)
	 * These are in float -> eg 0.3 = 30% duty
	 *
	 */
	mraa::Result result;

	if (duty > MAX_DUTY){
		duty = MAX_DUTY;
	}

	if (duty < MIN_DUTY){
		duty = MIN_DUTY;
	}

	result = PWM_MOTOR->write(duty);

	if (result == mraa::SUCCESS){
		return 0;
	}
	else{
		return -1;
	}

}

int Motor::motor_direction(int direction){
	/* Configure motor driver for use with specified direction
	 * direction = either MOTOR_CW or MOTOR_CCW
	 * return value = 0 if direction setting was successful
	 * 				= -1 if it failed
	 */

	/* MOTOR+ = OUT1, MOTOR- = OUT2
	 * CCW OPERATION:
	 * 		CONTROL SIGNALS:
	 * 			OUT1 = VS, OUT2 = GND
	 * 			IN1 = 1, IN2 = 0
	 * 		PWM = IN1
	 * 		DIRECTION = IN2 = 0
	 * 	CW OPERATION:
	 * 		CONTROL SIGNALS:
	 * 			OUT1 = GND, OUT2 = VS
	 * 			IN1 = 0, IN2 = 1
	 *		PWM = IN2
	 *		DIRECTION = IN1 = 0
	 *
	 */

	if (direction == oldDirection){
		// direction already set, don't do anything
		return 0;
	}

	oldDirection = direction;

	// if pwm channel was initialized for a direction, close it to enable it to be
	// initialized to a different pin; similar for direction gpio pin
	if (PWM_MOTOR != NULL){
		delete PWM_MOTOR;
		PWM_MOTOR = NULL;
	}

	if (DIRECTION_MOTOR != NULL){
		delete DIRECTION_MOTOR;
		DIRECTION_MOTOR = NULL;
	}

	if (direction == MOTOR_CCW){
		PWM_MOTOR = new mraa::Pwm(MOTOR_1_PIN);
		DIRECTION_MOTOR = new mraa::Gpio(MOTOR_2_PIN);
		pwm = MOTOR_1_PIN;
	}
	else if (direction == MOTOR_CW){
		PWM_MOTOR = new mraa::Pwm(MOTOR_2_PIN);
		DIRECTION_MOTOR = new mraa::Gpio(MOTOR_1_PIN);
		pwm = MOTOR_2_PIN;
	}

	if (PWM_MOTOR == NULL || DIRECTION_MOTOR == NULL){
		fprintf(stderr, "Could not initialize left motor, MRAA error!\n");
		return -1;
	}

	// configure pwm settings
	PWM_MOTOR->period_us(PERIOD_US);
	PWM_MOTOR->enable(true);

	// this sets pwm to work in correct direction
	DIRECTION_MOTOR->dir(mraa::DIR_OUT);
	DIRECTION_MOTOR->write(0);

	return 0;
}


int Motor::motor_brake(void){
	// Free PWM so that pin can be used as GPIO
	if (PWM_MOTOR != NULL){
		delete PWM_MOTOR;
		PWM_MOTOR = NULL;
	}

	oldDirection = -1;

	// Electrical brake when:
	// IN1 = 0, IN2 = 0,
	// or
	// IN1 = 1, IN2 = 1
	// Since direction pin is always set to 0 in motorLeft_direction(),
	// set brake pin to also be 0

	BRAKE_MOTOR = new mraa::Gpio(pwm);
	if (BRAKE_MOTOR == NULL){
		return -1;
	}

	BRAKE_MOTOR->write(0);
	delete BRAKE_MOTOR;

	return 0;
}


float Motor::motor_PID(void){
	// Set constants here
	float Accumulator = 0;
	static float PID = 0;
	static float error = 0;
	float prevError = 0;

	// Calculate the PID
	prevError = error;
	error = encoder->getRPM() - TARGET_SPEED;

	if((error < 0 && prevError > 0) || (error > 0 && prevError <0)){
		Accumulator = 0;
	}

	Accumulator += error;  // accumulator is sum of errors
	PID += -error*PTerm;     // start with proportional gain
	PID += ITerm*Accumulator; // add integral gain and error accumulation
	PID += DTerm*(error-prevError); // differential gain comes next

	return PID;
}
