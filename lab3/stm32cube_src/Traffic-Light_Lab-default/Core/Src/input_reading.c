/*
 * input_reading.c
 *
 *  Created on: Oct 31, 2026
 *      Author: ACER
 *
 *  Input driver (Program 1.2 - 1.4 cua de), mo rong cho 3 nut va sua cac loi:
 *  - doc dung chan cua tung nut (de goc luon doc BUTTON_1)
 *  - them {} cho if chong doi (de goc thieu -> chi lenh dau thuoc if)
 *  - khoi tao buffer = RELEASED (mac dinh 0 = GPIO_PIN_RESET = dang nhan)
 *  - is_button_pressed_1s tra ve 0 khi index sai (de goc tra 0xff = "true")
 */
#include "main.h"
#include "input_reading.h"
#include "timer.h"

// Ex4: 1s tinh theo so lan ngat, tu doi khi TIMER_CYCLE thay doi
#define DURATION_FOR_AUTO_INCREASING	(1000 / TIMER_CYCLE)
#define BUTTON_IS_PRESSED				GPIO_PIN_RESET	// pull-up: nhan -> 0
#define BUTTON_IS_RELEASED				GPIO_PIN_SET

// Bang chan cac nut, thu tu khop voi BTN_MODE / BTN_INC / BTN_SET
static GPIO_TypeDef * const button_port[NO_OF_BUTTONS] = { BTN1_GPIO_Port, BTN2_GPIO_Port, BTN3_GPIO_Port };
static const uint16_t       button_pin[NO_OF_BUTTONS]  = { BTN1_Pin,       BTN2_Pin,       BTN3_Pin };

// Ket qua cuoi cung sau chong doi (ISR ghi, main doc -> volatile)
static volatile GPIO_PinState buttonBuffer[NO_OF_BUTTONS] = { BUTTON_IS_RELEASED, BUTTON_IS_RELEASED, BUTTON_IS_RELEASED };
// Hai mau doc lien tiep de chong doi
static GPIO_PinState debounceButtonBuffer1[NO_OF_BUTTONS] = { BUTTON_IS_RELEASED, BUTTON_IS_RELEASED, BUTTON_IS_RELEASED };
static GPIO_PinState debounceButtonBuffer2[NO_OF_BUTTONS] = { BUTTON_IS_RELEASED, BUTTON_IS_RELEASED, BUTTON_IS_RELEASED };
// Co bao nut da giu hon 1s va bo dem tuong ung
static volatile uint8_t flagForButtonPress1s[NO_OF_BUTTONS];
static uint16_t counterForButtonPress1s[NO_OF_BUTTONS];

void button_reading(void){
	for (int i = 0; i < NO_OF_BUTTONS; i++){
		debounceButtonBuffer2[i] = debounceButtonBuffer1[i];
		debounceButtonBuffer1[i] = HAL_GPIO_ReadPin(button_port[i], button_pin[i]);

		// 2 mau lien tiep (cach nhau TIMER_CYCLE) giong nhau -> tin hieu da on dinh
		if (debounceButtonBuffer1[i] == debounceButtonBuffer2[i]){
			buttonBuffer[i] = debounceButtonBuffer1[i];

			if (buttonBuffer[i] == BUTTON_IS_PRESSED){
				// dang nhan: dem len, du 1s thi bat co
				if (counterForButtonPress1s[i] < DURATION_FOR_AUTO_INCREASING){
					counterForButtonPress1s[i]++;
				}
				else{
					flagForButtonPress1s[i] = 1;
				}
			}
			else{
				counterForButtonPress1s[i] = 0;
				flagForButtonPress1s[i] = 0;
			}
		}
	}
}

unsigned char is_button_pressed(unsigned char index){
	if (index >= NO_OF_BUTTONS) return 0;
	return (buttonBuffer[index] == BUTTON_IS_PRESSED);
}

unsigned char is_button_pressed_1s(unsigned char index){
	if (index >= NO_OF_BUTTONS) return 0;
	return (flagForButtonPress1s[index] == 1);
}
