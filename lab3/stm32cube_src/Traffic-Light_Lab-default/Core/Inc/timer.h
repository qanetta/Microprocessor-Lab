/*
 * timer.h
 *
 *  Created on: Oct 31, 2026
 *      Author: ACER
 */

#ifndef INC_TIMER_H_
#define INC_TIMER_H_

// Ex4: chu ky ngat TIM2 (ms). Doi chu ky ngat (1ms, 10ms, 100ms...) chi can sua
// dong nay cho khop voi Prescaler/Period cua TIM2, moi thoi gian ben duoi tinh bang ms
// nen khong bi anh huong (vd den van nhay dung 2Hz).
#define TIMER_CYCLE		10

typedef enum{
	TIMER_SECOND = 0,	// nhip 1s dem nguoc o Mode 1
	TIMER_BLINK,		// nhay den 2Hz o Mode 2-4
	TIMER_SCAN,			// quet 4 LED 7 doan
	TIMER_TIMEOUT,		// 10s khong bam nut o Mode 2-4 -> ve Mode 1
	TIMER_AUTO_INC,		// tang tu dong moi 500ms khi giu nut 2
	TIMER_DEBUG,		// nhay LED debug 1Hz: biet chip con chay
	NUM_OF_TIMERS
} TimerId;

void setTimer(TimerId id, int duration_ms);	// duration_ms <= 0: tat timer
int  isTimerExpired(TimerId id);				// 1 neu het gio (giu nguyen den khi setTimer lai)
void timer_run(void);							// goi trong ngat TIM2

#endif /* INC_TIMER_H_ */
