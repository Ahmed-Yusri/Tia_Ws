/*
 * Os_TaskQueue.h
 *
 *  Created on: Aug 29, 2020
 *      Author: Ahmed Yusri Mohamed..
 */

#ifndef ORTOS_OS_TASKQUEUE_H_
#define ORTOS_OS_TASKQUEUE_H_

#include "Compiler.h"
#include "OS.h"
#include "Os_PrivMacro.h"

/********************************************************************************************
 * MACROS
 ********************************************************************************************/

#define TASK_QUEUE_CONSTRUCTOR(ME, PRIORTIY, TASK_ID) (*((Os_TaskQueueEntryType*)ME) = (Os_TaskQueueEntryType){TASK_ID, PRIORTIY})

#define TASK_QUEUE_GET_PRIORITY(ME)                   (ME)->entry.priority
#define TASK_QUEUE_GET_NEXT_PTR(ME)                   (ME)->next
#define TASK_QUEUE_SET_NEXT_PTR(ME, NEXT)             (ME)->next = (NEXT)
#define TASK_QUEUE_GET_PREV_PTR(ME)                   (ME)->prev
#define TASK_QUEUE_SET_PREV_PTR(ME, PREV)             (ME)->prev = (PREV)
#define TASK_QUEUE_GET_ID(ME)                         (ME)->entry.id

#define OS_TASK_QUEUE_FULL(me)                    (OS_CFG_MAX_READY_TASK_QSIZE == (me)->used_size)
#define OS_TASK_QUEUE_EMPTY(me)                   (0 == (me)->used_size)

/*******************************************************************************
 * Q_Entry -> { Task Id, Task Priority } 
 *******************************************************************************/

typedef uint8_t OsQueuePtrRefType;

typedef struct
{
   TaskType id; 
   TaskPrioType priority;
}Os_TaskQueueEntryType;

struct TaskQueueNode
{
   Os_TaskQueueEntryType entry; /*Inherit the task properties..*/
   OsQueuePtrRefType next;
   OsQueuePtrRefType prev;
};

typedef struct
{
   struct TaskQueueNode node[OS_CFG_MAX_READY_TASK_QSIZE];
   OsQueuePtrRefType front;
   OsQueuePtrRefType rear;
   uint8_t used_size;
}Os_TaskQueueType;

/*************************************************************************
 *        <Os_Queue_NodeLink>
 * In : struct TaskQueueNode *me, OsQueuePtrRefType next, OsQueuePtrRefType prev
 * Out: me init    TaskQueueNode *n
 * return: None
 * description: Link (this) Queue node.
 ***************************************************************************/
LOCAL_INLINE void Os_Queue_NodeLink(struct TaskQueueNode *me, OsQueuePtrRefType next, OsQueuePtrRefType prev)
{
   me->next = next;
   me->prev = prev;
}/*Os_Queue_NodeLink*/

/*************************************************************************
 *        <Os_Queue_FreeNode>
 * In : Os_TaskQueueType *me, OsQueuePtrRefType node_ptr
 * Out: None
 * return: None
 * description: Free (this) node. 
 ***************************************************************************/
LOCAL_INLINE void Os_Queue_FreeNode(Os_TaskQueueType *me, OsQueuePtrRefType node_ptr)
{
   /*rear -> last free element..*/
   struct TaskQueueNode *queue_base = &me->node[0];
   OsQueuePtrRefType old_prev = TASK_QUEUE_GET_PREV_PTR(&queue_base[me->rear]);
   OsQueuePtrRefType old_next = TASK_QUEUE_GET_NEXT_PTR(&queue_base[me->rear]);

   Os_Queue_NodeLink(&queue_base[node_ptr], /*Link the new rear node..*/
                     old_next,
                     me->rear);

   Os_Queue_NodeLink(&queue_base[me->rear], /*Update the old rear node..*/
                     node_ptr,
                     old_prev);

   me->rear = node_ptr; /*Update the rear ptr..*/
}/*Os_Queue_FreeNode*/

/*************************************************************************
 *        <Os_Queue_Consume>
 * In : OsQueuePtrRefType node_ptr
 * Out: None
 * return: OsQueuePtrRefType
 * description: Get free node. 
 ***************************************************************************/
LOCAL_INLINE OsQueuePtrRefType Os_Queue_Consume(Os_TaskQueueType *me)
{
   /*rear -> last free element..*/
   struct TaskQueueNode *queue_base = &me->node[0];
   OsQueuePtrRefType prev = TASK_QUEUE_GET_PREV_PTR(&queue_base[me->rear]);
   OsQueuePtrRefType next = TASK_QUEUE_GET_NEXT_PTR(&queue_base[me->rear]);

   Os_Queue_NodeLink(&queue_base[prev], /*Update the prev rear node..*/
                     next,
                     TASK_QUEUE_GET_PREV_PTR(&queue_base[prev]));

   next = me->rear; /*temp holder*/
   me->rear = prev; /*update the rear ptr..*/
   return next;
}/*Os_Queue_Consume*/

/*************************************************************************
 *        <Os_Queue_Construct>
 * In : me address Os_TaskQueueType *me
 * Out: me init    Os_TaskQueueType *me
 * return: None
 * description: Initialize the me queue.
 ***************************************************************************/
