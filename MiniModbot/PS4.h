/*
 * PS4.h
 *
 *  Created on: Feb 20, 2016
 *      Author: Claire
 */

#ifndef PS4_H_
#define PS4_H_

#include "mraa.h"


	typedef enum {false, true} boolean;

	typedef unsigned char byte;
	#define MAXCHAR ((char)0x7f)
	#define MAXSHORT ((short)0x7fff)
	// Max pos 32-bit int.
	#define MAXINT ((int)0x7fffffff)
	#define MAXLONG ((long)0x7fffffff)
	#define MINCHAR ((char)0x80)
	#define MINSHORT ((short)0x8000)

	// Max negative 32-bit integer.
	#define MININT ((int)0x80000000)
	#define MINLONG ((long)0x80000000)

	#define BTN_SQUARE 		0x130
	#define BTN_CROSS 		0x131
	#define BTN_CIRCLE 		0x132
	#define BTN_TRIANGLE 	0x133
	#define BTN_LB 			0x134
	#define BTN_RB 			0x135
	#define BTN_LT 			0x136
	#define BTN_RT 			0x137
	#define BTN_SHARE 		0x138
	#define BTN_OPTIONS 	0x139
	#define BTN_LSTICK 		0x13a
	#define BTN_RSTICK 		0x13b
	#define BTN_PS 			0x13c
	#define BTN_TOUCHPAD 	0x13d
	#define MAXEVENTS 		64

	// Input event types
	typedef enum
	{
		ev_keydown,
		ev_keyup,
		ev_mouse,
		ev_joystick
	} evtype_t;

	// Event structure
	typedef struct
	{
		evtype_t type;
		int btn; // keys / mouse/joystick buttons
		int x; // mouse/joystick x move
		int y; // mouse/joystick y move
	} event_t;

class PS4 {
public:
	PS4();
	virtual ~PS4();

	int get_rcv(void);
	int get_updx(void);
	int get_updy(void);
	void get_joystickEvent(int* x, int* y, int* btn);
	void gp_postEvent(event_t joystickEvent);
	void gp_init(void);
	void gp_quit(void);
	void gp_getEvent(void);
};

#endif /* PS4_H_ */
