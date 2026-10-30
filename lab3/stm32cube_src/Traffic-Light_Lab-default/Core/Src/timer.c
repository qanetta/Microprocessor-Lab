/*
 * timer.c
 *
 *  Created on: Oct 31, 2026
 *      Author: ACER
 *
 *  Software timer dua tren ngat TIM2 (moi TIMER_CYCLE ms).
 *  Ngat chi lam viec ngan: doc nut + dem timer. Moi logic nam o vong while(1).
 */
#include "main.h"
#include "timer.h"
#include "input_reading.h"

// volatile: ISR ghi, vong while(1) doc
static volatile int timer_counter[NUM_OF_TIMERS];
static volatile int timer_flag[NUM_OF_TIMERS];

void setTimer(TimerId id, int duration_ms){
	if (id >= NUM_OF_TIMERS) return;
	if (duration_ms <= 0){				// tat timer
		timer_counter[id] = 0;
		timer_flag[id] = 0;
		return;
	}
	// lam tron len: luon chay it nhat 1 chu ky ngat
	timer_counter[id] = (duration_ms + TIMER_CYCLE - 1) / TIMER_CYCLE;
	timer_flag[id] = 0;					// xoa co SAU khi nap counter
}

int isTimerExpired(TimerId id){
	if (id >= NUM_OF_TIMERS) return 0;
	return timer_flag[id];
}

void timer_run(void){
	for (int i = 0; i < NUM_OF_TIMERS; i++){
		if (timer_counter[i] > 0){
			timer_counter[i]--;
			if (timer_counter[i] == 0){
				timer_flag[i] = 1;
			}
		}
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	if (htim->Instance == TIM2){
		button_reading();
		timer_run();
	}
}
