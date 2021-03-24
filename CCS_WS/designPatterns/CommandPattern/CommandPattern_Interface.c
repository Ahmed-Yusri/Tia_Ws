/*
 * CommandPattern_Interface.c
 *
 *  Created on: Oct 30, 2020
 *      Author: Ahmed Yusri Mohammed.
 */

#include "Command.h"
#include "CommandPattern_Interface.h"

#define GET_MYCOMMAND_OBJECT(CMD)           ((struct Command *)(*CMD))

static SYSLED_COMMAND_CONSTRUCT(Mode1Command, E_SYSTEM_EV1_IS_SET);
static SYSLED_COMMAND_CONSTRUCT(Mode2Command, E_SYSTEM_EV2_IS_SET);

typedef struct myCommand
{
   const struct Command *myCommand;
}myCommandType;

typedef myCommandType * myCommandRefType;

static struct
{
   myCommandRefType const Tbl[NUM_OF_SUPPOERTED_COMANDS];
   Command_Type history[10];
   uint8 idx;
   uint8 size;
}Invoker =\
{
   { (myCommandRefType)&Mode1Command, (myCommandRefType)&Mode2Command,NULL_PTR, NULL_PTR},
   { COMMAND_NO_COMMAND_ID, COMMAND_NO_COMMAND_ID, COMMAND_NO_COMMAND_ID, COMMAND_NO_COMMAND_ID, COMMAND_NO_COMMAND_ID, COMMAND_NO_COMMAND_ID, COMMAND_NO_COMMAND_ID, COMMAND_NO_COMMAND_ID, COMMAND_NO_COMMAND_ID, COMMAND_NO_COMMAND_ID },
   0, 0
};


/*************************************************************************************************
 *  LOCAL Functions
 *************************************************************************************************/
static inline void History_addNewCommand(Command_Type myCommand)
{
   Invoker.history[Invoker.idx] = myCommand; /*We want to memorize this command.*/
   Invoker.idx = (Invoker.idx + 1) % 10; /*POINT ON THE NEXT FREE LOCATION.*/

   if(Invoker.size < 10) /*Just increment the history size if we have a free slot.*/
   {
      ++Invoker.size;
   }
   /*Otherwise we overwrite one of the used slots.*/
}/*History_addNewCommand*/

static inline Command_Type History_getLastCommand(void)
{
   Command_Type lastCommand = COMMAND_NO_COMMAND_ID;

   if(0 < Invoker.size) /*We have some thing to undo..*/
   {
      --Invoker.size;

      Invoker.idx = (Invoker.idx + 9) % 10; /*GET THE LAST COMMAND INDEX.*/

      lastCommand = Invoker.history[Invoker.idx];   
   }

   return lastCommand;
}/*History_getLastCommand*/

/***********************************************************************************
 * Global Functions
 ***********************************************************************************/

/******************************************************************
 * Function Name: <Command_Dispatch>
 * Param (IN) : Command_IdType commandId
 * Param (OUT): None
 * Return: None
 * Description: Perform the appropriate actions for THIS COMMAND.
 ****************************************************************/
void Command_Dispatch(Command_Type commandId)
{
   if((COMMAND_NO_COMMAND_ID > commandId) &&
      (NULL_PTR != Invoker.Tbl[commandId]))
   {
      History_addNewCommand(commandId); /*Add the new command execute to the history. "Supporting: Undo Operation"*/

      Invoker.Tbl[commandId]->myCommand->execute(Invoker.Tbl[commandId]); /*execute my command..*/
   }
}/*Command_Dispatch*/

/******************************************************************
 * Function Name: <Undo_Command>
 * Param (IN) : None
 * Param (OUT): None
 * Return: None
 * Description: Undo the last action.
 ****************************************************************/
void Undo_Command(void)
{
   Command_Type lastCommand = History_getLastCommand();

   if(COMMAND_NO_COMMAND_ID != lastCommand) /*If we have some thing to undo..*/
   {
       Invoker.Tbl[lastCommand]->myCommand->undo(Invoker.Tbl[lastCommand]); /*undo the last command..*/
   }
}/*Undo_Command*/
