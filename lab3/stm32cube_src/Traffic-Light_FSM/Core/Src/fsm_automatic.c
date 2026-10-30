/*
 * fsm_automatic.c
 *
 *  Created on: Oct 28, 2026
 *      Author: ACER
 *
 *  Mode 1: den giao thong chay binh thuong.
 *  Vong pha: RED_GREEN -> RED_AMBER -> GREEN_RED -> AMBER_RED -> RED_GREEN ...
 *  (ten pha: <mau huong 1>_<mau huong 2>)
 *  LED 7 doan: cap 0 = so giay con lai huong 1, cap 1 = so giay con lai huong 2.
 */
#include "fsm_automatic.h"
#include "fsm_manual.h"
#include "global.h"
#include "software_timer.h"
#include "button.h"
#include "traffic_light.h"
#include "led7seg.h"

#define SECOND_MS	1000

// So giay con lai cua den dang sang o moi huong (dem nguoc, hien len LED 7 doan)
static int time_road1 = 0;
static int time_road2 = 0;

/* ---------------- Ham noi bo ---------------- */

// Lop bao ve cuoi: neu duration[] sai rang buoc (do = vang + xanh) thi ve mac dinh.
// Binh thuong fsm_manual da kiem tra truoc khi ve Mode 1, nen nhanh nay hiem khi chay.
static void check_duration(void){
	if (duration[RED] != duration[AMBER] + duration[GREEN]
			|| duration[AMBER] < 1 || duration[GREEN] < 1){
		duration[RED]   = 5;
		duration[AMBER] = 2;
		duration[GREEN] = 3;
	}
}

static void show_time(void){
	led7seg_setPair(0, time_road1);
	led7seg_setPair(1, time_road2);
}

// Entry action: chuyen sang pha moi, bat den va nap so giay cho den vua doi mau.
// Den khong doi mau thi giu nguyen so giay dang dem.
static void enter_phase(Traffic_State next){
	status = next;
	switch (next){
		case AUTO_RED_GREEN:
			time_road1 = duration[RED];
			time_road2 = duration[GREEN];
			traffic_set(ROAD_1, RED);
			traffic_set(ROAD_2, GREEN);
			break;
		case AUTO_RED_AMBER:
			time_road2 = duration[AMBER];		// huong 1 van do, dem tiep
			traffic_set(ROAD_2, AMBER);
			break;
		case AUTO_GREEN_RED:
			time_road1 = duration[GREEN];
			time_road2 = duration[RED];
			traffic_set(ROAD_1, GREEN);
			traffic_set(ROAD_2, RED);
			break;
		case AUTO_AMBER_RED:
			time_road1 = duration[AMBER];		// huong 2 van do, dem tiep
			traffic_set(ROAD_1, AMBER);
			break;
		default:
			break;
	}
	show_time();
}

// Pha hien tai ket thuc khi den vua doi mau dem ve 0
static int phase_done(void){
	switch (status){
		case AUTO_RED_GREEN:
		case AUTO_RED_AMBER:	return time_road2 <= 0;	// huong 2 doi mau
		case AUTO_GREEN_RED:
		case AUTO_AMBER_RED:	return time_road1 <= 0;	// huong 1 doi mau
		default:				return 0;
	}
}

static Traffic_State next_phase(Traffic_State s){
	switch (s){
		case AUTO_RED_GREEN:	return AUTO_RED_AMBER;
		case AUTO_RED_AMBER:	return AUTO_GREEN_RED;
		case AUTO_GREEN_RED:	return AUTO_AMBER_RED;
		default:				return AUTO_RED_GREEN;	// AUTO_AMBER_RED quay ve dau
	}
}

/* ---------------- Ham cho main goi ---------------- */

void fsm_automatic_run(void){
	switch (status){
		case INIT:
			// Vao Mode 1 (khoi dong hoac tu Mode 2-4 quay ve)
			check_duration();
			setTimer(TIMER_SECOND, SECOND_MS);
			enter_phase(AUTO_RED_GREEN);
			break;

		case AUTO_RED_GREEN:
		case AUTO_RED_AMBER:
		case AUTO_GREEN_RED:
		case AUTO_AMBER_RED:
			// Nut MODE -> Mode 2
			if (isButtonPressed(BTN_MODE)){
				setTimer(TIMER_SECOND, 0);		// tat nhip dem giay cua Mode 1
				fsm_manual_start();
				break;
			}
			// Moi 1s: dem nguoc ca 2 huong, het pha thi chuyen pha
			if (isTimerExpired(TIMER_SECOND)){
				setTimer(TIMER_SECOND, SECOND_MS);
				time_road1--;
				time_road2--;
				if (phase_done()){
					enter_phase(next_phase(status));
				}
				else{
					show_time();
				}
			}
			break;

		default:
			break;		// MAN_*: do fsm_manual xu ly
	}
}
