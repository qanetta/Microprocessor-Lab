/*
 * global.c
 *
 *  Created on: Oct 28, 2026
 *      Author: ACER
 */
#include "global.h"

Traffic_State status = INIT;

int duration[NUM_OF_COLORS] = {
	[RED]   = 5,
	[AMBER] = 2,
	[GREEN] = 3
};
