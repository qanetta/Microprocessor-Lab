/*
 * input_reading.h
 *
 *  Created on: Oct 31, 2026
 *      Author: ACER
 */

#ifndef INC_INPUT_READING_H_
#define INC_INPUT_READING_H_

#define NO_OF_BUTTONS	3

// Chi so nut (thu tu trong bang chan cua input_reading.c)
#define BTN_MODE		0	// BTN1 - PB12: chon mode 1 -> 2 -> 3 -> 4 -> 1
#define BTN_INC			1	// BTN2 - PB13: tang gia tri
#define BTN_SET			2	// BTN3 - PB14: luu gia tri

void button_reading(void);										// goi trong ngat TIM2
unsigned char is_button_pressed(unsigned char index);			// 1 khi nut DANG duoc nhan (muc, khong phai su kien)
unsigned char is_button_pressed_1s(unsigned char index);		// 1 khi nut duoc giu lien tuc > 1s

#endif /* INC_INPUT_READING_H_ */
