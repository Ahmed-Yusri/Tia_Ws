/*
 * Os_Events.h
 *
 *  Created on: Sep 23, 2020
 *      Author: Ahmed Yusri Mohamed..
 */

#ifndef TIA_RTOS_me_H_
#define TIA_RTOS_me_H_

#include "Compiler.h"
#include "OS.h"

/*************************************************************************
 *        <Os_Event_Set>
 * In : EventMaskRefType me, TaskType taskId, const EventType eventId
 * Out: none
 * return: void
 * description:set an event..
 ***************************************************************************/
LOCAL_INLINE void Os_Event_Set(EventMaskRefType me, TaskType taskId, EventType eventId)
{
   me[taskId] |=  (1 << (eventId % OS_CFG_MAX_EVENTS_PER_TASK));  /*Set the corsponding bit.*/
}/*Os_Event_Set*/

/*************************************************************************
 *        <Os_Event_SetEventMask>
 * In : EventMaskRefType me, TaskType taskId, const EventMaskType evMask
 * Out: none
 * return: void
 * description:set an event..
 ***************************************************************************/
LOCAL_INLINE void Os_Event_SetEventMask(EventMaskRefType me, TaskType taskId, const EventMaskType evMask)
{
   me[taskId] |= evMask; /*Set the corsponding bits.*/
}/*Os_Event_SetEventMask*/

/*************************************************************************
 *        <Os_Event_Clear>
 * In : EventMaskRefType me, const EventType eventId
 * Out: none
 * return: void
 * description:clear an event..
 ***************************************************************************/
LOCAL_INLINE void Os_Event_Clear(EventMaskRefType me, TaskType taskId, EventType eventId)
{
   me[taskId] &= ~(1 << (eventId % OS_CFG_MAX_EVENTS_PER_TASK)); /*Clear the corsponding bit.*/
}/*Os_Event_Clear*/

/*************************************************************************
 *        <Os_Event_GetTaskEvents>
 * In : EventMaskRefType me, TaskType taskId
 * Out: none
 * return: EventMaskType
 * description:get task events state..
 ***************************************************************************/
LOCAL_INLINE EventMaskType Os_Event_GetTaskEvents(EventMaskRefType me, TaskType taskId)
{
   return me[taskId];
}/*Os_Event_GetTaskEvents*/

/*************************************************************************
 *        <Os_Event_ClearTaskEvents>
 * In : EventMaskRefType me, TaskType taskId, EventMaskType mask
 * Out: none
 * return: none
 * description:clear task events..
 ***************************************************************************/
LOCAL_INLINE void Os_Event_ClearTaskEvents(EventMaskRefType me, TaskType taskId, EventMaskType mask)
{
   me[taskId] &= ~mask;
}/*Os_Event_ClearTaskEvents*/


#endif /* TIA_RTOS_me_H_ */
