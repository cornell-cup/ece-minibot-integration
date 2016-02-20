/*
 * GLOBAL_DEFINES.h
 *
 *  Created on: Feb 6, 2016
 *      Author: Claire
 */


#ifndef GLOBAL_DEFINES_H_
#define GLOBAL_DEFINES_H_

#define PWM_FREQUENCY 	1000
#define PERIOD_US		(int) ((1/(float)PWM_FREQUENCY)*1000000.0)
#define DUTY_STEP		0.01
#define DUTY_DT_US		50000


// PWM pins are 3, 5, 6, 9, 10, 11
#define MOTOR_LEFT_1_PIN	3
#define MOTOR_LEFT_2_PIN	5
#define MOTOR_RIGHT_1_PIN	6
#define MOTOR_RIGHT_2_PIN	9

#define PULSEIN_PIN_L		8
#define PULSEIN_PIN_R		11

//Target speeds
float TARGET_SPEED_L; //in RPM
float TARGET_SPEED_R; //in RPM

//PID terms
#define PTerm  0.075
#define ITerm  0.00
#define DTerm  0.01

#define IMUlog_file			"/home/imu.txt"

#endif /* GLOBAL_DEFINES_H_ */
