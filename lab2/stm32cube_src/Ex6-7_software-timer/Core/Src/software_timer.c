#include "software_timer.h"

#define TIMER_CYCLE 10     // TIM2 ngat moi 10ms

// Timer 0: nhay LED_RED (bai tap 6)
volatile int timer0_counter = 0;
volatile int timer0_flag = 0;

// Timer 1: dem giay cho dong ho va nhay DOT (bai tap 7)
volatile int timer1_counter = 0;
volatile int timer1_flag = 0;

void setTimer0(int duration){
    timer0_counter = duration / TIMER_CYCLE;
    timer0_flag = 0;
}

void setTimer1(int duration){
    timer1_counter = duration / TIMER_CYCLE;
    timer1_flag = 0;
}

// Goi trong ngat timer moi 10ms
void timer_run(){
    if(timer0_counter > 0){
        timer0_counter--;
        if(timer0_counter == 0) timer0_flag = 1;
    }
    if(timer1_counter > 0){
        timer1_counter--;
        if(timer1_counter == 0) timer1_flag = 1;
    }
}
