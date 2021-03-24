/*
 * OS.c
 *
 *  Created on: Aug 23, 2020
 *      Author: Ahmed Yusri
 */

#define OS_SRC_FILE

#include "OS.h"
#include "Os_Hal.h"
#include "Os_TaskQueue.h"
#include "Os_Events.h"
#include "Os_PrivMacro.h"
#include "Os_Scheduler.h"

/*************************************************************************
 * Os.Global Variables.
 *************************************************************************/

 /*The following variables are shared with the OS Dispatcher ASM.*/
volatile Os_DynTaskInfoType Os_GlobalTaskCtxInfo =\
{
   .curTaskCfgBlockPtr = (Os_TaskCfgBlockRefType)NULL_PTR,   /*STARTUP CODE.*/
   .newTaskCfgBlockPtr = (Os_TaskCfgBlockRefType)&Os_GlobalTaskCtxInfo.dynCtxInfo[OS_CFG_NUMBER_OF_OS_TASKS - 1],   /*Idle Task.*/
   .hooks =  NULL_PTR,
   .sysTick = 0x0
};

/*************************************************************************
 *  OS.Local Functions
 *************************************************************************/

LOCAL_INLINE void Os_UpdateTimingEvents(void)
{
   uint8_t time_event_idx = OS_CFG_NUMBER_OF_TIMING_EVENTS;
   uint32_t currCnt = OS_GET_SYSTICK_COUNTER();
   
   while(time_event_idx--) /*Loop over all the timing events and updat the corresponding task/s*/
   {
      if(!(currCnt % OS_GET_TIMING_EVENT_TIME(time_event_idx)))   /*If we hit this pEvent.*/
      {
         Os_UpdateScheduleTbl(OS_GET_TIMING_EVENT_BIT_NUM(time_event_idx)); /* Update the Os schedule table and 
                                                                               mark the corresponding task/s as ready.*/
      }
   }
}/*Os_UpdateTimingEvents*/

static void Os_AssertDevError()
{
   /* TODO: Logging mechanisim.*/
   OS_HAL_DISABLE_ALL_INTERRUPTS();
   while(1);
}/*Os_AssertDevError*/

LOCAL_INLINE void Os_TaskConstruct(TaskType Task_Id)
{
   OsTaskRefType task = (OsTaskRefType)Os_GetTaskbyId(Task_Id);
   Os_SPType tempSp = task->sp;

   OS_SET_TASK_DYN_ID(Task_Id,  /*Set the dynamic task Id to the orginal task Id.*/
                      Task_Id);

   OS_SET_TASK_SP(Task_Id,
                  task->sp);

   OS_SET_TASK_STATE(Task_Id,  /*Change the task state..*/
                     OS_TASK_WAITING);

   OS_SET_TASK_SP(Task_Id, /*Call the task constructor and update the stack pointer accordingly.*/
                  Os_Hal_TaskStackConstruct(OS_GET_TASK_SP(Task_Id),
                                            task->stack_size,
                                            Os_GetThreadbyTask(task),
                                            Os_AssertDevError));

   do /*Fill the remaining stack area..*/
   {
      *tempSp = OS_CFG_STACK_PATTERN;
      ++tempSp;
   }while(tempSp < OS_GET_TASK_SP(Task_Id));

}/*Os_TaskConstruct*/

LOCAL_INLINE void Os_InitMemory(void)
{
   uint8_t taskId = OS_CFG_NUMBER_OF_OS_TASKS;
   Os_GlobalTaskCtxInfo.hooks = &OsHooks_Cfg;

   while(taskId--) /*Init task stack memory..*/
   {
      Os_TaskConstruct(taskId);
   }

   Os_Queue_Construct(&Os_Queue); /*Init the Os Queue..*/
}/*Os_InitMemory*/

/*************************************************************************
 *  OS.Global Functions
 *************************************************************************/
/*************************************************************************
 *        <StartOS>
 * In : AppModeType
 * Out: None
 * return: None
 * description: The user can call this system service to 
 *              start the operating system in a specific mode.
 *              This call does not need to return.
 ***************************************************************************/
