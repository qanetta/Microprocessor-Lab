/*
 * software_timer.h
 *
 *  Created on: Sep 30, 2026
 *      Author: ACER
 */

#ifndef SOFTWARE_TIMER_H_
#define SOFTWARE_TIMER_H_

extern volatile int timer1_flag;
extern volatile int timer2_flag;

void setTimer1(int duration);
void setTimer2(int duration);
void timerRun();
#endif /* SOFTWARE_TIMER_H_ */
