/*
 * software_timer.c
 *
 *  Created on: Sep 30, 2026
 *      Author: ACER
 */

#include "software_timer.h"

// Timer 1: Dành cho LED DOT (1s chớp 1 lần)
volatile int timer1_counter = 0;
volatile int timer1_flag = 0;

// Timer 2: Dành cho quét 4 LED 7 đoạn (500ms chuyển số)
volatile int timer2_counter = 0;
volatile int timer2_flag = 0;

void setTimer1(int duration) {
    timer1_counter = duration + 1;
    timer1_flag = 0;
}

void setTimer2(int duration) {
    timer2_counter = duration + 1;
    timer2_flag = 0;
}

// Gọi trong ngắt mỗi 10ms
void timerRun() {
    if (timer1_counter > 0) {
        timer1_counter--;
        if (timer1_counter <= 0) {
            timer1_flag = 1;
        }
    }
    if (timer2_counter > 0) {
        timer2_counter--;
        if (timer2_counter <= 0) {
            timer2_flag = 1;
        }
    }
}
