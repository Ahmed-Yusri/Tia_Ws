/*
 * StatePattern.c
 *
 *  Created on: Oct 25, 2020
 *      Author: Ahmed Yusri Mohamed Ezzat.
 */

#include "LedSys_FsmInterface.h"
#include "Led.h"


#if 0
static struct
{
   SysEventType pendingEv;
   SysEventType lastEv;
}StatePattern_Ctx =
{
   E_SYSTEM_NO_EVENTS,
   E_SYSTEM_NO_EVENTS
};

void FSM_newEventReq(SysEventType ev)
{
   StatePattern_Ctx.pendingEv = ev;
}/*FSM_newEventReq*/

void FSM_cyclicProc(void)
{
   if(E_SYSTEM_EV1_IS_SET == StatePattern_Ctx.pendingEv)
   {
      if(E_SYSTEM_EV1_IS_SET == StatePattern_Ctx.lastEv)
      {
         Led_startSequence(E_LED_OFF);
         StatePattern_Ctx.lastEv = E_SYSTEM_NO_EVENTS;
      }
      else if(E_SYSTEM_EV2_IS_SET == StatePattern_Ctx.lastEv)
      {
         Led_startSequence(E_LED_OFF);
         StatePattern_Ctx.lastEv = E_SYSTEM_EV1_IS_SET;
      }
      else
      {
         Led_startSequence(E_LED_BLINKY);
         StatePattern_Ctx.lastEv = E_SYSTEM_EV1_IS_SET;
      }
   }
   else if(E_SYSTEM_EV2_IS_SET == StatePattern_Ctx.pendingEv)
   {
      if(E_SYSTEM_EV2_IS_SET == StatePattern_Ctx.lastEv)
      {
         Led_startSequence(E_LED_OFF);
         StatePattern_Ctx.lastEv = E_SYSTEM_NO_EVENTS;
      }
      else if(E_SYSTEM_EV1_IS_SET == StatePattern_Ctx.lastEv)
      {
         Led_startSequence(E_LED_OFF);
         StatePattern_Ctx.lastEv = E_SYSTEM_EV2_IS_SET;
      }
      else
      {
         Led_startSequence(E_LED_BLINKY);
         StatePattern_Ctx.lastEv = E_SYSTEM_EV2_IS_SET;
      }
   }
   else
   {
      
   }
   StatePattern_Ctx.pendingEv = E_SYSTEM_NO_EVENTS;
}/*FSM_cyclicProc*/

#endif

void waitOnEnter_isDone(void)
{
#if 0
    switch(StatePattern_Ctx.lastEv)
   {
      case E_SYSTEM_EV1_IS_SET:
      {
         Led_startSequence(E_LED_SHIFT_LEFT);
         break;
      }
      case E_SYSTEM_EV2_IS_SET:
      {
         Led_startSequence(E_LED_SHIFT_RIGHT);
         break;
      }
      case E_SYSTEM_NO_EVENTS:
      default:
      {
         break;
      }
   }
#endif
}/*waitOnEnter_isDone*/

void waitOnExit_isDone(void)
{
#if 0
   Led_startSequence(E_LED_BLINKY); /*Indicate that we are serving the new event.*/
#endif
}/*waitOnExit_isDone*/

