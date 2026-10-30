/*
 * led7seg.c
 *
 *  Created on: Oct 28, 2026
 *      Author: ACER
 *
 */
#include "main.h"
#include "led7seg.h"

#define NUM_OF_SEGMENTS	7

int led_buffer[NUM_OF_LED7SEG] = {0};

// cau hinh chan
static const uint16_t seg_pins[NUM_OF_SEGMENTS] = {
	seg_a_Pin, seg_b_Pin, seg_c_Pin, seg_d_Pin, seg_e_Pin, seg_f_Pin, seg_g_Pin
};
static const uint16_t en_pins[NUM_OF_LED7SEG] = {
	EN0_Pin, EN1_Pin, EN2_Pin, EN3_Pin
};

void display7SEG(int num){
	static const uint8_t segment_patterns[10] = {
		0x3F,	// 0: a b c d e f
		0x06,	// 1: b c
		0x5B,	// 2: a b d e g
		0x4F,	// 3: a b c d g
		0x66,	// 4: b c f g
		0x6D,	// 5: a c d f g
		0x7D,	// 6: a c d e f g
		0x07,	// 7: a b c
		0x7F,	// 8: tat ca
		0x6F	// 9: a b c d f g
	};

	// So ngoai 0-9: tat het doan, tranh LED moi giu so cu cua LED truoc (ghost)
	uint8_t pattern = (num >= 0 && num <= 9) ? segment_patterns[num] : 0x00;

	// Bit i cua pattern ung voi doan thu i (a = 0 ... g = 6)
	for (int i = 0; i < NUM_OF_SEGMENTS; i++){
		// Anode chung: bit = 1 -> xuat muc 0 de doan sang
		HAL_GPIO_WritePin(GPIOB, seg_pins[i], (pattern & (1 << i)) ? GPIO_PIN_RESET : GPIO_PIN_SET);
	}
}

void update7SEG(int index){
	if (index < 0 || index >= NUM_OF_LED7SEG) return;

	// Tat tat ca led trong luc doi doan khong LED nao sang
	for (int i = 0; i < NUM_OF_LED7SEG; i++){
		HAL_GPIO_WritePin(GPIOB, en_pins[i], GPIO_PIN_SET);
	}
	// Ghi doan cho so cua LED index
	display7SEG(led_buffer[index]);
	// Bat dung LED index -> chi LED nay hien so vua ghi
	HAL_GPIO_WritePin(GPIOB, en_pins[index], GPIO_PIN_RESET);
}

void led7seg_scan(void){
	// moi lan goi chi xu ly 1 LED, lan sau tu chuyen sang LED ke tiep
	static int scan_index = 0;

	update7SEG(scan_index);

	// Quay vong 0 -> 1 -> 2 -> 3 -> 0
	scan_index++;
	if (scan_index >= NUM_OF_LED7SEG){
		scan_index = 0;
	}
}

void led7seg_setPair(int pair, int value){
	// pair 0 -> LED 0,1 ; pair 1 -> LED 2,3 => LED dau tien cua cap = pair * 2
	if (pair < 0 || pair >= NUM_OF_LED7SEG / 2) return;

	// Gioi han 0-99 vi moi cap chi co 2 chu so
	if (value < 0)  value = 0;
	if (value > 99) value = 99;

	int first = pair * 2;
	led_buffer[first]     = value / 10;	// hang chuc: 25 / 10 = 2
	led_buffer[first + 1] = value % 10;	// hang don vi: 25 % 10 = 5
}
