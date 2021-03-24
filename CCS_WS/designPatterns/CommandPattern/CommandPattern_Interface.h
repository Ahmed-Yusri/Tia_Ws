/*
 * CommandPattern_Interface.h
 *
 *  Created on: Oct 30, 2020
 *      Author: Ahmed Yusri Mohammed Ezzat
 */

#ifndef DESIGNPATTERNS_COMMANDPATTERN_COMMANDPATTERN_INTERFACE_H_
#define DESIGNPATTERNS_COMMANDPATTERN_COMMANDPATTERN_INTERFACE_H_

#include "Std_Types.h"

/*Command_Type*/
#define LED_SYS_MODE_1_COMMAND_ID           ((uint8)0)
#define LED_SYS_MODE_2_COMMAND_ID           ((uint8)1)
#define DOOR_SYS_OPEN_COMMAND_ID            ((uint8)2)
#define DOOR_SYS_CLOSE_COMMAND_ID           ((uint8)3)
#define COMMAND_NO_COMMAND_ID               ((uint8)4)
#define NUM_OF_SUPPOERTED_COMANDS           COMMAND_NO_COMMAND_ID

typedef uint8 Command_Type;

/******************************************************************
 * Function Name: <Command_Dispatch>
 * Param (IN) : Command_IdType myCommand
 * Param (OUT): None
 * Return: None
 * Description: Perform the appropriate actions for THIS COMMAND.
 ****************************************************************/
void Command_Dispatch(Command_Type myCommand);

/******************************************************************
 * Function Name: <Undo_Command>
 * Param (IN) : None
 * Param (OUT): None
 * Return: None
 * Description: Undo the last action.
 ****************************************************************/
void Undo_Command(void);

#endif /* DESIGNPATTERNS_COMMANDPATTERN_COMMANDPATTERN_INTERFACE_H_ */
