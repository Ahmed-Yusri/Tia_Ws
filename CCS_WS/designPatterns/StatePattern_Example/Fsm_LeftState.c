/*
 * Fsm_LeftState.c
 *
 *  Created on: Oct 27, 2020
 *      Author: ahmed
 */

#include "Fsm_InternalState.h"


static boolean Fsm_enterLeftState(void);
static boolean Fsm_exitLeftState(void);
static void Fsm_LeftStateNewEv(SysEventType Ev);
static const struct internalStateCtx *Fsm_cyclicLeftState(void);
static void Fsm_LeftStateCreateMemento(Fsm_MementoType *me);

const struct internalStateCtx Fsm_LeftState =\
{
   &Fsm_enterLeftState,
   &Fsm_exitLeftState,
   &Fsm_LeftStateNewEv,
   &Fsm_cyclicLeftState,
   &Fsm_LeftStateCreateMemento
};

static struct internalFsmState intFsm_LeftState = {&Fsm_LeftState};

static boolean Fsm_enterLeftState(void)
{
   Led_Blinky(12); /*State Enter.. Indication*/

   return TRUE;
}/*Fsm_enterLeftState*/

static boolean Fsm_exitLeftState(void)
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
}/*Fsm_exitLeftState*/

static void Fsm_LeftStateNewEv(SysEventType Ev)
{
   if(E_SYSTEM_EV2_IS_SET == Ev)
   {
      intFsm_LeftState.nextCtx = &Fsm_RightState;
   }
   else
   {
      intFsm_LeftState.nextCtx = &Fsm_IdleState;
   }
}/*Fsm_LeftStateNewEv*/

static void Fsm_LeftStateCreateMemento(Fsm_MementoType *me)
{
   me->state = E_SYSTEM_EV1_IS_SET;
}/*Fsm_IdleStateCreateMemento*/

static const struct internalStateCtx *Fsm_cyclicLeftState(void)
{
   const struct internalStateCtx *curCtx = &Fsm_LeftState;

   if(curCtx != intFsm_LeftState.nextCtx)
   {
      if(curCtx->exitEv())
      {
         curCtx = intFsm_LeftState.nextCtx;
         (void)curCtx->enterEv();
         intFsm_LeftState.nextCtx = &Fsm_LeftState;
      }
   }
   else if(E_LED_IS_IDLE == Led_getSequenceId())
   {
      Led_startSequence(E_LED_SHIFT_LEFT);
   }
   else{}

   return curCtx;
}/*Fsm_cyclicLeftState*/
