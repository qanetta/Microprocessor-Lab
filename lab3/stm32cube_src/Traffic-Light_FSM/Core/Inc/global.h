/*
 * global.h
 *
 *  Created on: Oct 28, 2026
 *      Author: ACER
 */

#ifndef INC_GLOBAL_H_
#define INC_GLOBAL_H_

// Trang thai he thong: dung chung cho fsm_automatic va fsm_manual
typedef enum{
	INIT = 0,
	AUTO_RED_GREEN = 1, AUTO_RED_AMBER, AUTO_GREEN_RED, AUTO_AMBER_RED,	// Mode 1
	MAN_RED = 11, MAN_AMBER, MAN_GREEN									// Mode 2, 3, 4
} Traffic_State;

// Chi so mau, dung cho duration[] va traffic_light
typedef enum{ RED = 0, AMBER, GREEN, NUM_OF_COLORS } Color;

// Hai huong duong: hai huong doi dien luon cung mau
typedef enum{ ROAD_1 = 0, ROAD_2, NUM_OF_ROADS } Road;

extern Traffic_State status;
extern int duration[NUM_OF_COLORS];	// thoi gian moi mau (giay), dinh nghia trong global.c

#endif /* INC_GLOBAL_H_ */
