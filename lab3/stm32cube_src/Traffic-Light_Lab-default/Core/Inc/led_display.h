/*
 * led_display.h
 *
 *  Created on: Oct 31, 2026
 *      Author: ACER
 */

#ifndef INC_LED_DISPLAY_H_
#define INC_LED_DISPLAY_H_

#define NO_OF_LED7SEG	4

typedef enum{ RED = 0, AMBER, GREEN, NO_OF_COLORS } Color;
typedef enum{ ROAD_1 = 0, ROAD_2, NO_OF_ROADS } Road;	// 2 huong doi dien luon cung mau

/* ---- LED 7 doan (Anode chung, chon LED qua PNP: EN muc 0 = bat) ---- */
void display7SEG(int num);					// xuat so 0-9 ra seg_a..seg_g, so khac -> tat
void update7SEG(int index);					// bat LED thu index (0-3) voi so trong buffer
void led7_scan(void);						// goi dinh ky: quet lan luot LED 0 -> 3
void led7_set_pair(int pair, int value);	// pair 0: LED 0-1, pair 1: LED 2-3, value 0-99

/* ---- 12 den giao thong ---- */
void traffic_all_off(void);
void traffic_set(Road road, Color color);	// huong road chi sang dung mau color
void traffic_show_all(Color color, int on);	// ca 2 huong: mau color theo on, mau khac tat (nhay o Mode 2-4)

/* ---- LED debug ---- */
void led_debug_toggle(void);

#endif /* INC_LED_DISPLAY_H_ */
