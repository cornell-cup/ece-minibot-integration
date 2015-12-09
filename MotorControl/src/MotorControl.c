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
	int sum;

	printf("Please enter a value for the Target RPM");
	scanf("%d", &sum );

	TARGET_SPEED = sum;

	//float RPM;
	//float usec;
	float duty;
	float PID;
	motorLeft_direction(MOTOR_CW);
	motorLeft_drive(0.5);
	sleep(1);
	initEncoder();
	//int i=0;

	while(1){
		usleep(20000);
		PID = motorPID();
		duty = PID/1000.0;
		if(duty <0) duty =0;
		if(duty >0.95) duty =0.95;
		motorLeft_drive(duty);

		/*
		if(++i ==30) {
			RPM = getRPM_L();
			usec = getusecPerCount_L();
			//fprintf(stdout, "PID: %.1f   RPM: %.1f   duty: %.3f   usec: %.1f\n", (float)PID, (float)RPM, duty, usec);
			i=0;
		}
		*/

	}

	while (1){
		float duty = 0.05;

		// start in the CW direction
		motorLeft_direction(MOTOR_CW);
		motorRight_direction(MOTOR_CW);

		// ramp up to 95%
		while (duty < 0.95){
			duty += DUTY_STEP;
			motorLeft_drive(duty);
			motorRight_drive(duty);
			usleep(DUTY_DT_US);
		}

		// ramp down to 5%
		while (duty > 0.05){
			duty -= DUTY_STEP;
			motorLeft_drive(duty);
			motorRight_drive(duty);
			usleep(DUTY_DT_US);
		}

		// brake for 0.5 seconds
		motorLeft_brake();
		motorRight_brake();
		usleep(500000);

		// go to CCW direction
		motorLeft_direction(MOTOR_CCW);
		motorRight_direction(MOTOR_CCW);

		// ramp up to 95%
		while (duty < 0.95){
			duty += DUTY_STEP;
			motorLeft_drive(duty);
			motorRight_drive(duty);
			usleep(DUTY_DT_US);
		}

		// ramp down to 5%
		while (duty > 0.05){
			duty -= DUTY_STEP;
			motorLeft_drive(duty);
			motorRight_drive(duty);
			usleep(DUTY_DT_US);
		}

		// brake for 0.5 seconds
		motorLeft_brake();
		motorRight_brake();
		usleep(500000);

	}

	return MRAA_SUCCESS;
}
