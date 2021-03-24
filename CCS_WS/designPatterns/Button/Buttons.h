/*
 * Buttons.h
 *
 *  Created on: Oct 25, 2020
 *      Author: ahmed
 */

#ifndef STATEPATTERN_EXAMPLE_BUTTONS_H_
#define STATEPATTERN_EXAMPLE_BUTTONS_H_

#include "CommandPattern_Interface.h"


void Buttons_MainHdl(void);
void RemoteLoader(const uint32 *buttonsCfg);

/***********************************************************************
 * Function Name: <SetCommand>
 * Param (IN) : uint8 inputIdx, Command_Type commandId
 * Param (OUT): None
 * Return: None
 * Description: Load the input tbl with the appropriate command Id.
 **********************************************************************/
void SetCommand(uint8 inputIdx, Command_Type commandId);

#endif /* STATEPATTERN_EXAMPLE_BUTTONS_H_ */
