/*
 * input_processing.c
 *
 *  Created on: Oct 31, 2026
 *      Author: ACER
 *
 *  input_output_processing_fsm (Figure 1.5 cua de):
 *  doc input buffer (input_reading) -> xu ly FSM -> ghi output (led_display).
 *
 *  Gom 2 tang FSM:
 *  1. FSM xu ly nut (Figure 1.9): bien MUC nhan nut thanh SU KIEN
 *     - vua nhan -> 1 su kien
 *     - giu > 1s -> su kien lap lai moi 500ms (chi nut INC)
 *  2. FSM mode den giao thong:
 *     Mode 1 (binh thuong) -> Mode 2 (do) -> Mode 3 (vang) -> Mode 4 (xanh) -> Mode 1
 *     Mode 2-4: den dang chinh nhay 2Hz, cap LED 0 = gia tri, cap LED 1 = so mode,
 *     nut 2 tang gia tri (1-99), nut 3 luu gia tri,
 *     10s khong bam nut -> ve Mode 1 (bo gia tri chua luu).
 */
#include "main.h"
#include "input_processing.h"
#include "input_reading.h"
#include "led_display.h"
#include "timer.h"

#define SECOND_MS			1000
#define BLINK_MS			250		// 250ms sang + 250ms tat = 2Hz
#define AUTO_INC_MS			500		// giu nut 2 > 1s: tang moi 500ms
#define MANUAL_TIMEOUT_MS	10000
#define MIN_VALUE			1
#define MAX_VALUE			99

/* ================= FSM xu ly nut (Figure 1.9) ================= */

enum ButtonState{ BUTTON_RELEASED, BUTTON_PRESSED, BUTTON_PRESSED_MORE_THAN_1_SECOND };
static enum ButtonState buttonState[NO_OF_BUTTONS] = { BUTTON_RELEASED, BUTTON_RELEASED, BUTTON_RELEASED };

// Tra ve 1 khi can thuc hien hanh dong cua nut index.
// auto_repeat = 1: giu > 1s thi tra ve 1 lap lai moi AUTO_INC_MS.
static int button_event(int index, int auto_repeat){
	int event = 0;
	switch (buttonState[index]){
		case BUTTON_RELEASED:
			if (is_button_pressed(index)){
				buttonState[index] = BUTTON_PRESSED;
				event = 1;							// vua nhan: 1 su kien
			}
			break;
		case BUTTON_PRESSED:
			if (!is_button_pressed(index)){
				buttonState[index] = BUTTON_RELEASED;
			}
			else if (auto_repeat && is_button_pressed_1s(index)){
				buttonState[index] = BUTTON_PRESSED_MORE_THAN_1_SECOND;
				setTimer(TIMER_AUTO_INC, AUTO_INC_MS);
				event = 1;
			}
			break;
		case BUTTON_PRESSED_MORE_THAN_1_SECOND:
			if (!is_button_pressed(index)){
				buttonState[index] = BUTTON_RELEASED;
				setTimer(TIMER_AUTO_INC, 0);
			}
			else if (isTimerExpired(TIMER_AUTO_INC)){
				setTimer(TIMER_AUTO_INC, AUTO_INC_MS);
				event = 1;							// tang tu dong
			}
			break;
	}
	return event;
}

/* ================= FSM den giao thong ================= */

typedef enum{ MODE_NORMAL = 1, MODE_RED, MODE_AMBER, MODE_GREEN } Mode;	// gia tri = so mode hien tren LED
// Pha Mode 1: <mau huong 1>_<mau huong 2>
typedef enum{ PHASE_RED_GREEN, PHASE_RED_AMBER, PHASE_GREEN_RED, PHASE_AMBER_RED } Phase;

static Mode  mode  = MODE_NORMAL;
static Phase phase = PHASE_RED_GREEN;

static int duration[NO_OF_COLORS] = { [RED] = 5, [AMBER] = 2, [GREEN] = 3 };	// giay
static int backup[NO_OF_COLORS];		// ban sao luc vao Mode 2, khoi phuc neu sai rang buoc

static int time_road1 = 0;				// so giay con lai cua den dang sang moi huong
static int time_road2 = 0;
static int temp_value = 0;				// gia tri dang chinh o Mode 2-4, chi luu khi nhan SET
static int blink_on   = 0;

/* ---- Mode 1 ---- */

static void show_time(void){
	led7_set_pair(0, time_road1);
	led7_set_pair(1, time_road2);
}

