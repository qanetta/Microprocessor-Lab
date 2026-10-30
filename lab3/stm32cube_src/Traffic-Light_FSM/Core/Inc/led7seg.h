/*
 * led7seg.h
 *
 *  Created on: Oct 28, 2026
 *      Author: ACER
 */

#ifndef INC_LED7SEG_H_
#define INC_LED7SEG_H_

#define NUM_OF_LED7SEG 4

extern int led_buffer[NUM_OF_LED7SEG];	// dinh nghia trong led7seg.c

void display7SEG(int num);					// xuat so 0-9 ra seg_a..seg_g (PB0-PB6)
void update7SEG(int index);					// bat LED thu index (0-3) voi led_buffer[index]
void led7seg_scan(void);					// goi dinh ky: tu quet lan luot LED 0 -> 3
void led7seg_setPair(int pair, int value);	// pair 0: LED 0-1, pair 1: LED 2-3, value 0-99

#endif /* INC_LED7SEG_H_ */