void StartOS(AppModeType mode)
{
   (void)mode; /*Unused var.. it may be used like a post-build configuration which will change the behavior of the system based on a Hw configuraton pins. 
                  modes could be -> service center mode / normal mode / data logging mode 'debugging'.. */
   
   Os_InitMemory();  /*Init the Os/Task memory..*/
   
   Os_GlobalTaskCtxInfo.hooks->Startup();   /*Call application hook init..*/
   
   Os_Hal_Init();  /*Init the required hw resource for the Os..*/

   Os_Hal_SysTick_Init(OS_CFG_SYS_TICK_IN_MS); /*Init the OS SysTick timer.*/

   Os_EnqueueTask(&Os_Queue,  /*Insert the requested task into the Os queue.*/
                  OS_IDLE_TASK_ID,
                  OS_GET_TASK_PRIORITY(OS_IDLE_TASK_ID));

   Os_Hal_StartOs();   /*This call shall not be returned*/

   Os_AssertDevError();
}/*StartOS*/

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
 * Usage:  A task can make this call to terminate the running task
 *         and to activate the task indicated.
 ************************************************************************************************/
StatusType ChainTask(TaskType TaskId)
{
   Os_Hal_DisableOsInterrupts(); /*Enter Critical section..*/

   if(!OS_TASK_QUEUE_EMPTY(&Os_Queue)) /*The task shall be terminated..*/
   {
      OS_SET_TASK_STATE(Os_DequeueTask(&Os_Queue),  /*Remove the running task from the waiting list..*/
                        OS_TASK_SUSPENDED);
   }

   Os_EnqueueTask(&Os_Queue,  /*Insert the requested task into the Os queue.*/
                  TaskId,
                  OS_GET_TASK_PRIORITY(TaskId));

   Os_Hal_EnableOsInterrupts();

   Os_Hal_SysCall(); /*Trigger Os System interrupt to serve the pending request..*/
   
   return E_OS_LIMIT;
}/*ChainTask*/

/*************************************************************************************************
 *                                           <Os_SysTickTimer>
 * In : None
 * Out: None
 * return: None
 * description: The Os main timer ISR used to handle the timing events.
 ************************************************************************************************/
void Os_SysTickTimer(void)
{
   /* TODO: Move the handling of the timing events to a system task > to decrease the SysTick ISR eTime.*/
 
   OS_INCREMENT_TICK_COUNTER(OS_CFG_SYS_TICK_IN_MS); /*Increment SysTick Counter..*/

   Os_UpdateTimingEvents(); /* Upate the timing events if needed..*/ 

   Os_Hal_SysCall(); /*Trigger a Sys trap to dispatch a new task if needed. The SysTrap shall come after the sysTick ISR.*/
}/*Os_SysTickTimer*/

/*************************************************************************
 *        <Os_EnterIdleState>
 * In : None
 * Out: None
 * return: None
 * description: Triggered by the Idle task.
 ***************************************************************************/
void Os_EnterIdleState(void)
{
   Os_Hal_WaitOnInterrupt(); /*we can not use the wait on interrupt while accessing the EEPROM.*/
}/*Os_EnterIdleState*/

/*************************************************************************************************
 *                                           <Os_WaitEvent>
 * In : event mask
 * Out: None
 * return: None
 * description: Wait on events.
 ************************************************************************************************/
