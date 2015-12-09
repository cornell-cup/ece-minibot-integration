#include <stdio.h>
#include <stdlib.h>
#include "GLOBAL_DEFINES.h"
#include "mraa.h"
#include <time.h>
#include <unistd.h>

void ISR_PULSEIN_L();
void ISR_PULSEIN_R();
long absoluteTime(void);

float CPR = 64.0;
float gear_ratio = 30.0;
float RPM_L;
float RPM_R;
long prevTime;
long currTime;
float usecPerCount_L = 0;
float usecPerCount_R = 0;

int initEncoder(void){
	currTime = 0;

	// Initialize GPIO pins for left and right input pulse reading
	mraa_gpio_context PULSEIN_L = mraa_gpio_init(PULSEIN_PIN_L);
	if (PULSEIN_L == NULL){
		return 1; //return 1 if unable to initialize pulse input pin
	}
	mraa_gpio_context PULSEIN_R = mraa_gpio_init(PULSEIN_PIN_R);
	if (PULSEIN_R == NULL){
		return 1; //return 1 if unable to initialize pulse input pin
	}

	// Set pin directions to be input
	mraa_gpio_dir(PULSEIN_L, MRAA_GPIO_IN);
	mraa_gpio_dir(PULSEIN_R, MRAA_GPIO_IN);

	// Attach ISR to pin
	mraa_gpio_isr(PULSEIN_L, MRAA_GPIO_EDGE_FALLING, ISR_PULSEIN_L, NULL);
	mraa_gpio_isr(PULSEIN_R, MRAA_GPIO_EDGE_FALLING, ISR_PULSEIN_R, NULL);
	return 0;
}

float getRPM_L(void){
	return RPM_L;
}

float getRPM_R(void){
	return RPM_R;
}

float getusecPerCount_L(void){
	return usecPerCount_L;
}

float getusecPerCount_R(void){
	return usecPerCount_R;
}

void ISR_PULSEIN_L(void* args){
	prevTime = currTime;
	currTime = absoluteTime();
	usecPerCount_L = currTime - prevTime;
	RPM_L = 1000000.0 * 60.0 * (4/usecPerCount_L)/(CPR * gear_ratio);
}

void ISR_PULSEIN_R(void* args){

	prevTime = currTime;
	currTime = absoluteTime();
	usecPerCount_R = currTime - prevTime;
	RPM_R = 1000000.0 * 60.0 * (4/usecPerCount_R)/(CPR * gear_ratio);
}
long absoluteTime(void){
	// Return absolute time in us
	struct timespec t;
	clock_gettime(CLOCK_MONOTONIC, &t);
	//	return (t.tv_nsec/1000);
	return ((long)t.tv_sec*1000+t.tv_nsec/1000);
}
