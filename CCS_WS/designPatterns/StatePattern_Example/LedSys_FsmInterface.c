/*
 * LedSys_FsmInterface.c
 *
 *  Created on: Oct 26, 2020
 *      Author: ahmed
 */

#include "Fsm_InternalState.h"
#include "LedSys_FsmInterface.h"

static struct
{
   internalStateCtxRefType ctx;
   struct Fsm_Memento memento[10];
   uint8 usedMementoIdx;
}Fsm_Object = 
{
   .ctx = &Fsm_IdleState,
   .usedMementoIdx = 0
};

/******************************************************************
 * Function Name: <FSM_newEventReq>
 * Param (IN) : None
 * Param (OUT): None
 * Return: SysEventType
 * Description: register the new event.
 ****************************************************************/
void FSM_newEventReq(SysEventType ev)
{
   Fsm_Object.ctx->newEv(ev);
}/*FSM_newEventReq*/

/******************************************************************
 * Function Name: <FSM_cyclicProc>
 * Param (IN) : None
 * Param (OUT): None
 * Return: None
 * Description:Cyclic function to handle the internal state machine
 *******************************************************************/
void FSM_cyclicProc(void)
{
   Fsm_Object.ctx = Fsm_Object.ctx->cyclicEv();
}/*FSM_cyclicProc*/

/******************************************************************
 * Function Name: <FSM_createMemento>
 * Param (IN) : None
 * Param (OUT): struct Fsm_Memento **me
 * Return: None
 * Description: Capture the current internal state.
 *******************************************************************/
void FSM_createMemento(struct Fsm_Memento **me)
{
   *me = &Fsm_Object.memento[Fsm_Object.usedMementoIdx];

   Fsm_Object.ctx->createMemento(*me);

   Fsm_Object.usedMementoIdx = (Fsm_Object.usedMementoIdx + 1) % 10;
}/*FSM_createMemento*/

/******************************************************************
 * Function Name: <FSM_setMemento>
 * Param (IN) : const struct Fsm_Memento *me
 * Param (OUT): None
 * Return: None
 * Description: Restore state.
 *******************************************************************/
void FSM_setMemento(const struct Fsm_Memento *me)
{  
   uint8 tempIdx = (Fsm_Object.usedMementoIdx + 9) % 10;

   if(me == &Fsm_Object.memento[tempIdx])
   {
       Fsm_Object.usedMementoIdx = tempIdx;
      Fsm_Object.ctx->newEv(me->state); /*restore the state.*/
   }
   /*Unsupported behavior.*/
   
}/*FSM_setMemento*/
