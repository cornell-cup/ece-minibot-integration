/*
 * value.cpp
 *
 *  Created on: Dec 12, 2015
 *      Author: Eric Lee
 */

#include <iostream>
#include <string>
#include <stdexcept>

#include <irsensor.h>
#include "aio.h"
#include <mraa/aio.h>

float upm::irsensor::value(float aref, uint8_t samples)
    {
      int val;
      int sum = 0;

      for (int i=0; i<samples; i++)
        {
          val = mraa_aio_read(m_aio);
          sum += val;
        }

      val = sum / samples;
      float volts = float(val) * aref / float(m_aRes);

      return volts;
    }



