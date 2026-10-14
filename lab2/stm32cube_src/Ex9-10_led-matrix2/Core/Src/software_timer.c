#include "software_timer.h"

#define TIMER_CYCLE 5      // TIM2 ngat moi 5ms

// Timer 0: nhay LED_RED (bai tap 6)
volatile int timer0_counter = 0;
volatile int timer0_flag = 0;

// Timer 1: dem giay cho dong ho va nhay DOT (bai tap 7)
volatile int timer1_counter = 0;
volatile int timer1_flag = 0;

// Timer 2: quet LED 7 doan (bai tap 8)
volatile int timer2_counter = 0;
volatile int timer2_flag = 0;

// Timer 3: quet ma tran LED (bai tap 9)
volatile int timer3_counter = 0;
volatile int timer3_flag = 0;

// Timer 4: buoc hieu ung chu chay (bai tap 10)
volatile int timer4_counter = 0;
volatile int timer4_flag = 0;

void setTimer0(int duration){
    timer0_counter = duration / TIMER_CYCLE;
    timer0_flag = 0;
}

void setTimer1(int duration){
    timer1_counter = duration / TIMER_CYCLE;
    timer1_flag = 0;
}

void setTimer2(int duration){
    timer2_counter = duration / TIMER_CYCLE;
    timer2_flag = 0;
}

void setTimer3(int duration){
    timer3_counter = duration / TIMER_CYCLE;
    timer3_flag = 0;
}

void setTimer4(int duration){
    timer4_counter = duration / TIMER_CYCLE;
    timer4_flag = 0;
}

// Goi trong ngat timer moi TIMER_CYCLE ms
void timer_run(){
    if(timer0_counter > 0){
        timer0_counter--;
        if(timer0_counter == 0) timer0_flag = 1;
    }
    if(timer1_counter > 0){
        timer1_counter--;
        if(timer1_counter == 0) timer1_flag = 1;
    }
    if(timer2_counter > 0){
        timer2_counter--;
        if(timer2_counter == 0) timer2_flag = 1;
    }
    if(timer3_counter > 0){
        timer3_counter--;
        if(timer3_counter == 0) timer3_flag = 1;
    }
    if(timer4_counter > 0){
        timer4_counter--;
        if(timer4_counter == 0) timer4_flag = 1;
    }
}
