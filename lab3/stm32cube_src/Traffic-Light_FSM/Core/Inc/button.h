/*
 * button.h
 *
 *  Created on: Oct 28, 2026
 *      Author: ACER
 */

#ifndef INC_BUTTON_H_
#define INC_BUTTON_H_

#include "main.h"

#define NORMAL_STATE	GPIO_PIN_SET	// pull-up: tha nut doc duoc 1
#define PRESSED_STATE	GPIO_PIN_RESET	// nhan nut noi xuong GND: doc duoc 0

// Ten cac nut, phan tu cuoi tu dong la so luong nut
typedef enum{
	BTN_MODE = 0,	// BTN1: chuyen mode 1 -> 2 -> 3 -> 4 -> 1
	BTN_INC,		// BTN2: tang gia tri thoi gian
	BTN_SET,		// BTN3: luu gia tri
	NUM_OF_BUTTONS
} Button_ID;

void getKeyInput(void);				// goi trong ISR moi TIMER_CYCLE ms: doc chan + chong doi
int isButtonPressed(int index);		// tra ve 1 MOT LAN cho moi lan nhan, roi tu xoa co
int isButtonLongPressed(int index);	// tra ve 1 khi dang giu nut lau hon 1s

#endif /* INC_BUTTON_H_ */
