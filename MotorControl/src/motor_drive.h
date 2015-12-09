/*
 * motor_drive.h
 *
 *  Created on: Nov 4, 2015
 *      Author: Cornell Cup
 */

#ifndef MOTOR_DRIVE_H_
#define MOTOR_DRIVE_H_

#include "mraa.h"

#define MOTOR_CW	0
#define MOTOR_CCW	1

#define MAX_DUTY	0.95
#define MIN_DUTY	0.05

/*
 * motor direction function must be called before using drive or brake functions
 */

int motorLeft_drive(float duty);
int motorLeft_direction(int direction);
int motorLeft_brake(void);

int motorRight_drive(float duty);
int motorRight_direction(int direction);
int motorRight_brake(void);

float motorPID(void);

#endif /* MOTOR_DRIVE_H_ */

