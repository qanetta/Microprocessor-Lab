/*
 * fsm_manual.h
 *
 *  Created on: Oct 28, 2026
 *      Author: ACER
 */

#ifndef INC_FSM_MANUAL_H_
#define INC_FSM_MANUAL_H_

void fsm_manual_start(void);	// vao Mode 2 (MAN_RED), fsm_automatic goi khi nhan BTN_MODE
void fsm_manual_run(void);		// Mode 2-4: MAN_RED/AMBER/GREEN + timeout 10s, goi trong while(1)

#endif /* INC_FSM_MANUAL_H_ */
