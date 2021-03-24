/*
 * OS.h
 *
 *  Created on: Aug 23, 2020
 *      Author: Ahmed Yusri Mohamed Ezzat
 *      File Revision:
 *                     Aug 23, 2020   Ahmed Yusri  v0.1 Initial Version
 */

#ifndef ORTOS_OS_H_
#define ORTOS_OS_H_

#include "Std_Types.h"
#include "OS_Cfg.h"

/*typedef uint8_t StatusType;*/
#ifndef E_OK
#define E_OK                      ((uint8_t)0x00)
#endif
#define E_OS_ACCESS               ((uint8_t)0x01)
#define E_OS_CALLEVEL             ((uint8_t)0x02)
#define E_OS_ID                   ((uint8_t)0x03)
#define E_OS_LIMIT                ((uint8_t)0x04)
#define E_OS_NOFUNC               ((uint8_t)0x05) 
#define E_OS_RESOURCE             ((uint8_t)0x06) 
#define E_OS_STATE                ((uint8_t)0x07)
#define E_OS_VALUE                ((uint8_t)0x08)
/*Internal Status.*/
#define E_OS_SYS_STACK            ((uint8_t)0x09)
#define E_OS_SYS_NOT_STARTED      ((uint8_t)0x0A)


/*typedef uint8_t TaskStateType;*/
#define OS_TASK_RUNNING           ((uint8_t)0xAA)
#define OS_TASK_WAITING           ((uint8_t)0xAB)
#define OS_TASK_READY             ((uint8_t)0xAC)
#define OS_TASK_SUSPENDED         ((uint8_t)0xAD)
#define OS_TASK_INVALID           ((uint8_t)0xAE)

/***************************************************************************
 * DeclareTask serves as an external declaration of a task. 
 * The function and use of this service are similar to that of the 
 * external declaration of variables
 ***************************************************************************/
#define DeclareTask(TASK_ID)        extern void (TASK_ID##_Thread)(void)
#define OS_TASK(TASK_ID)            void (TASK_ID##_Thread)(void)
#define OS_TASK_ADDRESS(TASK_ID)    &(TASK_ID##_Thread)

typedef uint8_t TaskType;
typedef uint8_t TaskStateType;
typedef uint8_t AppModeType;


typedef uint32_t TickType;
typedef TickType TickRefType;

/*************************************************************************
 *        <StartOS>
 * In : AppModeType
 * Out: None
 * return: None
 * description: The user can call this system service to 
 *              start the operating system in a specific mode.
 *              This call does not need to return.
 ***************************************************************************/
void StartOS(AppModeType mode);

/*************************************************************************
 *        <Os_EnterIdleState>
 * In : None
 * Out: None
 * return: None
 * description: Triggered by the Idle task.
 ***************************************************************************/
void Os_EnterIdleState(void);

/*************************************************************************************************
 *                                           <Os_Dispatcher>
 * In : None
 * Out: None
 * return: None
 * description: If a higher-priority task is ready, the internal state will be prepared to 
 *              perform the context switch or the task state will be changed and linked to a task
 *              parent.
 * 
 * Usage: This function will be triggered as a sys Os interrupt from the application level (SetEvent)
 *        or from the Os tick timer. 
 *          (The context switch will take place at the end of this function if needed). 
 ************************************************************************************************/
void Os_Dispatcher(void) ;

/*************************************************************************************************
 *                                        <ChainTask>
 * In : TaskType
 * Out: None
 * return: None
 * description: This service causes the termination of the calling task. 
 *              After termination of the calling task a succeeding task <TaskID> is activated. 
 *              Using this service, it ensures that the succeeding task 
 *              starts to run at the earliest after the calling task has been terminated.
 * 
 * Usage:  A task can make this call to terminate the currently running task
 *         and to activate the task indicated.
 ************************************************************************************************/
StatusType ChainTask(TaskType TaskId) ;

/*************************************************************************************************
 *                                           <Os_SysTickTimer>
 * In : None
 * Out: None
 * return: None
 * description: The Os main timer ISR used to handle the timing events.
 ************************************************************************************************/
void Os_SysTickTimer(void);

/*************************************************************************************************
 *                                           <Os_SVC_Handler>
 * In : None
 * Out: None
 * return: None
 * description: The Os Supervisor call. 'SysCall'.
 ************************************************************************************************/
void Os_SVC_Handler(void);

/*************************************************************************************************
 *                                           <Os_WaitEvent>
 * In : event mask
 * Out: None
 * return: None
 * description: Wait on events.
 ************************************************************************************************/