void Os_WaitEvent(EventMaskType event)
{
   EventMaskType active_events;

   Os_Hal_DisableOsInterrupts();

   active_events = Os_Event_GetTaskEvents(&Os_Events[0], 
                                          OS_GET_CURR_TASK_ID());
  
   if(!(active_events & event)) /*If there is no active events 'mask'..*/
   {
      TaskType taskId = Os_DequeueTask(&Os_Queue); /*Remove the current task from the Queue..*/
      
      OS_SET_TASK_STATE(taskId,   /*Change the task state > Waiting..*/
                        OS_TASK_WAITING);
                        
#if DEV_ERROR
      if(OS_GET_CURR_TASK_ID() == taskId) /*DEV ERROR CHECK..*/
#endif
      {
         Os_Hal_EnableOsInterrupts();

         Os_Hal_SysCall();
      }
#if DEV_ERROR
      else
      {
         Os_Hal_EnableOsInterrupts();
         Os_AssertDevError();
      }
#endif
   }
   else
   {
      Os_Hal_EnableOsInterrupts();
   }
}/*Os_WaitEvent*/

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
StatusType Os_SetEvent(TaskType taskId, EventMaskType eventMask)
{
   StatusType result = E_OK;

#if DEV_ERROR
   /**
    * To know if this is a correct event mask 
    *    we need to loop over-all the configured events for this task.
    **/
   if((OS_CFG_NUMBER_OF_OS_TASKS - 1) >= taskId) /*Check the task Id..*/
   {
      result = E_OS_ID;
   }
   else
#endif
   {
      Os_Hal_DisableOsInterrupts(); /* Enter a critical section > 
                                        make sure that the internal states will not changed during the following sequence.*/

      if(OS_TASK_SUSPENDED != OS_GET_TASK_STATE(taskId))  /*Check if the refrenced task is not in the suspend state
                                                             - if the refrenced task is suspended then:
                                                                 - it we will be eligible to use CPU time until it is resumed ..*/
      {
         Os_Event_SetEventMask(OS_GET_EVENTS_REF(), /*Set Event/s..*/ 
                               taskId,
                               eventMask);
          
         if(OS_TASK_WAITING == OS_GET_TASK_STATE(taskId)) /* If it is not waiting then it shall be (Running/Ready)-> 
                                                                  which in both cases will serve this event..*/
         {
            OS_SET_TASK_TO_READY(taskId); /*Make it ready..*/

            Os_EnqueueTask(OS_GET_SCHEDULER_QUEUE_REF(), /*Add the task to the queue..*/
                           taskId,
                           OS_GET_TASK_PRIORITY(taskId));

            Os_Hal_EnableOsInterrupts(); /*Exit from the critical section*/

            Os_Hal_SysCall(); /*Call the dispatcher to check if we need to dispatch the new ready task..*/
         }
         else
         {
            Os_Hal_EnableOsInterrupts(); /*Exit from the critical section*/
         }
      }
      else /*Referenced task <TaskID> is in the suspended state*/
      {
         Os_Hal_EnableOsInterrupts(); /*Exit from the critical section*/

         result = E_OS_STATE;
      }
   }

   return result;
}/*Os_SetEvent*/
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
void DisableAllInterrupts(void)
{
   /*TODO: prevents activation of all exceptions except for the Non-Maskable..*/
   OS_HAL_DISABLE_ALL_INTERRUPTS(); /*DISABLE ALL..*/
}/*DisableAllInterrupts*/

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
void EnableAllInterrupts(void)
{
   OS_HAL_ENABLE_ALL_INTERRUPTS(); /*Enable All*/
}/*EnableAllInterrupts*/

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
void SuspendAllInterrupts(void)
{
   /*TODO: This function shall be adapted to support nesting...*/
   OS_HAL_DISABLE_ALL_INTERRUPTS(); /*DISABLE ALL..*/
}/*SuspendAllInterrupts*/

/*************************************************************************************************
 *                                           <ResumeAllInterrupts>
 * In : None
 * Out: None
 * return: None
 * description: This service restores the recognition status of all interrupts
 *              saved by the SuspendAllInterrupts service.
 ************************************************************************************************/
void ResumeAllInterrupts(void)
{
   /*TODO: This function shall be adapted to support nesting...*/
   OS_HAL_ENABLE_ALL_INTERRUPTS(); /*Enable All*/
}/*ResumeAllInterrupts*/

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
void ResumeOSInterrupts(void)
{
   /*TODO: This function shall be adapted to support nesting...*/
   Os_Hal_EnableOsInterrupts();
}/*ResumeOSInterrupts*/

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
void SuspendOSInterrupts(void)
{
   /*TODO: This function shall be adapted to support nesting...*/
   Os_Hal_DisableOsInterrupts();
}/*SuspendOSInterrupts*/

/*************************************************************************************************
 *                                           <Os_GetEvent>
 * In : taskId, event mask
 * Out: None
 * return: None
 * description: get the events state.
 ************************************************************************************************/
void Os_GetEvent(EventMaskRefType event)
{
   Os_Hal_DisableOsInterrupts();

   *event = Os_Event_GetTaskEvents(&Os_Events[0],
                                   OS_GET_CURR_TASK_ID());
   
   Os_Hal_EnableOsInterrupts();
}/*Os_GetEvent*/

/*************************************************************************************************
 *                                           <Os_ClearEvent>
 * In : taskId, event mask
 * Out: None
 * return: None
 * description: Clear the events state.
 ************************************************************************************************/
void Os_ClearEvent(EventMaskType event)
{
   Os_Hal_DisableOsInterrupts();

   Os_Event_ClearTaskEvents(&Os_Events[0],
                            OS_GET_CURR_TASK_ID(),
                            event);

   Os_Hal_EnableOsInterrupts();
}/*Os_ClearEvent*/
