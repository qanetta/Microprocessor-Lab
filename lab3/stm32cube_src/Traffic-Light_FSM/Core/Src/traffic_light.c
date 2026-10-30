/*
 * traffic_light.c
 *
 *  Created on: Oct 28, 2026
 *      Author: ACER
 *
 *  Output driver cho 12 den giao thong.
 *  Moi chan R1/Y1/G1 dieu khien 2 den cua 2 huong doi dien (cung mau)
 *  -> chi can 6 chan cho 12 den.
 */
#include "main.h"
#include "traffic_light.h"

// Muc logic lam den sang. Den noi: VCC -> dien tro -> LED -> chan MCU thi sang o muc 0.
// Neu mach sang o muc 1 thi chi doi 2 dong nay.
#define LED_ON	GPIO_PIN_RESET
#define LED_OFF	GPIO_PIN_SET

// 1 den = 1 cap (port, pin)
typedef struct{
	GPIO_TypeDef *port;
	uint16_t pin;
} Led_Pin;

// Bang chan: light_pins[road][color]
static const Led_Pin light_pins[NUM_OF_ROADS][NUM_OF_COLORS] = {
	[ROAD_1] = {
		[RED]   = { R1_GPIO_Port, R1_Pin },
		[AMBER] = { Y1_GPIO_Port, Y1_Pin },
		[GREEN] = { G1_GPIO_Port, G1_Pin }
	},
	[ROAD_2] = {
		[RED]   = { R2_GPIO_Port, R2_Pin },
		[AMBER] = { Y2_GPIO_Port, Y2_Pin },
		[GREEN] = { G2_GPIO_Port, G2_Pin }
	}
};

// Pha nhay dung chung cho ca 2 huong: 1 = dang sang, 0 = dang tat
static int blink_on = 0;

/* Ham noi bo */

// Bat (on = 1) hoac tat (on = 0) dung 1 den
static void write_light(Road road, Color color, int on){
	const Led_Pin *led = &light_pins[road][color];
	HAL_GPIO_WritePin(led->port, led->pin, on ? LED_ON : LED_OFF);
}

// Huong road: mau color theo on, 2 mau con lai luon tat
static void show_only(Road road, Color color, int on){
	for (Color c = 0; c < NUM_OF_COLORS; c++){
		write_light(road, c, (c == color) && on);
	}
}

/* Ham cho FSM goi- */

void traffic_all_off(void){
	for (Road r = 0; r < NUM_OF_ROADS; r++){
		for (Color c = 0; c < NUM_OF_COLORS; c++){
			write_light(r, c, 0);
		}
	}
}

void traffic_set(Road road, Color color){
	// Enum khong co gia tri am -> chi can kiem tra can tren
	if (road >= NUM_OF_ROADS) return;
	show_only(road, color, 1);		// color ngoai RED/AMBER/GREEN -> tat ca 3 mau
}

void traffic_blink_reset(void){
	blink_on = 0;					// lan toggle ke tiep chac chan BAT den
}

void traffic_toggle_color(Color color){
	if (color >= NUM_OF_COLORS) return;

	// Dung 1 bien chung thay vi HAL_GPIO_TogglePin tung chan:
	// neu truoc do 2 huong dang khac mau (vd Mode 1: do / xanh)
	// thi toggle rieng tung chan se lam 2 huong nhay lech pha nhau.
	blink_on = !blink_on;

	for (Road r = 0; r < NUM_OF_ROADS; r++){
		show_only(r, color, blink_on);
	}
}
