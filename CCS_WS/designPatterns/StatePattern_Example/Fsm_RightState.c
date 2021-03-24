/*
 * Fsm_RightState.c
 *
 *  Created on: Oct 27, 2020
 *      Author: Ahmed Yusri Mohamed Ezzat.
 */

#include "Fsm_InternalState.h"


static boolean Fsm_enterRightState(void);
static boolean Fsm_exitRightState(void);
static void Fsm_RightStateNewEv(SysEventType Ev);
static const struct internalStateCtx *Fsm_cyclicRightState(void);
static void Fsm_RightStateCreateMemento(Fsm_MementoType *me);

const struct internalStateCtx Fsm_RightState =\
{
   &Fsm_enterRightState,
   &Fsm_exitRightState,
   &Fsm_RightStateNewEv,
   &Fsm_cyclicRightState,
   &Fsm_RightStateCreateMemento
};

static struct internalFsmState intFsm_RightState = {&Fsm_RightState};

static boolean Fsm_enterRightState(void)
{
   Led_Blinky(6); /*State Enter.. Indication*/

   return TRUE;
}/*Fsm_enterRightState*/

static boolean Fsm_exitRightState(void)
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
}/*Fsm_exitRightState*/

static void Fsm_RightStateNewEv(SysEventType Ev)
{
   if(E_SYSTEM_EV1_IS_SET == Ev)
   {
      intFsm_RightState.nextCtx = &Fsm_LeftState;
   }
   else
   {
      intFsm_RightState.nextCtx = &Fsm_IdleState;
   }
}/*Fsm_RightStateNewEv*/

static void Fsm_RightStateCreateMemento(Fsm_MementoType *me)
{
   me->state = E_SYSTEM_EV2_IS_SET;
}/*Fsm_RightStateCreateMemento*/

static const struct internalStateCtx *Fsm_cyclicRightState(void)
{
   const struct internalStateCtx *curCtx = &Fsm_RightState;

   if(curCtx != intFsm_RightState.nextCtx)
   {
      if(curCtx->exitEv())
      {
         curCtx = intFsm_RightState.nextCtx;
         (void)curCtx->enterEv();
         intFsm_RightState.nextCtx = &Fsm_RightState;
      }
   }
   else if(E_LED_IS_IDLE == Led_getSequenceId())
   {
      Led_startSequence(E_LED_SHIFT_RIGHT);
   }
   else{}

   return curCtx;
}/*Fsm_cyclicRightState*/


