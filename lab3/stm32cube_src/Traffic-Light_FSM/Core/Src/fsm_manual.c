/*
 * fsm_manual.c
 *
 *  Created on: Oct 28, 2026
 *      Author: ACER
 *
 *  Mode 2-4: chinh thoi gian den do / vang / xanh.
 *  - Den dang chinh nhay 2Hz o ca 2 huong
 *  - LED 7 doan: cap 0 = gia tri dang chinh, cap 1 = so mode (02/03/04)
 *  - BTN_MODE: sang mode ke tiep (Mode 4 -> Mode 1), gia tri chua SET bi bo
 *  - BTN_INC : tang gia tri (99 -> 1), giu nut thi tang lien tuc
 *  - BTN_SET : luu gia tri vao duration[]
 *  - 10s khong bam nut nao -> ve Mode 1, gia tri chua SET bi bo
 *  - Khi ve Mode 1: neu do != vang + xanh thi bo het thay doi, dung lai gia tri cu
 */
#include "fsm_manual.h"
#include "global.h"
#include "software_timer.h"
#include "button.h"
#include "traffic_light.h"
#include "led7seg.h"

#define BLINK_MS	250		// 250ms sang + 250ms tat = 2Hz
#define TIMEOUT_MS	10000
#define MAX_VALUE	99
#define MIN_VALUE	1

// Gia tri dang chinh, chi ghi vao duration[] khi nhan SET
static int temp_value = 0;

// Ban sao duration[] luc vao Mode 2, de khoi phuc neu cac gia tri moi sai rang buoc
static int backup[NUM_OF_COLORS];

/* ---------------- Ham noi bo ---------------- */

// MAN_RED -> RED, MAN_AMBER -> AMBER, MAN_GREEN -> GREEN
static Color state_color(Traffic_State s){
	return (Color)(s - MAN_RED);
}

// MAN_RED -> 2, MAN_AMBER -> 3, MAN_GREEN -> 4
static int state_mode(Traffic_State s){
	return s - MAN_RED + 2;
}

static void show_value(void){
	led7seg_setPair(0, temp_value);
	led7seg_setPair(1, state_mode(status));
}

// Bo cac lan nhan cu (vd nhan INC luc o Mode 1) de khong bi ap dung khi vao Mode 2
static void flush_buttons(void){
	for (int i = 0; i < NUM_OF_BUTTONS; i++){
		isButtonPressed(i);
		isButtonLongPressed(i);
	}
}

// Entry action cua moi trang thai MAN_*
static void enter_state(Traffic_State next){
	status = next;
	temp_value = duration[state_color(next)];

	traffic_blink_reset();
	traffic_toggle_color(state_color(next));	// sau reset -> bat den ngay
	setTimer(TIMER_BLINK, BLINK_MS);
	setTimer(TIMER_TIMEOUT, TIMEOUT_MS);

	show_value();
}

// Roi Mode 2-4, ve Mode 1: tat timer rieng, fsm_automatic lo phan con lai tai INIT
static void exit_to_auto(void){
	// Rang buoc: den do 1 huong = xanh + vang huong kia, sai thi khoi phuc gia tri cu
	if (duration[RED] != duration[AMBER] + duration[GREEN]){
		for (int c = 0; c < NUM_OF_COLORS; c++){
			duration[c] = backup[c];
		}
	}
	setTimer(TIMER_BLINK, 0);
	setTimer(TIMER_TIMEOUT, 0);
	traffic_all_off();
	status = INIT;
}

static void increase_value(void){
	temp_value++;
	if (temp_value > MAX_VALUE){
		temp_value = MIN_VALUE;
	}
	show_value();
}

/* ---------------- Ham cho module khac goi ---------------- */

void fsm_manual_start(void){
	for (int c = 0; c < NUM_OF_COLORS; c++){
		backup[c] = duration[c];
	}
	flush_buttons();
	enter_state(MAN_RED);
}

void fsm_manual_run(void){
	if (status != MAN_RED && status != MAN_AMBER && status != MAN_GREEN) return;

	// 1. Het 10s khong bam nut -> ve Mode 1
	if (isTimerExpired(TIMER_TIMEOUT)){
		exit_to_auto();
		return;
	}

	// 2. Nut MODE: sang mode ke tiep, Mode 4 -> Mode 1
	if (isButtonPressed(BTN_MODE)){
		if (status == MAN_GREEN){
			exit_to_auto();
		}
		else{
			enter_state(status + 1);	// MAN_RED -> MAN_AMBER -> MAN_GREEN
		}
		return;
	}

	// 3. Nut INC (nhan hoac giu): tang gia tri, dem lai timeout
	if (isButtonPressed(BTN_INC) || isButtonLongPressed(BTN_INC)){
		increase_value();
		setTimer(TIMER_TIMEOUT, TIMEOUT_MS);
	}

	// 4. Nut SET: luu gia tri, dem lai timeout
	if (isButtonPressed(BTN_SET)){
		duration[state_color(status)] = temp_value;
		setTimer(TIMER_TIMEOUT, TIMEOUT_MS);
	}

	// 5. Nhay den dang chinh
	if (isTimerExpired(TIMER_BLINK)){
		setTimer(TIMER_BLINK, BLINK_MS);
		traffic_toggle_color(state_color(status));
	}
}
