/*
 * main.cpp
 *
 *  Created on: Feb 9, 2016
 *      Author: Claire
 */

#include "mraa.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "GLOBAL_DEFINES.h"

#include "Motor.h"
#include "Encoder.h"
#include "PS4.h"

#include <pthread.h>
#include <sched.h>
#include <time.h>

#include <string.h>

#define DIRECTION_STOP		0
#define DIRECTION_FORWARD	1
#define DIRECTION_BACKWARD	2
#define DIRECTION_LEFT		3
#define DIRECTION_RIGHT		4
#define DIRECTION_NULL		5

// Define thread functions
void* logImu(void*);
void* gamepadGet(void*);
void* pidLoop(void*);

//Initializes left and right speed in RPM
int left_speed = 30;
int right_speed = 30;

int speed_increment = 3;

//Initializes direction
volatile int direction = DIRECTION_STOP;

int f_logImu = 0;

//Create left and right motors
Motor motorLeft = *(new Motor(MOTOR_LEFT_1_PIN, MOTOR_LEFT_2_PIN, PULSEIN_PIN_L, TARGET_SPEED_L));
Motor motorRight = *(new Motor(MOTOR_RIGHT_1_PIN, MOTOR_RIGHT_2_PIN, PULSEIN_PIN_R, TARGET_SPEED_R));

int main()
{
	//Pthreads for game pad, motor PID, and logging IMU data
	pthread_t gp_thread, pid_thread, imu_thread;
	pthread_create(&gp_thread, NULL, gamepadGet, NULL);
	pthread_create(&pid_thread, NULL, pidLoop, NULL);
	pthread_create(&imu_thread, NULL, logImu, NULL);

	TARGET_SPEED_L = left_speed;
	TARGET_SPEED_R = right_speed;

	while (1){
	}

	pthread_exit(NULL);
	return MRAA_SUCCESS;
}


volatile int OLD_DIRECTION = DIRECTION_NULL;
/*
 * Calculates PID error from actual motor speeds from motor_encoder.c
 * Sets motor direction and speed based on input from game pad
 */
void* pidLoop(void* args){

	static float duty_L, PID_L;
	static float duty_R, PID_R;

//	static int OLD_DIRECTION;

//	OLD_DIRECTION = DIRECTION_NULL;

	while(1){
		usleep(20000);

		//Set left and right target speeds
		TARGET_SPEED_L = left_speed;
		TARGET_SPEED_R = right_speed;

		//Brake both motors
		if (direction == DIRECTION_STOP)
			if (OLD_DIRECTION != DIRECTION_STOP){
				left_speed = 30;
				right_speed = 30;
				TARGET_SPEED_L = left_speed;
				TARGET_SPEED_R = right_speed;
				duty_R = MIN_DUTY;
				duty_L = MIN_DUTY;


				motorLeft.motor_direction(MOTOR_CW);
				motorRight.motor_direction(MOTOR_CW);
				motorLeft.motor_drive(duty_L);
				motorRight.motor_drive(duty_R);

				motorLeft.motor_brake();
				motorRight.motor_brake();

				OLD_DIRECTION = DIRECTION_STOP;

//				continue;
			}

//		OLD_DIRECTION = DIRECTION_NULL;

		//Perform PID calculation on left motor if left motor is not braked
		if (direction != DIRECTION_LEFT){
			PID_L = motorLeft.motor_PID();
			duty_L = PID_L/1000.0;
			if(duty_L <0) duty_L =0;
			if(duty_L >0.95) duty_L =0.95;
		}

		//Perform PID calculation on right motor if right motor is not braked
		if (direction != DIRECTION_RIGHT){
			PID_R = motorRight.motor_PID();
			duty_R = PID_R/1000.0;
			if(duty_R <0) duty_R =0;
			if(duty_R >0.95) duty_R =0.95;
		}

		switch (direction){
		//FORWARD: drive both motors at target speed in forward direction
		case DIRECTION_FORWARD:
			motorLeft.motor_direction(MOTOR_CW);
			motorRight.motor_direction(MOTOR_CCW);
			motorLeft.motor_drive(duty_L);
			motorRight.motor_drive(duty_R);

			OLD_DIRECTION = DIRECTION_FORWARD;

			break;
			//BACKWARD: drive both motors at target speed in backwards direction
		case DIRECTION_BACKWARD:
			motorLeft.motor_direction(MOTOR_CCW);
			motorRight.motor_direction(MOTOR_CW);
			motorLeft.motor_drive(duty_L);
			motorRight.motor_drive(duty_R);

			OLD_DIRECTION = DIRECTION_BACKWARD;
			break;
			//LEFT: brake left motor, drive right motor at target speed in forwards direction
		case DIRECTION_LEFT:
			motorLeft.motor_brake();
			motorRight.motor_direction(MOTOR_CCW);
			motorRight.motor_drive(duty_R);

			OLD_DIRECTION = DIRECTION_LEFT;

			break;
			//RIGHT: brake right motor, drive left motor at target speed in forwards direction
		case DIRECTION_RIGHT:
			motorRight.motor_brake();
			motorLeft.motor_direction(MOTOR_CW);
			motorLeft.motor_drive(duty_L);

			OLD_DIRECTION = DIRECTION_RIGHT;

			break;
		}
	}

	pthread_exit(NULL);
}