// Entry action cua tung pha: bat den + nap so giay cho den vua doi mau
static void enter_phase(Phase next){
	phase = next;
	switch (next){
		case PHASE_RED_GREEN:
			time_road1 = duration[RED];
			time_road2 = duration[GREEN];
			traffic_set(ROAD_1, RED);
			traffic_set(ROAD_2, GREEN);
			break;
		case PHASE_RED_AMBER:
			time_road2 = duration[AMBER];		// huong 1 van do, dem tiep
			traffic_set(ROAD_2, AMBER);
			break;
		case PHASE_GREEN_RED:
			time_road1 = duration[GREEN];
			time_road2 = duration[RED];
			traffic_set(ROAD_1, GREEN);
			traffic_set(ROAD_2, RED);
			break;
		case PHASE_AMBER_RED:
			time_road1 = duration[AMBER];		// huong 2 van do, dem tiep
			traffic_set(ROAD_1, AMBER);
			break;
	}
	show_time();
}

// Moi 1s: dem nguoc ca 2 huong, den vua doi mau ve 0 thi sang pha ke tiep
static void normal_run(void){
	if (!isTimerExpired(TIMER_SECOND)) return;
	setTimer(TIMER_SECOND, SECOND_MS);
	time_road1--;
	time_road2--;

	switch (phase){
		case PHASE_RED_GREEN:	if (time_road2 <= 0) enter_phase(PHASE_RED_AMBER); else show_time(); break;
		case PHASE_RED_AMBER:	if (time_road2 <= 0) enter_phase(PHASE_GREEN_RED); else show_time(); break;
		case PHASE_GREEN_RED:	if (time_road1 <= 0) enter_phase(PHASE_AMBER_RED); else show_time(); break;
		case PHASE_AMBER_RED:	if (time_road1 <= 0) enter_phase(PHASE_RED_GREEN); else show_time(); break;
	}
}

/* ---- Mode 2-4 ---- */

static Color mode_color(Mode m){
	return (Color)(m - MODE_RED);		// MODE_RED -> RED, MODE_AMBER -> AMBER, MODE_GREEN -> GREEN
}

static void show_value(void){
	led7_set_pair(0, temp_value);
	led7_set_pair(1, mode);
}

/* ---- Chuyen mode: moi chuyen trang thai deu di qua day (entry action) ---- */

static void enter_mode(Mode next){
	Mode prev = mode;
	mode = next;

	if (next == MODE_NORMAL){
		// Rang buoc: do 1 huong = xanh + vang huong kia, sai thi khoi phuc gia tri cu
		if (duration[RED] != duration[AMBER] + duration[GREEN]){
			for (int c = 0; c < NO_OF_COLORS; c++) duration[c] = backup[c];
		}
		setTimer(TIMER_BLINK, 0);
		setTimer(TIMER_TIMEOUT, 0);
		setTimer(TIMER_SECOND, SECOND_MS);
		enter_phase(PHASE_RED_GREEN);
		return;
	}

	if (prev == MODE_NORMAL){				// tu Mode 1 vao Mode 2
		for (int c = 0; c < NO_OF_COLORS; c++) backup[c] = duration[c];
		setTimer(TIMER_SECOND, 0);
	}
	temp_value = duration[mode_color(next)];
	blink_on = 1;							// vao mode moi: bat den ngay
	traffic_show_all(mode_color(next), blink_on);
	setTimer(TIMER_BLINK, BLINK_MS);
	setTimer(TIMER_TIMEOUT, MANUAL_TIMEOUT_MS);
	show_value();
}

/* ================= Ham public ================= */

void fsm_init(void){
	for (int c = 0; c < NO_OF_COLORS; c++) backup[c] = duration[c];
	traffic_all_off();
	enter_mode(MODE_NORMAL);
}

void fsm_for_input_processing(void){
	// Goi FSM cua ca 3 nut moi vong lap de trang thai nut luon cap nhat
	int mode_pressed = button_event(BTN_MODE, 0);
	int inc_pressed  = button_event(BTN_INC, 1);
	int set_pressed  = button_event(BTN_SET, 0);

	switch (mode){
		case MODE_NORMAL:
			if (mode_pressed){
				enter_mode(MODE_RED);
			}
			else{
				normal_run();				// nut 2, 3 khong co tac dung o Mode 1
			}
			break;

		case MODE_RED:
		case MODE_AMBER:
		case MODE_GREEN:
			if (mode_pressed){
				enter_mode(mode == MODE_GREEN ? MODE_NORMAL : mode + 1);
				break;
			}
			if (isTimerExpired(TIMER_TIMEOUT)){
				enter_mode(MODE_NORMAL);
				break;
			}
			if (inc_pressed){
				temp_value = (temp_value >= MAX_VALUE) ? MIN_VALUE : temp_value + 1;
				show_value();
				setTimer(TIMER_TIMEOUT, MANUAL_TIMEOUT_MS);
			}
			if (set_pressed){
				duration[mode_color(mode)] = temp_value;
				setTimer(TIMER_TIMEOUT, MANUAL_TIMEOUT_MS);
			}
			if (isTimerExpired(TIMER_BLINK)){
				setTimer(TIMER_BLINK, BLINK_MS);
				blink_on = !blink_on;
				traffic_show_all(mode_color(mode), blink_on);
			}
			break;
	}
}