void Os_WaitEvent(EventMaskType event) ;

/*************************************************************************************************
 *                                           <Os_GetEvent>
 * In : taskId, event mask
 * Out: None
 * return: None
 * description: get the events state.
 ************************************************************************************************/
void Os_GetEvent(EventMaskRefType event) ;

/*************************************************************************************************
 *                                           <Os_ClearEvent>
 * In : taskId, event mask
 * Out: None
 * return: None
 * description: Clear the events state.
 ************************************************************************************************/
void Os_ClearEvent(EventMaskType event) ;


/*************************************************************************************************
 *                                           <Os_SetEvent>
 * Parameter (In):
                    taskId    : Reference to the task for which one or several events are to be set.
                    eventMask : Mask of the events to be set
 * Out: None
 * return: StatusType
 * 
 * description: 
 * 
 *      - The service may be called from an interrupt service routine and from the task level,
 *         but not from hook routines. 
 * 
 *      - The events of task <taskId> are set according to the event mask <eventMask>. 
 * 
 *      - Calling SetEvent causes the task <taskId> to be transferred to the ready state, 
 *        if it was waiting for at least one of the events specified in <eventMask>.
 ************************************************************************************************/
StatusType Os_SetEvent(TaskType taskId, EventMaskType eventMask);

/*************************************************************************************************
 *                                           <DisableAllInterrupts>
 * In : None
 * Out: None
 * return: None
 * description: This service disables all interrupts for which the hardware supports disabling.
 *              Note that this service does not support nesting.
 ************************************************************************************************/
void DisableAllInterrupts(void);

/*************************************************************************************************
 *                                           <DisableAllInterrupts>
 * In : None
 * Out: None
 * return: None
 * description: This service disables all interrupts for which the hardware supports disabling.
 *              This service is intended to start a critical section of the code.
 *              This section shall be finished by calling the EnableAllInterrupts service.
 *              No API service calls are allowed within this critical section.
 *              Note that this service does not support nesting.
 ************************************************************************************************/
void DisableAllInterrupts(void);

/*************************************************************************************************
 *                                           <EnableAllInterrupts>
 * In : None
 * Out: None
 * return: None
 * description: This service restores the state saved by DisableAllInterrupts.
 *              This service is a counterpart of DisableAllInterrupts service,
 *              which has to be called before, and its aim is the completion of the critical section of code.
 *           ** No API service calls are allowed within this critical section.
 ************************************************************************************************/
void EnableAllInterrupts(void) ;

/*************************************************************************************************
 *                                           <SuspendAllInterrupts>
 * Parameter (In): none
 * Parameter (Out): none
 * Description: This service saves the recognition status of all interrupts and disables all interrupts
 *              for which the hardware supports disabling.
 * Particularities: This service is intended to protect a critical section of code from interruptions of any kind.
 *                  This section shall be finished by calling the ResumeAllInterrupts service.
 *            *** No API service calls beside SuspendAllInterrupts/ResumeAllInterrupts pairs
 ************************************************************************************************/
void SuspendAllInterrupts(void);

/*************************************************************************************************
 *                                           <ResumeAllInterrupts>
 * In : None
 * Out: None
 * return: None
 * description: This service restores the recognition status of all interrupts
 *              saved by the SuspendAllInterrupts service.
 *              Note that this service does not support nesting.
 ************************************************************************************************/
void ResumeAllInterrupts(void);

/*************************************************************************************************
 *                                           <ResumeOSInterrupts>
 * In : None
 * Out: None
 * return: None
 * description: This service restores the recognition status of interrupts saved by
 *              the SuspendOSInterrupts service.
 *              This service is the counterpart of SuspendOSInterrupts service,
 *              which has to have been called before, and its aim is the completion of the critical section of code.
 *          *** No API service calls beside SuspendAllInterrupts/ResumeAllInterrupts pairs
 ************************************************************************************************/
void ResumeOSInterrupts(void) ;

/*************************************************************************************************
 *                                           <SuspendOSInterrupts>
 * In : None
 * Out: None
 * return: None
 * description: This service saves the recognition status of interrupts of category 2 and
 *              disables the recognition of these interrupts.
 *              This service is intended to protect a critical section of code.
 *              This section shall be finished by calling the ResumeOSInterrupts service.
 *          *** No API service calls beside SuspendAllInterrupts/ResumeAllInterrupts pairs
 ************************************************************************************************/
void SuspendOSInterrupts(void) ;

#endif /* ORTOS_OS_H_ */
