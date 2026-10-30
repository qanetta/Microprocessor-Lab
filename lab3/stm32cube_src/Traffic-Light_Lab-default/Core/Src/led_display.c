/*
 * led_display.c
 *
 *  Created on: Oct 31, 2026
 *      Author: ACER
 *
 *  Output driver: chi module nay goi HAL_GPIO_WritePin cho LED.
 *  Output buffer: led_buffer[] (so tren 4 LED 7 doan).
 */
#include "main.h"
#include "led_display.h"

#define NUM_OF_SEGMENTS	7

// Muc logic lam den giao thong sang: VCC -> dien tro -> LED -> chan MCU
#define LIGHT_ON	GPIO_PIN_RESET
#define LIGHT_OFF	GPIO_PIN_SET

// Output buffer: moi phan tu la 1 chu so cua 1 LED 7 doan
static int led_buffer[NO_OF_LED7SEG] = {0};

static const uint16_t seg_pins[NUM_OF_SEGMENTS] = {
	seg_a_Pin, seg_b_Pin, seg_c_Pin, seg_d_Pin, seg_e_Pin, seg_f_Pin, seg_g_Pin
};
static const uint16_t en_pins[NO_OF_LED7SEG] = { EN0_Pin, EN1_Pin, EN2_Pin, EN3_Pin };

// Bang chan den: light_pins[road][color], tat ca o PORT A
static const uint16_t light_pins[NO_OF_ROADS][NO_OF_COLORS] = {
	[ROAD_1] = { [RED] = R1_Pin, [AMBER] = Y1_Pin, [GREEN] = G1_Pin },
	[ROAD_2] = { [RED] = R2_Pin, [AMBER] = Y2_Pin, [GREEN] = G2_Pin }
};

/* ---------------- LED 7 doan ---------------- */

void display7SEG(int num){
	// Thu tu bit: g f e d c b a (bit 6 -> bit 0), bit = 1 la doan sang
	static const uint8_t segment_patterns[10] = {
		0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F
	};
	uint8_t pattern = (num >= 0 && num <= 9) ? segment_patterns[num] : 0x00;

	for (int i = 0; i < NUM_OF_SEGMENTS; i++){
		// Anode chung: bit = 1 -> xuat muc 0 de doan sang
		HAL_GPIO_WritePin(GPIOB, seg_pins[i], (pattern & (1 << i)) ? GPIO_PIN_RESET : GPIO_PIN_SET);
	}
}

void update7SEG(int index){
	if (index < 0 || index >= NO_OF_LED7SEG) return;

	// tat het -> ghi doan -> bat dung LED (chong ghost)
	for (int i = 0; i < NO_OF_LED7SEG; i++){
		HAL_GPIO_WritePin(GPIOB, en_pins[i], GPIO_PIN_SET);
	}
	display7SEG(led_buffer[index]);
	HAL_GPIO_WritePin(GPIOB, en_pins[index], GPIO_PIN_RESET);
}

void led7_scan(void){
	static int scan_index = 0;		// nho LED dang quet giua cac lan goi
	update7SEG(scan_index);
	scan_index = (scan_index + 1) % NO_OF_LED7SEG;
}

void led7_set_pair(int pair, int value){
	if (pair < 0 || pair >= NO_OF_LED7SEG / 2) return;
	if (value < 0)  value = 0;
	if (value > 99) value = 99;
	led_buffer[pair * 2]     = value / 10;	// hang chuc
	led_buffer[pair * 2 + 1] = value % 10;	// hang don vi
}

/* ---------------- Den giao thong ---------------- */

void traffic_all_off(void){
	for (Road r = 0; r < NO_OF_ROADS; r++){
		for (Color c = 0; c < NO_OF_COLORS; c++){
			HAL_GPIO_WritePin(GPIOA, light_pins[r][c], LIGHT_OFF);
		}
	}
}

void traffic_set(Road road, Color color){
	if (road >= NO_OF_ROADS) return;
	for (Color c = 0; c < NO_OF_COLORS; c++){
		HAL_GPIO_WritePin(GPIOA, light_pins[road][c], (c == color) ? LIGHT_ON : LIGHT_OFF);
	}
}

void traffic_show_all(Color color, int on){
	for (Road r = 0; r < NO_OF_ROADS; r++){
		for (Color c = 0; c < NO_OF_COLORS; c++){
			HAL_GPIO_WritePin(GPIOA, light_pins[r][c], (c == color && on) ? LIGHT_ON : LIGHT_OFF);
		}
	}
}

/* ---------------- LED debug ---------------- */

void led_debug_toggle(void){
	HAL_GPIO_TogglePin(LED_DEBUG_GPIO_Port, LED_DEBUG_Pin);
}
