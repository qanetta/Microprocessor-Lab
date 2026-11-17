/*
 * watchdog.c
 *
 *  Ghi thang thanh ghi IWDG (RM0008 muc 19), vi project chua bat IWDG trong .ioc
 *  (khong co driver stm32f1xx_hal_iwdg.c).
 *    KR  : 0xCCCC = bat dau dem, 0x5555 = mo khoa PR/RLR, 0xAAAA = nap lai (refresh)
 *    PR  : bo chia LSI (3 -> chia 32)
 *    RLR : gia tri nap lai 12 bit, bo dem dem xuong tu RLR, ve 0 thi reset chip
 *    SR  : khac 0 khi PR/RLR dang duoc cap nhat
 */
#include "watchdog.h"
#include "main.h"

// Ghi trong ISR (tang), ghi trong while (xoa) -> volatile
static volatile uint32_t counter_for_watchdog = 0;

// Timeout = 32 * 1250 / 40 kHz = 1 s (LSI thuc te 30-60 kHz -> 0.67-1.33 s)
void Watchdog_Init(void){
	IWDG->KR  = 0xCCCC;			// bat IWDG (phan cung tu bat LSI). Da bat thi chi reset moi tat duoc
	IWDG->KR  = 0x5555;			// cho phep ghi PR, RLR
	IWDG->PR  = 3;				// chia 32
	IWDG->RLR = 1250;
	while (IWDG->SR){}			// cho PR, RLR cap nhat xong
	IWDG->KR  = 0xAAAA;			// nap gia tri moi vao bo dem
	counter_for_watchdog = 0;
}

void Watchdog_Refresh(void){
	IWDG->KR = 0xAAAA;
}

uint8_t Is_Watchdog_Reset(void){
	return counter_for_watchdog > WDG_MAX_MISSED_TICKS;
}

void Watchdog_Counting(void){
	counter_for_watchdog++;
}

void Reset_Watchdog_Counting(void){
	counter_for_watchdog = 0;
}
