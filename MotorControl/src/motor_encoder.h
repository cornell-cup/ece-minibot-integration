/*
 * motor_encoder.h
 *
 *  Created on: Nov 20, 2015
 *      Author: Claire
 */

#ifndef MOTOR_ENCODER_H_
#define MOTOR_ENCODER_H_


int initEncoder();
float getRPM_L();
float getRPM_R();
float getusecPerCount_L();
float getusecPerCount_R();


#endif /* MOTOR_ENCODER_H_ */
