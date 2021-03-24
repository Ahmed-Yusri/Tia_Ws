/*
 * Os_Application.h
 *
 *  Created on: Aug 26, 2020
 *      Author: Ahmed Yusri Mohamed
 */

#ifndef ORTOS_OS_APPLICATION_H_
#define ORTOS_OS_APPLICATION_H_

#include "OS.h"


extern void MCU_Init(void);

DeclareTask(OS_LED_TASK);
DeclareTask(OS_STATE_PATTERN_TASK);
DeclareTask(OS_IDLE_TASK_ID);

#endif /* ORTOS_OS_APPLICATION_H_ */
