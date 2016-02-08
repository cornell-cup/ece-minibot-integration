#include <gp2y0a.h>
#include <stdlib.h>
#include "mraa.h"
#include <iostream>
#include <unistd.h>
#include <math.h>



 // Instantiate a GP2Y0A on analog pin A1
	upm::GP2Y0A *volts = new upm::GP2Y0A(1);

// Define global variables for voltage
  double R,A,B,C,D;
  float V;

  int main(int argc, char **argv)
   {

	  while (1)
		{

		 V	= volts->value(4.95,1);
		 sleep(1);


		 /*// Returns distance in units of cm for long range.
		  A = 0.0082712905;
		  B = 939.57652;
		  C = -3.3978697;
		  D = 17.339222;
		  // Curve fit.
		  R = (A + B * V) / (1 + C * V + D * V * V);

		 */

		 // Returns distance in units of cm for short range.
		A = 112.734;
		B = - 18.635;
		C = 13.78;
		D = 2.449;

		// Curve fit.
		R = (A + B * V) / (1 + C * V + D * V * V);

		printf ("\nObstacle detected and input voltage is %.2f\t",V);
		printf ("Distance = %.2f\n",R);

		}
	  return(0);

   }
