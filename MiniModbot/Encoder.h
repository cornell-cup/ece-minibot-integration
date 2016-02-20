/*
 * Encoder.h
 *
 *  Created on: Feb 6, 2016
 *      Author: Claire
 */

#ifndef ENCODER_H_
#define ENCODER_H_

#include "mraa.h"

static class Encoder {
public:
	Encoder();
	virtual ~Encoder();

	int initEncoder();
	float getRPM();
	float getusecPerCount();
	void ISR_PULSEIN(void* args);

private:
	long Encoder::absoluteTime(void);
};



#endif /* ENCODER_H_ */
