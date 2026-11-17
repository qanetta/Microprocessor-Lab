/*
 * watchdog.h
 *
 *  IWDG (Independent Watchdog) cho cooperative scheduler - muc 2.3.10 cua de.
 *  - IWDG chay bang LSI (~40 kHz) rieng, doc lap voi clock chinh -> CPU treo van reset duoc chip.
 *  - Refresh trong SCH_Update (ngat TIM2) NHUNG chi khi vong while con song:
 *      ISR      : Watchdog_Counting() moi tick, refresh neu chua qua WDG_MAX_MISSED_TICKS
 *      while(1) : SCH_Dispatch_Tasks() goi Reset_Watchdog_Counting() truoc moi task
 *    -> task treo: bo dem tang mai -> ISR ngung refresh -> khoang 1 s sau IWDG reset chip
 */

#ifndef INC_WATCHDOG_H_
#define INC_WATCHDOG_H_
#include <stdint.h>

#define WDG_MAX_MISSED_TICKS	10		// vong while khong quay lai dispatcher qua 10 tick (100 ms) -> coi la treo

void Watchdog_Init(void);				// bat IWDG (timeout ~1 s), goi ngay truoc khi bat TIM2
void Watchdog_Refresh(void);			// nap lai bo dem IWDG
uint8_t Is_Watchdog_Reset(void);		// 1 = vong while da treo qua WDG_MAX_MISSED_TICKS -> de IWDG reset chip
void Watchdog_Counting(void);			// goi trong SCH_Update moi tick
void Reset_Watchdog_Counting(void);		// goi trong SCH_Dispatch_Tasks (vong while)

#endif /* INC_WATCHDOG_H_ */