LOCAL_INLINE void Os_Queue_Construct(Os_TaskQueueType *me)
{
   OsQueuePtrRefType entry_ptr;
   struct TaskQueueNode *queue_base = &me->node[0];

   me->front = (OsQueuePtrRefType)0;
   me->rear =  (OsQueuePtrRefType)OS_CFG_MAX_READY_TASK_QSIZE - 1;
   me->used_size = (OsQueuePtrRefType)0;

   for(entry_ptr = 0;entry_ptr < OS_CFG_MAX_READY_TASK_QSIZE;entry_ptr++)
   {
      Os_Queue_NodeLink(&queue_base[entry_ptr], /*Update the prev rear node..*/
                        ((entry_ptr + 1) % OS_CFG_MAX_READY_TASK_QSIZE),
                        ((entry_ptr + (OS_CFG_MAX_READY_TASK_QSIZE - 1)) % OS_CFG_MAX_READY_TASK_QSIZE));
   }
}/*Os_Queue_Constructor*/

/*************************************************************************
 *        <Os_EnqueueTask>
 * In : Os_TaskQueueType *me, TaskType TaskId, uint8_t task_priority
 * Out: None
 * return: None
 * description: Add task to the me queue and increment the me queue size..
 ***************************************************************************/
LOCAL_INLINE void Os_EnqueueTask(Os_TaskQueueType *me, TaskType TaskId, TaskPrioType task_priority)
{
   OsQueuePtrRefType xnode_ptr = me->front;
   OsQueuePtrRefType xnode_prev;
   OsQueuePtrRefType xnode_next;
   struct TaskQueueNode *queue_base = &me->node[0];
   uint8_t used_size = me->used_size;
   OsQueuePtrRefType new_node_ptr;

   if(!OS_TASK_QUEUE_FULL(me)) /*At least we have a free location..*/
   {
      new_node_ptr = Os_Queue_Consume(me); /*Consume a free location..*/

      while(used_size) /*Loop overall the used size..*/
      {
         if(TASK_QUEUE_GET_PRIORITY(&queue_base[xnode_ptr]) > task_priority)
         {
            break; /*We found a suitable location break out the loop..*/
         }
         xnode_ptr = TASK_QUEUE_GET_NEXT_PTR(&queue_base[xnode_ptr]);
         --used_size;
      }

      if(me->front == xnode_ptr) /*If we need to change the front ptr..*/
      {
         me->front = new_node_ptr; /*Set the front ptr to the new node ptr..*/
         OS_SET_NEXT_TASK_PTR(TaskId); /*Update the next task ptr..*/
      }
      xnode_prev = TASK_QUEUE_GET_PREV_PTR(&queue_base[xnode_ptr]);
      xnode_next = TASK_QUEUE_GET_NEXT_PTR(&queue_base[xnode_ptr]);

      Os_Queue_NodeLink(&queue_base[xnode_prev], /*Link the (prev of x task node).next to the new node ptr..*/
                        new_node_ptr,
                        TASK_QUEUE_GET_PREV_PTR(&queue_base[xnode_prev]));

      Os_Queue_NodeLink(&queue_base[xnode_ptr],  /*Link the (x task node). prev to the new node ptr..*/
                        xnode_next,
                        new_node_ptr);

      Os_Queue_NodeLink(&queue_base[new_node_ptr], /*Link the new task node..*/
                        xnode_ptr,
                        xnode_prev);

      TASK_QUEUE_CONSTRUCTOR(&queue_base[new_node_ptr], /*Add new task properties..*/
                             task_priority,
                             TaskId);
      
      ++me->used_size; /*Increment the used size*/
   }
}/*Os_EnqueueTask*/

/*****************************************************************************
 *        <Os_DequeueTask>
 * In : Os_TaskQueueType *me
 * Out: None
 * return: TaskType return the dequed task Id..
 * description: delete the top task from the me queue and return the task id.
 ******************************************************************************/
LOCAL_INLINE TaskType Os_DequeueTask(Os_TaskQueueType *me)
{
   TaskType to_be_removed = OS_IDLE_TASK_ID;
   OsQueuePtrRefType old_front = me->front;

   if(!OS_TASK_QUEUE_EMPTY(me))
   {
      to_be_removed = TASK_QUEUE_GET_ID(&me->node[me->front]); /*return the top task id..*/
      me->front = TASK_QUEUE_GET_NEXT_PTR(&me->node[me->front]); /*move to the next entry...*/

      Os_Queue_FreeNode(me, old_front); /*Free the old top 'Head/Front'..*/
      
      --me->used_size; /*Decrement the used size*/
      OS_SET_NEXT_TASK_PTR(TASK_QUEUE_GET_ID(&me->node[me->front])); /*Update the next task ptr..*/
   }

   return to_be_removed;
}/*Os_DequeueTask*/

/*****************************************************************************
 *        <Os_TaskQueueTop>
 * In : Os_TaskQueueType *me
 * Out: None
 * return: TaskType 
 * description: return the top task Id.
 ******************************************************************************/
LOCAL_INLINE TaskType Os_TaskQueueTop(Os_TaskQueueType *me)
{
   return TASK_QUEUE_GET_ID(&me->node[me->front]); /*return the top task id..*/
}/*Os_TaskQueueTop*/
#endif /* ORTOS_OS_TASKQUEUE_H_ */
