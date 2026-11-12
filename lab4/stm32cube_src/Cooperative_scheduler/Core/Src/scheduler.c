/*
 * scheduler.c
 *
 *  Created on: Nov 11, 2026
 *      Author: ACER
 *
 *  Cach (a): ISR tru Delay cua task dau danh sach.
 *  - Task dau ve 0 -> bat RunMe. Neu RunMe da bat ma dispatcher chua kip chay
 *    (task truoc chay qua 1 tick) thi ISR ghi lai vao SCH_lost_ticks de bu sau,
 *    khong tang RunMe them (tranh chay 1 task nhieu lan, tranh troi gio cac task sau).
 *  - Moi thao tac sua mang o vong while deu nam trong vung gang (tat ngat ngan).
 */
#include "scheduler.h"
#include "main.h"
#include <stddef.h>

static sTask SCH_Tasks[SCH_MAX_TASKS];
static uint8_t SCH_Count;
static uint32_t SCH_next_id;

static volatile uint32_t SCH_ticks;			// tong so tick tu luc khoi dong (khong reset trong SCH_Init)
static volatile uint32_t SCH_lost_ticks;	// so tick troi qua khi task dau da den han ma chua duoc chay

// Task dang chay trong SCH_Dispatch_Tasks: neu no tu xoa chinh minh thi khong them lai
static uint32_t SCH_running_id = NO_TASK_ID;
static uint8_t SCH_running_deleted;

/* ---------------- Vung gang ----------------
 * Luu PRIMASK roi tat ngat; khoi phuc lai gia tri cu (goi long nhau van dung).
 * __disable_irq() cua CMSIS co memory clobber -> compiler khong cache mang qua vung gang. */
static uint32_t sch_lock(void){
	uint32_t primask = __get_PRIMASK();
	__disable_irq();
	return primask;
}

static void sch_unlock(uint32_t primask){
	__set_PRIMASK(primask);
}

/* ---------------- Ham noi bo (goi khi da khoa ngat) ---------------- */

// Task dau co Delay = 0 la da den han -> bat RunMe ngay, khong doi tick sau
static void sch_mark_head(void){
	if (SCH_Count > 0 && SCH_Tasks[0].Delay == 0) SCH_Tasks[0].RunMe = 1;
}

// Chen task vao dung vi tri theo thoi diem den han
static uint32_t sch_insert(void (*pTask)(void), uint32_t delay, uint32_t period, uint32_t id){
	if (SCH_Count >= SCH_MAX_TASKS) return NO_TASK_ID;

	// Di qua cac task den han truoc hoac CUNG LUC task moi, tru dan delay.
	// Dung >= de task moi dung SAU cac task cung han -> chay theo thu tu them (FIFO)
	uint8_t i = 0;
	while (i < SCH_Count && delay >= SCH_Tasks[i].Delay){
		delay -= SCH_Tasks[i].Delay;
		i++;
	}

	// Chi task dung ngay sau bi sua: Delay cua no gio tinh tu task moi
	if (i < SCH_Count) SCH_Tasks[i].Delay -= delay;

	// Dich lui tu cuoi ve vi tri i (dich tu dau se ghi de du lieu)
	for (uint8_t k = SCH_Count; k > i; k--) SCH_Tasks[k] = SCH_Tasks[k - 1];

	SCH_Tasks[i].pTask = pTask;
	SCH_Tasks[i].Delay = delay;
	SCH_Tasks[i].Period = period;
	SCH_Tasks[i].RunMe = 0;
	SCH_Tasks[i].TaskID = id;
	SCH_Count++;

	sch_mark_head();
	return id;
}

// Xoa task o vi tri i
static void sch_remove_at(uint8_t i){
	// Thoi gian cua task bi xoa cong sang task ke tiep -> cac task phia sau giu dung moc
	if (i + 1 < SCH_Count) SCH_Tasks[i + 1].Delay += SCH_Tasks[i].Delay;

	for (uint8_t k = i; k + 1 < SCH_Count; k++) SCH_Tasks[k] = SCH_Tasks[k + 1];
	SCH_Count--;

	SCH_Tasks[SCH_Count].pTask = NULL;
	SCH_Tasks[SCH_Count].TaskID = NO_TASK_ID;
	sch_mark_head();
}

static uint32_t sch_new_id(void){
	if (++SCH_next_id == NO_TASK_ID) SCH_next_id = 1;	// tran 32 bit thi bo qua 0
	return SCH_next_id;
}

