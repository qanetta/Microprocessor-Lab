/*
 * traffic_light.h
 *
 *  Created on: Oct 28, 2026
 *      Author: ACER
 */

#ifndef INC_TRAFFIC_LIGHT_H_
#define INC_TRAFFIC_LIGHT_H_

#include "global.h"

// FSM chi goi cac ham duoi day, khong goi HAL_GPIO_WritePin truc tiep.
// Bang chan R1/Y1/G1, R2/Y2/G2 va muc sang (LED_ON) nam trong traffic_light.c

void traffic_all_off(void);					// tat ca 12 den
void traffic_set(Road road, Color color);	// huong road chi sang dung mau color

// Nhay o Mode 2-4 (2Hz): goi traffic_blink_reset() 1 lan khi vao trang thai MAN_*,
// sau do goi traffic_toggle_color() moi 250ms (250ms sang + 250ms tat = 2Hz)
void traffic_blink_reset(void);				// dat lai pha: lan toggle dau tien se BAT den
void traffic_toggle_color(Color color);		// dao mau color o ca 2 huong, cac mau khac tat

#endif /* INC_TRAFFIC_LIGHT_H_ */
