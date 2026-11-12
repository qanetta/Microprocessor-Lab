/*
 * scheduler.h
 *
 *  Created on: Nov 11, 2026
 *      Author: ACER
 *
 *  Cooperative scheduler, SCH_Update O(1) (delta list).
 *  - Mang task duoc sap xep theo thoi diem den han, task sap chay nhat o dau mang.
 *  - Delay luu TUONG DOI so voi task dung truoc: den han o tick 10, 25, 30 -> luu [10, 15, 5]
 *    -> SCH_Update (trong ngat) chi can xet task dau tien.
 *  - Don vi DELAY, PERIOD la TICK (1 tick = SCH_TICK_MS = chu ky ngat TIM2).
 */

#ifndef INC_SCHEDULER_H_
#define INC_SCHEDULER_H_
#include <stdint.h>

#define SCH_TICK_MS		10						// chu ky ngat TIM2 (PSC 7999, ARR 9, f_TIM 8 MHz)
#define SCH_MS(ms)		((ms) / SCH_TICK_MS)	// doi ms -> tick, vd SCH_MS(500) = 50
#define SCH_MAX_TASKS	16
#define NO_TASK_ID		0  // SCH_Add_Task tra ve gia tri nay khi mang day

typedef struct{
	void (*pTask) (void); // con tro ham thuc hien tac vu
	uint32_t Delay; // so tick con lai, TUONG DOI so voi task dung truoc (delta list)
	uint32_t Period; // so tick lap lai moi tac vu, 0 = one-shot
	uint8_t RunMe; // bat co danh dau tac vu can chay
	uint32_t TaskID; // ID duy nhat cua task, khong doi suot vong doi (khong phai vi tri trong mang)
} sTask;

void SCH_Init(void);							// goi truoc khi bat ngat TIM2
void SCH_Update(void);							// goi trong ngat TIM2, O(1)
void SCH_Dispatch_Tasks(void);					// goi trong while(1)
uint32_t SCH_Add_Task(void (*pTask)(void), uint32_t Delay, uint32_t Period);	// tra ve TaskID hoac NO_TASK_ID
uint8_t SCH_Delete_Task(uint32_t TaskID);		// 1 = da xoa, 0 = khong tim thay
uint32_t SCH_Get_Time(void);					// thoi gian tu luc khoi dong (ms), chinh xac den 1 tick

#endif /* INC_SCHEDULER_H_ */