/*
 * Gets input from game pad and sets direction and speed accordingly
 * Keypad inputs and corresponding modbot behavior:
 * UP: move forward with increasing speed
 * DOWN: move backward with increasing speed
 * LEFT: turn left
 * RIGHT: turn right
 * CROSS: brake both motors
 * CIRCLE: start/stop logging IMU data
 * If neither UP, DOWN, LEFT, or RIGHT are being pressed, decrease motor speed
 */
void* gamepadGet(void* args){
	static int x=0, y=0, btn=0;

	// Assume that PS4 controller is already paired and connected
	gp_init(); // Initialize gamepad control

	while (1)
	{
		gp_getEvent(); // Event handling
		get_joystickEvent(&x, &y, &btn); //Get x, y, and button information

		//UP pressed
		if (y == 1){
			left_speed = left_speed + speed_increment;
			right_speed = right_speed + speed_increment;
			direction = DIRECTION_FORWARD;
			usleep(100000);
		}
		//DOWN pressed
		else if (y == -1){
			left_speed = left_speed + speed_increment;
			right_speed = right_speed + speed_increment;
			direction = DIRECTION_BACKWARD;
			usleep(100000);
		}
		//RIGHT pressed
		else if (x == 1){
			direction = DIRECTION_RIGHT;
			usleep(100000);
		}
		//LEFT pressed
		else if (x == -1){
			direction = DIRECTION_LEFT;
			usleep(100000);
		}

		//UP, DOWN, LEFT, RIGHT NOT pressed, start decreasing speed
		if  (y == 0 && x == 0){
			if (left_speed < 10){
				left_speed = 9;
			}
			else{
				left_speed = left_speed - speed_increment;
				right_speed = right_speed - speed_increment;
				usleep(10000);
			}
		}

		switch (btn){
		//BRAKE
		case BTN_CROSS:
			direction = DIRECTION_STOP;
			usleep(100000);
			break;
		//IMU
		case BTN_CIRCLE:
			f_logImu = 1 - f_logImu;
			if (f_logImu){
				fprintf(stdout, "Start logging IMU data!\n");
			}
			else{
				fprintf(stdout, "Stop logging IMU data!\n");
			}
			usleep(100000);
			break;
		}
	}

	gp_quit();

	pthread_exit(NULL);
}

/*
 * Log data from IMU
 */
void* logImu(void* args){

	FILE* fp;

	fp = fopen(IMUlog_file, "a");

	if (fp == NULL){
		fprintf(stderr, "Couldn't open file!\n");
		while (1);
	}

	mraa_i2c_context i2c = mraa_i2c_init(6);
	unsigned char rx_tx_buf[20] = {0};

	mraa_i2c_address(i2c, 40);
	if (mraa_i2c_read_byte_data(i2c, 0x00) == 0xA0) {
		fprintf(stdout, "IMU: Correct Device Address.\n");
	} else {
		fprintf(stderr, "IMU: Incorrect Device Address.\n");
		while (1){

		}
	}

	if (mraa_i2c_read_byte_data(i2c, 0x36) == 0x0F) {
		fprintf(stdout, "IMU: Self-Test Passed.\n");
	} else {
		fprintf(stderr, "IMU: Self-Test failed.\n");
		while (1);
	}

	mraa_i2c_write_byte_data(i2c, 0x0C, 0x3D);

	while (1) {
		if (f_logImu){
			int16_t roll, pitch, heading;
			mraa_i2c_read_bytes_data(i2c, 0x1A, rx_tx_buf, 6);
			heading = rx_tx_buf[1]*256+rx_tx_buf[0];
			roll = rx_tx_buf[3]*256+rx_tx_buf[2];
			pitch = rx_tx_buf[5]*256+rx_tx_buf[4];
			fprintf(fp, "H: % 6.2f\tR: % 6.2f\tP: % 6.2f\t",heading/16.0, roll/16.0, pitch/16.0);
			int16_t lia_x, lia_y, lia_z;
			mraa_i2c_read_bytes_data(i2c, 0x28, rx_tx_buf, 6);
			lia_x = rx_tx_buf[1]*256+rx_tx_buf[0];
			lia_y = rx_tx_buf[3]*256+rx_tx_buf[2];
			lia_z = rx_tx_buf[5]*256+rx_tx_buf[4];
			fprintf(fp, "AX: % 6.2f\tAY: % 6.2f\tAZ: % 6.2f\t",lia_x/100.0, lia_y/100.0, lia_z/100.0);
			mraa_i2c_read_bytes_data(i2c, 0x2E, rx_tx_buf, 6);
			int16_t g_x, g_y, g_z;
			g_x = rx_tx_buf[1]*256+rx_tx_buf[0];
			g_y = rx_tx_buf[3]*256+rx_tx_buf[2];
			g_z = rx_tx_buf[5]*256+rx_tx_buf[4];
			fprintf(fp, "GX: % 6.2f\tGY: % 6.2f\tGZ: % 6.2f\n",g_x/100.0, g_y/100.0, g_z/100.0);
			usleep(5337);
		}
	}
}