/* ---------------- Public ---------------- */

void SCH_Init(void){
	for(uint8_t i = 0; i < SCH_MAX_TASKS; i++){
		SCH_Tasks[i].pTask = NULL; // =0 co duoc khong nhi
		SCH_Tasks[i].Delay = 0;
		SCH_Tasks[i].Period = 0;
		SCH_Tasks[i].RunMe = 0;
		SCH_Tasks[i].TaskID = NO_TASK_ID;
	}

	SCH_Count = 0;
	SCH_next_id = 0;
	SCH_lost_ticks = 0;
}

uint32_t SCH_Add_Task(void (*pTask)(void), uint32_t Delay, uint32_t Period){
	if (pTask == NULL) return NO_TASK_ID;

	uint32_t pm = sch_lock();
	uint32_t id = (SCH_Count < SCH_MAX_TASKS) ? sch_insert(pTask, Delay, Period, sch_new_id()) : NO_TASK_ID;
	sch_unlock(pm);
	return id;
}

uint8_t SCH_Delete_Task(uint32_t TaskID){
	if (TaskID == NO_TASK_ID) return 0;

	// Task dang chay da duoc lay ra khoi mang -> chi danh dau de dispatcher khong them lai
	if (TaskID == SCH_running_id){
		SCH_running_deleted = 1;
		return 1;
	}

	uint32_t pm = sch_lock();
	for (uint8_t i = 0; i < SCH_Count; i++){
		if (SCH_Tasks[i].TaskID == TaskID){
			sch_remove_at(i);
			sch_unlock(pm);
			return 1;
		}
	}
	sch_unlock(pm);
	return 0;
}

// Trong ngat TIM2: O(1), chi xet task dau danh sach
void SCH_Update(void){
	SCH_ticks++;
	if (SCH_Count == 0) return;

	// Task dau da den han nhung vong while chua kip chay: ghi lai tick nay de bu
	if (SCH_Tasks[0].RunMe){
		SCH_lost_ticks++;
		return;
	}
	if (SCH_Tasks[0].Delay > 0) SCH_Tasks[0].Delay--;
	if (SCH_Tasks[0].Delay == 0) SCH_Tasks[0].RunMe = 1;
}

void SCH_Dispatch_Tasks(void){
	for (;;){
		uint32_t pm = sch_lock();

		// 1. Bu cac tick bi tre cho task dau (neu task dau chua den han)
		if (SCH_Count > 0 && !SCH_Tasks[0].RunMe && SCH_lost_ticks > 0){
			SCH_lost_ticks--;
			if (SCH_Tasks[0].Delay > 0) SCH_Tasks[0].Delay--;
			sch_mark_head();
			sch_unlock(pm);
			continue;
		}

		// 2. Khong con task den han -> thoat
		if (SCH_Count == 0 || !SCH_Tasks[0].RunMe){
			if (SCH_Count == 0) SCH_lost_ticks = 0;
			sch_unlock(pm);
			return;
		}

		// 3. Lay task dau ra khoi mang. Task ke tiep co Delay = 0 (cung han) se duoc bat RunMe
		sTask t = SCH_Tasks[0];
		sch_remove_at(0);
		SCH_running_id = t.TaskID;
		SCH_running_deleted = 0;
		uint32_t ticks_before = SCH_ticks;
		uint32_t lost_before = SCH_lost_ticks;
		sch_unlock(pm);

		// 4. Chay task NGOAI vung gang: ngat TIM2 van hoat dong trong luc task chay
		t.pTask();

		// 5. Task tuan hoan: chen lai, giu nguyen TaskID. One-shot (Period = 0): khong chen -> tu xoa
		if (t.Period > 0 && !SCH_running_deleted){
			pm = sch_lock();
			// Cac tick troi qua trong luc task chay da duoc tru vao danh sach -> tru bot
			// cho task nay de lan chay sau van dung moc Period (khong troi gio)
			uint32_t applied = (SCH_ticks - ticks_before) - (SCH_lost_ticks - lost_before);
			uint32_t delay = (t.Period > applied) ? t.Period - applied : 0;
			sch_insert(t.pTask, delay, t.Period, t.TaskID);
			sch_unlock(pm);
		}
		SCH_running_id = NO_TASK_ID;
	}
}

uint32_t SCH_Get_Time(void){
	return SCH_ticks * SCH_TICK_MS;		// doc 32 bit da can le tren Cortex-M3 la nguyen tu
}
