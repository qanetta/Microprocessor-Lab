/*
 * software_timer.h
 *
 *  Created on: Oct 28, 2026
 *      Author: ACER
 */

#ifndef INC_SOFTWARE_TIMER_H_
#define INC_SOFTWARE_TIMER_H_

#define TIMER_CYCLE 10	// chu ky ngat TIM2 (ms) - Ex4: doi chu ky ngat chi sua o day

// Ten cac software timer, phan tu cuoi tu dong la so luong timer
typedef enum{
	TIMER_SECOND = 0,	// nhip 1s: dem nguoc thoi gian den o Mode 1
	TIMER_SCAN,			// quet 4 LED 7 doan
	TIMER_BLINK,		// nhay den 2Hz o Mode 2-4
	TIMER_TIMEOUT,		// 10s khong bam nut o Mode 2-4 -> ve Mode 1
	NUM_OF_TIMERS
} Timer_ID;

void setTimer(int index, int duration);	// duration tinh bang ms
int isTimerExpired(int index);				// tra ve 1 neu het thoi gian
void timerRun(void);						// goi trong ISR moi TIMER_CYCLE ms

#endif /* INC_SOFTWARE_TIMER_H_ */
