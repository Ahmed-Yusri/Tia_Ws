/*
 * Os_PrivMacro.h
 *
 *  Created on: Dec 1, 2020
 *      Author: Ahmed Yusri Moahmmed Ezzat.
 */

#ifdef OS_SRC_FILE
#ifndef TIA_RTOS_OS_PRIVMACRO_H_
#define TIA_RTOS_OS_PRIVMACRO_H_

#include "OS.h"

#define OS_TRUE                                    ((uint8_t)0xAA)
#define OS_FALSE                                   ((uint8_t)0x55)

#define OS_SET_CURR_TASK_ID(ID)                    Os_GlobalTaskCtxInfo.curTaskCfgBlockPtr->id = (ID)
#define OS_GET_CURR_TASK_ID()                      Os_GlobalTaskCtxInfo.curTaskCfgBlockPtr->id
#define OS_GET_NEXT_TASK_ID()                      OS_GET_TOP_TASK_ID()

#define OS_GET_TOP_TASK_ID()                       Os_GlobalTaskCtxInfo.newTaskCfgBlockPtr->id

#define OS_INCREMENT_TICK_COUNTER(SYSTICK)         Os_GlobalTaskCtxInfo.sysTick += (SYSTICK)
#define OS_GET_SYSTICK_COUNTER()                   Os_GlobalTaskCtxInfo.sysTick

#define OS_GET_TIMING_EVENT_TIME(IDX)              Os_Timing_Event[(IDX)].ms
#define OS_GET_TIMING_EVENT_BIT_NUM(IDX)           Os_Timing_Event[(IDX)].eventId

#define Os_GetThreadbyTask(task)                   (task)->Thread
#define Os_GetTaskbyId(TASK_ID)                    &Os_TaskCfg[(TASK_ID)].task
   
   
#define Os_GetTaskStartSpbyTaskId(TASK_ID)         Os_TaskCfg[(TASK_ID)].task.sp
#define OS_GET_TASK_SP(TASK_ID)                    Os_GlobalTaskCtxInfo.dynCtxInfo[(TASK_ID)].sp
#define OS_SET_TASK_SP(TASK_ID, NEW_SP)            Os_GlobalTaskCtxInfo.dynCtxInfo[(TASK_ID)].sp = (NEW_SP)
   
#define OS_SET_TASK_TO_READY(TASK_ID)              Os_GlobalTaskCtxInfo.dynCtxInfo[(TASK_ID)].state = OS_TASK_READY
#define OS_SET_TASK_STATE(TASK_ID, STATE)          Os_GlobalTaskCtxInfo.dynCtxInfo[(TASK_ID)].state = (STATE)
#define OS_GET_TASK_STATE(TASK_ID)                 Os_GlobalTaskCtxInfo.dynCtxInfo[(TASK_ID)].state

#define OS_SET_TASK_DYN_ID(TASK_ID, DYN_TASK_ID)   Os_GlobalTaskCtxInfo.dynCtxInfo[(TASK_ID)].id = (DYN_TASK_ID)

#define OS_GET_TASK_PRIORITY(TASK_ID)              Os_TaskCfg[(TASK_ID)].task.prio

#define OS_GET_EVENTS_REF()                        &Os_Events[0]
#define OS_GET_SCHEDULER_QUEUE_REF()               &Os_Queue
#define OS_GET_TOP_TASK_REF()                      &Os_GlobalTaskCtxInfo.newTaskCfgBlockPtr->id
#define OS_SET_NEXT_TASK_PTR(TASK_ID)              Os_GlobalTaskCtxInfo.newTaskCfgBlockPtr = (Os_TaskCfgBlockRefType)&Os_GlobalTaskCtxInfo.dynCtxInfo[(TASK_ID)]

typedef struct
{
   Os_SPType sp;
   TaskStateType state;
   TaskType id;
}Os_TaskCfgBlockType;

typedef Os_TaskCfgBlockType *Os_TaskCfgBlockRefType;

typedef struct
{
   Os_TaskCfgBlockRefType curTaskCfgBlockPtr;
   Os_TaskCfgBlockRefType newTaskCfgBlockPtr;
   OsHooksRefType hooks;
   Os_TaskCfgBlockType dynCtxInfo[OS_CFG_NUMBER_OF_OS_TASKS];
   TickType sysTick;
}Os_DynTaskInfoType;

volatile Os_DynTaskInfoType Os_GlobalTaskCtxInfo;

#endif /* TIA_RTOS_OS_PRIVMACRO_H_ */

#endif /* OS_SRC_FILE */
