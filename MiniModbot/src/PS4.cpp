/*
 * PS4.cpp
 *
 *  Created on: Feb 20, 2016
 *      Author: Claire
 */

#include "PS4.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <sys/time.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/input.h> // SUPER IMPORTANT - FOR EVENTS


const char* gamepadName = "/dev/input/event2";
volatile int gamepad = -1;

event_t joystickEvent = { ev_joystick };

int rcv = -1;
int upd_x = -1, upd_y = -1;
int jx=0,jy=0,jbtn=0;

PS4::PS4() {
	// TODO Auto-generated constructor stub

}

PS4::~PS4() {
	// TODO Auto-generated destructor stub
}

int get_rcv(void)
{
	return rcv;
}

int get_updx(void)
{
	return upd_x;
}

int get_updy(void)
{
	return upd_y;
}

void get_joystickEvent(int* x, int* y, int* btn)
{
	*x = jx;
	*y = jy;
	*btn = jbtn;
}

void gp_postEvent(event_t joystickEvent)
{
	// joystickEvent contains 3 ints: btn, x, y
	static int oldx=0, oldy=0;
	upd_x = -1;
	upd_y = -1;
	jx = joystickEvent.x;
	jy = joystickEvent.y;

	jbtn = joystickEvent.btn;

	if (joystickEvent.x != oldx)
	{
		upd_x = 0;
	}
	if (joystickEvent.y != oldy)
	{
		upd_y = 0;
	}
	oldx = joystickEvent.x;
	oldy = joystickEvent.y;
}

void gp_init(void)
{
	fprintf(stdout, "Attempting to connect to gamepad!\n");
	do {
		gamepad = open(gamepadName, O_RDONLY | O_NONBLOCK);
		//if (gamepad < 0)
		//fprintf(stderr, "Cannot access gamepad at %s\n", gamepadName);
	} while (gamepad < 0);

	fprintf(stdout, "Connection to gamepad successful!\n");
}

void gp_quit(void)
{
	if (gamepad >= 0)
		close(gamepad);
}

void gp_getEvent(void)
{
	struct input_event ev[64];
	joystickEvent.btn = 0;
	int rd = read(gamepad, ev, sizeof(ev));
	if (rd < (int) sizeof(struct input_event))
	{
		rcv = -1;
		return;
	}
	int i;
	for (i = 0; i < rd / (int)sizeof(struct input_event); i++)
	{
		struct input_event e = ev[i];
		switch (e.type)
		{
		case EV_ABS:
			if (e.code == ABS_HAT0X) // Digital pins L and R
				joystickEvent.x = e.value;
			else if (e.code == ABS_HAT0Y) // Digital pins U and D
				joystickEvent.y = -e.value;
			break;
		case EV_KEY:
			// Map keys (SEE HEADER FILE):
			joystickEvent.btn = e.code; //Button pressed
			break;
		}
		gp_postEvent(joystickEvent);
		// Do something with this info
		rcv = 0;
	}
}
