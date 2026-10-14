#ifndef INC_SOFTWARE_TIMER_H_
#define INC_SOFTWARE_TIMER_H_

extern volatile int timer0_flag;
extern volatile int timer1_flag;
extern volatile int timer2_flag;
extern volatile int timer3_flag;
extern volatile int timer4_flag;

void setTimer0(int duration);
void setTimer1(int duration);
void setTimer2(int duration);
void setTimer3(int duration);
void setTimer4(int duration);
void timer_run(void);

#endif /* INC_SOFTWARE_TIMER_H_ */
