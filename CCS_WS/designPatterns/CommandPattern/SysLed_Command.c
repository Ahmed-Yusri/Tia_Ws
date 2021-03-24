/*
 * Command1.c
 *
 *  Created on: Oct 30, 2020
 *      Author: Ahmed Yusri Mohammed Ezzat.
 */

#include "Command.h"
#include "LedSys_FsmInterface.h"

#define MAX_NUM_MEMO_EVENTS                               (10)

#define COMMAND_DEC_EVENT_CNT()                           (Cmd1.idx = (Cmd1.idx + MAX_NUM_MEMO_EVENTS - 1) % MAX_NUM_MEMO_EVENTS)
#define COMMAND_INC_EVENT_CNT()                           (Cmd1.idx = (Cmd1.idx + 1) % MAX_NUM_MEMO_EVENTS)


static void Execute_SysLedCommand(struct SysLedCommand *);
static void Unexecute_SysLedCommand(struct SysLedCommand *);

const struct Command SysLed_CommandObject = 
{
   (Command_ExecuteType)Execute_SysLedCommand,
   (Command_UnexecuteType)Unexecute_SysLedCommand
};

/*********************************************************************
 * Function Name: <Execute_SysLedCommand>
 * Param (IN) : struct SysLedCommand *me
 * Param (OUT): None
 * Return: None
 * Description: Execute this instance from the SysLed_Command Class
 ********************************************************************/
static void Execute_SysLedCommand(struct SysLedCommand *me)
{
   FSM_createMemento(&me->lState); /*Memorize the current state. "State Reminder"*/
   
   FSM_newEventReq(me->ev);
}/*Execute_SysLedCommand*/

/***********************************************************************
 * Function Name: <Unexecute_SysLedCommand>
 * Param (IN) : struct SysLedCommand *me
 * Param (OUT): None
 * Return: None
 * Description: Undo this instance from the SysLed_Command Class
 **********************************************************************/
static void Unexecute_SysLedCommand(struct SysLedCommand *me)
{
   FSM_setMemento(me->lState); /*Revert the last action.. Set "State Reminder".*/
}/*Unexecute_SysLedCommand*/

void SysLed_Command_Constructor(struct SysLedCommand *me, const SysEventType event)
{
   /*Init the execute/undo funtion pointer..*/
   //me->myCommand.execute = Execute_SysLedCommand;
   //me->myCommand.undo = Unexecute_SysLedCommand;
   //me->ev = event; /*Update the mapped event..*/
}/*SysLed_Command_Constructor*/
