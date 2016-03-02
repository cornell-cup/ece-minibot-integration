/*
 * Encoder.h
 *
 *  Created on: Feb 6, 2016
 *      Author: Claire
 */

#ifndef ENCODER_H_
#define ENCODER_H_

#include "mraa.hpp"

class Encoder {
public:
	Encoder();
	Encoder(int);
	virtual ~Encoder();

	int initEncoder();
	float getRPM();
	float getusecPerCount();



private:
	long absoluteTime(void);
	void ISR_PULSEIN(void* args);

};



#endif /* ENCODER_H_ */
