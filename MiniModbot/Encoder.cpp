/*
 * Encoder.cpp
 *
 *  Created on: Feb 6, 2016
 *      Author: Claire
 */

#include "Encoder.h"
#include <stdio.h>
#include <stdlib.h>
#include "GLOBAL_DEFINES.h"
#include "mraa.h"
#include <time.h>
#include <unistd.h>


float CPR = 64.0;
float gear_ratio = 30.0;
float usecPerCount = 0;
float RPM = 0;


Encoder::Encoder() {
	initEncoder();
}

Encoder::~Encoder() {

}

int Encoder::initEncoder(){
	// Initialize GPIO pins for left and right input pulse reading
	mraa::Gpio* PULSEIN_L = new mraa::Gpio(PULSEIN_PIN_L);
	if (PULSEIN_L == NULL){
		return 1; //return 1 if unable to initialize pulse input pin
	}
	mraa::Gpio* PULSEIN_R = new mraa::Gpio(PULSEIN_PIN_R);
	if (PULSEIN_R == NULL){
		return 1; //return 1 if unable to initialize pulse input pin
	}

	// Set pin directions to be input
	PULSEIN_L->dir(mraa::DIR_IN);
	PULSEIN_R->dir(mraa::DIR_IN);


	// Attach ISR to pin
	PULSEIN_L->isr(mraa::EDGE_FALLING, &ISR_PULSEIN, NULL);
	PULSEIN_R->isr(mraa::EDGE_FALLING, &ISR_PULSEIN, NULL);
	return 0;
}

/*
 * Return RPM
 */
float Encoder::getRPM(void){
	return RPM;
}

void Encoder::ISR_PULSEIN(void* args){
	static long prevTime = 0;
	static long currTime = 0;
	prevTime = currTime;
	currTime = absoluteTime();
	usecPerCount = currTime - prevTime;
	RPM = 1000000.0 * 60.0 * (4/usecPerCount)/(CPR * gear_ratio);
}

long Encoder::absoluteTime(void){
	struct timespec t;
	clock_gettime(CLOCK_MONOTONIC, &t);
	//	return (t.tv_nsec/1000);
	return ((long)t.tv_sec*1000+t.tv_nsec/1000);
}
