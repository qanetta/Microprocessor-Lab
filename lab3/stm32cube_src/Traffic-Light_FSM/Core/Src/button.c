/*
 * button.c
 *
 *  Created on: Oct 28, 2026
 *      Author: ACER
 */

#include "main.h"
#include "button.h"
#include "software_timer.h"

#define LONG_PRESS_MS 1000
#define LONG_PRESS_TICK (LONG_PRESS_MS / TIMER_CYCLE)

static const uint16_t buttons_pin[NUM_OF_BUTTONS] = {BTN1_Pin, BTN2_Pin, BTN3_Pin};

static volatile int button_flag[NUM_OF_BUTTONS];		// ISR ghi 1, main doc roi xoa
static volatile int long_press_flag[NUM_OF_BUTTONS];
static int hold_counter[NUM_OF_BUTTONS];				// chi ISR dung

static int keyReg0[NUM_OF_BUTTONS] = {NORMAL_STATE, NORMAL_STATE, NORMAL_STATE};
static int keyReg1[NUM_OF_BUTTONS] = {NORMAL_STATE, NORMAL_STATE, NORMAL_STATE};
static int keyReg2[NUM_OF_BUTTONS] = {NORMAL_STATE, NORMAL_STATE, NORMAL_STATE};
static int keyReg3[NUM_OF_BUTTONS] = {NORMAL_STATE, NORMAL_STATE, NORMAL_STATE};

void getKeyInput(void){
	for (int i = 0; i < NUM_OF_BUTTONS; i++){
		// Dich thanh ghi: keyReg2 la mau moi nhat
		keyReg0[i] = keyReg1[i];
		keyReg1[i] = keyReg2[i];
		keyReg2[i] = HAL_GPIO_ReadPin(GPIOB, buttons_pin[i]);

		// Chi xu ly khi 3 mau giong nhau (da het doi)
		if ((keyReg0[i] == keyReg1[i]) && (keyReg1[i] == keyReg2[i])){

			if (keyReg2[i] != keyReg3[i]){
				// Co canh: trang thai on dinh moi khac lan chot truoc
				keyReg3[i] = keyReg2[i];
				if (keyReg2[i] == PRESSED_STATE){		// canh xuong = vua nhan
					button_flag[i] = 1;
					hold_counter[i] = LONG_PRESS_TICK;
				}
			}
			else if (keyReg2[i] == PRESSED_STATE){
				// Khong co canh va dang nhan -> dang giu nut
				hold_counter[i]--;
				if (hold_counter[i] <= 0){
					long_press_flag[i] = 1;
					hold_counter[i] = LONG_PRESS_TICK;	// nap lai -> bao lai moi 1s neu tiep tuc giu
				}
			}
		}
	}
}

int isButtonPressed(int index){
	if (index < 0 || index >= NUM_OF_BUTTONS) return 0;
	if (button_flag[index] == 1){
		button_flag[index] = 0;		// doc xong xoa -> moi lan nhan chi bao 1 lan
		return 1;
	}
	return 0;
}

int isButtonLongPressed(int index){
	if (index < 0 || index >= NUM_OF_BUTTONS) return 0;
	if (long_press_flag[index] == 1){
		long_press_flag[index] = 0;
		return 1;
	}
	return 0;
}
