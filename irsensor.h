/*
 * IR-sensor.h
 *
 *  Created on: Dec 12, 2015
 *      Author: Eric Lee
 */

#ifndef IRSENSOR_H_
#define IRSENSOR_H_
#pragma once

#include <iostream>
#include <string>
#include <mraa/aio.h>

namespace upm {

  class irsensor{
  public:
	  //initialize analog pin
    irsensor(int pin);

      //remove analog pin
    ~irsensor();

      //Gets an average voltage value from the sensor
    float value(float aref, uint8_t samples);

  private:
    mraa_aio_context m_aio;
    // ADC resolution
    int m_aRes;
  };
}

#endif /* IR_SENSOR_H_ */
