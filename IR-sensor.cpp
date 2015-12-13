
#include <stdio.h>
#include <stdlib.h>
#include "mraa.hpp"
#include <iostream>
#include <unistd.h>
#include <math.h>
#include <irsensor.h>



 // Instantiate a GP2Y0A on analog pin A1
	upm::irsensor *volts = new upm::irsensor(1);

// Define global variables for voltage
  double R,A,B,C,D;
  float V;
  //when we calibrate the sensors, always start out more than 15 cm away
  bool long_range = true;

  int main(int argc, char **argv)
   {


	  while (1)
		{

		 V	= volts->value(4.95,1);
		 sleep(1);

		 if (V == 2.7)
		 {
			 long_range = not(long_range);
		 }

		 if (long_range)
		 {
			 A = 0.0082712905;
			 B = 939.57652;
			 C = -3.3978697;
			 D = 17.339222;
			 // Curve fit, R in units of cm
			 R = (A + B * V) / (1 + C * V + D * V * V);

		 }
		 else
		 {
			 A = -0.005;
			 B = 0.25855;
			 C = 0.05355;
			 //Curve Fit, R in units of cm
			 R = A*V*V + B*V + C;

		 }

		}
	  return(0);

   }
