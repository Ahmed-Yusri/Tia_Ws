/*
 * Fsm_IdleState.c
 *
 *  Created on: Oct 27, 2020
 *      Author: ahmed
 */

#include "Fsm_InternalState.h"


static boolean Fsm_enterIdleState(void);
static boolean Fsm_exitIdleState(void);
static void Fsm_IdleStateNewEv(SysEventType Ev);
static const struct internalStateCtx *Fsm_cyclicIdleState(void);
static void Fsm_IdleStateCreateMemento(Fsm_MementoType *me);

const struct internalStateCtx Fsm_IdleState =\
{
   &Fsm_enterIdleState,
   &Fsm_exitIdleState,
   &Fsm_IdleStateNewEv,
   &Fsm_cyclicIdleState,
   &Fsm_IdleStateCreateMemento
};

static struct internalFsmState intFsm_IdleState = {&Fsm_IdleState};

static boolean Fsm_enterIdleState(void)
{
   Led_Blinky(24); /*State Enter.. Indication*/

   return TRUE;
}/*Fsm_enterIdleState*/

static boolean Fsm_exitIdleState(void)
{
   boolean result = FALSE;

   if(E_LED_IS_IDLE == Led_getSequenceId())
   {
      result = TRUE;
   }
   else if(E_LED_OFF != Led_getSequenceId())
   {
      Led_startSequence(E_LED_OFF);
   }
   else{}

   return result;
}/*Fsm_exitIdleState*/

static void Fsm_IdleStateNewEv(SysEventType Ev)
{
   if(E_SYSTEM_EV2_IS_SET == Ev)
   {
      intFsm_IdleState.nextCtx = &Fsm_RightState;
   }
   else
   {
      intFsm_IdleState.nextCtx = &Fsm_LeftState;
   }
}/*Fsm_IdleStateNewEv*/

static void Fsm_IdleStateCreateMemento(Fsm_MementoType *me)
{
   me->state = E_SYSTEM_NO_EVENTS;
}/*Fsm_IdleStateCreateMemento*/

static const struct internalStateCtx *Fsm_cyclicIdleState(void)
{
   const struct internalStateCtx *curCtx = &Fsm_IdleState;

   if(curCtx != intFsm_IdleState.nextCtx)
   {
      if(curCtx->exitEv())
      {
         curCtx = intFsm_IdleState.nextCtx;
         (void)curCtx->enterEv();
         intFsm_IdleState.nextCtx = &Fsm_IdleState;
      }
   }

   return curCtx;
}/*Fsm_cyclicIdleState*/
