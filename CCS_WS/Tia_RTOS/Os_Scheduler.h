/*
 * Os_Scheduler.h
 *
 *  Created on: Sep 21, 2020
 *      Author: Ahmed Yusri Moahmed.
 */

#if  defined(OS_SRC_FILE)
#ifndef TIA_RTOS_OS_SCHEDULER_H_
#define TIA_RTOS_OS_SCHEDULER_H_


/*************************************************************************
 *  OS.Local Variables
 *************************************************************************/
/*This file will be applied only in the 
    Os Src code as per the above guard "OS_SRC_FILE"*/
static EventMaskArrType Os_Events;
static Os_TaskQueueType Os_Queue;

/*************************************************************************
 * Os.Local Schudler Functions.
 *************************************************************************/

/*************************************************************************
 *                      <Os_UpdateScheduleTbl>
 * In : EventType eventId
 * Out: None
 * return: None
 * description: This function shall update the event state.
 *              As will as the corresponding task/s state.
 *          Task Id 00 < b00 b01 b02 b03 b04 b05 b06 b07 > Event 7 -> Task 0
 *          Task Id 01 < b08 b09 b0A b0B b0C b0D b0E b0F > Event 15 -> Task 1
 *          (eventId >> OS_CFG_MAX_EVENTS_PER_TASK_P2)   > TaskId
 ***************************************************************************/
LOCAL_INLINE void Os_UpdateScheduleTbl(EventType eventId)
{
   TaskType taskId = (eventId >> OS_CFG_MAX_EVENTS_PER_TASK_P2); /*Get the corresponding task Id.*/

   Os_Hal_DisableOsInterrupts(); /* > Make sure that no other Os users will interrupt us at this point.*/

   Os_Event_Set(OS_GET_EVENTS_REF(), /*Activate Event..*/ 
                taskId,
                eventId);

   if(OS_TASK_WAITING == OS_GET_TASK_STATE(taskId)) /* If it is not waiting then it shall be (Running/Ready)-> 
                                                            which in both cases will serve this event
                              if the task is suspended then it we will be eligible to use CPU time until it is resumed ..*/
   {
      OS_SET_TASK_TO_READY(taskId); /*Make it ready..*/

      Os_EnqueueTask(OS_GET_SCHEDULER_QUEUE_REF(), /*Add the task to the queue..*/
                     taskId,
                     OS_GET_TASK_PRIORITY(taskId));
   }

   Os_Hal_EnableOsInterrupts(); /* > Dispatch the pending Os interrupts if there is any.*/
}/*Os_UpdateScheduleTbl*/

#endif /* TIA_RTOS_OS_SCHEDULER_H_ */

#endif /* OS_SRC_FILE*/
