/*
 * Port.c
 *  Created on: Oct 23, 2020
 *      Author: Ahmed Yusri Mohamed Ezzat.
 *
 *      File Revision:
 *                     Oct 23, 2020   Ahmed Yusri  v0.1 Initial Version
 */


#ifndef MCAL_PORT_PORT_H_
#define MCAL_PORT_PORT_H_

#include "Std_Types.h"
#include "Port_Cfg.h"

/******************************************************************
 * Function Name: <Port_Init>
 * Param (IN) : const Port_ConfigType* ConfigPtr
 * Param (OUT): None
 * Return: None
 * Description: Initializes the Port Driver module. 
 ****************************************************************/
void Port_Init(const Port_ConfigType* ConfigPtr);

/******************************************************************
 * Function Name: <Port_SetPinDirection>
 * Param (IN) : Port_PinType Pin, Port_PinDirectionType Direction
 * Param (OUT): None
 * Return: None
 * Description: Sets the port pin direction.
 ****************************************************************/
void Port_SetPinDirection(Port_PinType Pin, Port_PinDirectionType Direction);

/******************************************************************
 * Function Name: <Port_SetPinMode>
 * Param (IN) : Port_PinType Pin, Port_PinModeType Mode
 * Param (OUT): None
 * Return: None
 * Description: Sets the port pin mode.
 ****************************************************************/
void Port_SetPinMode(Port_PinType Pin, Port_PinModeType Mode);
#endif /* MCAL_PORT_PORT_H_ */
