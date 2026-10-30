/*
 * software_timer.c
 *
 *  Created on: Oct 28, 2026
 *      Author: ACER
 */

#include "software_timer.h"

static volatile int timer_flag[NUM_OF_TIMERS];
static volatile int timer_counter[NUM_OF_TIMERS];

void setTimer(int index, int duration){
	if (index < 0 || index >= NUM_OF_TIMERS) return;
	// lam tron len: luon chay it nhat 1 chu ky ngat
	timer_counter[index] = (duration + TIMER_CYCLE - 1) / TIMER_CYCLE;
	timer_flag[index] = 0;   // xoa co SAU khi nap counter
}

int isTimerExpired(int index){
	if (index < 0 || index >= NUM_OF_TIMERS) return 0;
	return timer_flag[index];
}

void timerRun(void){
	for (int i = 0; i < NUM_OF_TIMERS; i++){
		if (timer_counter[i] > 0){
			timer_counter[i]--;
			if (timer_counter[i] == 0){
				timer_flag[i] = 1;
			}
		}
	}
}
