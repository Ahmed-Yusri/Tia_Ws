/*
 * OS_Cfg.h
 *
 *  Created on: Aug 25, 2020
 *      Author: Ahmed Yusri Mohammed
 */


#ifndef ORTOS_OS_CFG_H_
#define ORTOS_OS_CFG_H_

#include <stdint.h>

/*typedef uint8_t TaskType;*/
#define OS_LED_TASK                          ((uint8_t)0)
#define OS_STATE_PATTERN_TASK                ((uint8_t)1)
#define OS_IDLE_TASK_ID                      ((uint8_t)2)
#define OS_CFG_NUMBER_OF_OS_TASKS            ((uint8_t)0x3)

#define OS_CFG_STACK_PATTERN                 ((uint32_t)0xA55A5AA5)

#define OS_CFG_SYS_TICK_IN_MS                ((uint32_t)0x0A)

#define OS_CFG_MAX_EVENTS_PER_TASK           ((uint8_t)0x08)
#define OS_CFG_MAX_EVENTS_PER_TASK_P2        ((uint8_t)0x03)   /*Gen: Max Event number = 2Pow(OS_CFG_MAX_EVENTS_PER_TASK_P2)*/

/* 0: Waiting for an event.. 
 * 1: The event has been occurred.. */
#define OS_CFG_NUMBER_OF_TIMING_EVENTS                ((EventType)4)
/*OS Task0 Events..*/
#define OS_EVENT_STATE_PATTERN_TASK_P50MS_BIT_MASK    ((EventType)0x01)
/*OS Task0 Events..*/
#define OS_EVENT_RED_LED_P300MS_BIT_MASK              ((EventType)0x02)
/*OS Task1 Events..*/
#define OS_EVENT_GREAN_LED_P10MS_BIT_MASK             ((EventType)0x01)
/*OS Task1 Events..*/
#define OS_EVENT_GREAN_LED_P500MS_BIT_MASK            ((EventType)0x02)

#define OS_EVENT_OS_LED_TASK_UART_DATA_READY          ((EventType)0x04)

/*Os Queue Size.*/
#define OS_CFG_MAX_READY_TASK_QSIZE                   ((uint8_t)0x04)


typedef uint8_t TaskPrioType;
typedef void (*Os_ThreadRefType)(void);
typedef uint32_t *Os_SPType;   /*Stack pointer type.*/

typedef uint8_t EventType; /*Max of 256 Event per system, and 8 events per task, with max of 32 task..*/

typedef struct
{
   uint16_t ms;
   EventType eventId;
}Os_TimingEventType;

typedef struct
{
   const Os_SPType sp;
   const Os_ThreadRefType Thread;
   const uint32_t stack_size;
   const TaskPrioType prio;
}OsTaskType;

typedef OsTaskType *OsTaskRefType;
typedef uint8_t ProtectionReturnType;

typedef ProtectionReturnType (*ProtectionHookRefType)(StatusType);
typedef void (*StartupHookRefType)(void);
typedef void (*ErrorHookRefType)(StatusType);
typedef void (*ShutdownHookRefType)(StatusType);

typedef struct
{
   ProtectionHookRefType Protection;
   StartupHookRefType Startup;
   ErrorHookRefType Error;
   ShutdownHookRefType Shutdown;
}OsHooksType;

typedef const OsHooksType *OsHooksRefType;

typedef struct
{
   OsTaskType task; /*The current nodes attributes.. inherited from the OsTaskType object..*/
}OsTaskCfgType;

typedef OsTaskCfgType *OsTaskCfgRefType;

typedef uint8_t EventMaskType;
typedef EventMaskType *EventMaskRefType;

typedef EventMaskType EventMaskArrType[2]; /*Generated based on the number of events ((OS_CFG_NUMBER_OF_TIMING_EVENTS / 8) + 1)..*/

extern const OsTaskCfgType Os_TaskCfg[OS_CFG_NUMBER_OF_OS_TASKS];
extern const OsHooksType OsHooks_Cfg;
extern const Os_TimingEventType Os_Timing_Event[OS_CFG_NUMBER_OF_TIMING_EVENTS];

#endif /* ORTOS_OS_CFG_H_ */
