/*
 * Author: Syed Tahmid Mahbub <sm893@cornell.edu>
 */

#include "mraa.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "GLOBAL_DEFINES.h"

#include "motor_drive.h"
#include "motor_encoder.h"
/*
 */

int main()
{
	int left_speed;
	int right_speed;
	float duty_L, PID_L;
	float duty_R, PID_R;

	motorLeft_direction(MOTOR_CW);
	motorLeft_drive(0.05);
	motorRight_direction(MOTOR_CW);
	motorRight_drive(0.05);

	printf("Left Target RPM:");
	scanf("%d", &left_speed);
	printf("Right Target RPM:");
	scanf("%d", &right_speed);


	TARGET_SPEED_L = left_speed;
	TARGET_SPEED_R = right_speed;

	sleep(1);
	initEncoder();

	int i = 0; //for print statements

	while(1){
		usleep(20000);
		PID_L = motorLeft_PID();
		duty_L = PID_L/1000.0;
		if(duty_L <0) duty_L =0;
		if(duty_L >0.95) duty_L =0.95;
		motorLeft_drive(duty_L);

		PID_R = motorRight_PID();
		duty_R = PID_R/1000.0;
		if(duty_R <0) duty_R =0;
		if(duty_R >0.95) duty_R =0.95;
		motorRight_drive(duty_R);

		//Prints RPM and period for testing
		float RPM_L, usec_L;
		float RPM_R, usec_R;
		if(++i ==30) {
			RPM_L = getRPM_L();
			usec_L = getusecPerCount_L();
			RPM_R = getRPM_R();
			usec_R = getusecPerCount_R();
			fprintf(stdout, "LEFT RPM: %.1f  usec: %.1f\n", (float)RPM_L, usec_L);
			fprintf(stdout, "RIGHT RPM: %.1f  usec: %.1f\n", (float)RPM_R, usec_R);
			fprintf(stdout, "\n");
			i=0;
		}
	}
	return MRAA_SUCCESS;
}
