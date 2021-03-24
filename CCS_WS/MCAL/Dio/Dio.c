/*
 * Dio.c
 *
 *  Created on: Oct 23, 2020
 *      Author: ahmed
 */

#include "Dio.h"

/********************************************************************
 * PRIV MACROS.
 ********************************************************************/
#define DIO_LWRITE_CHANNEL(BASE, PIN, LEVEL)                 ((BASE)[1 << (PIN)] = ((LEVEL) << (PIN)))
#define DIO_LREAD_CHANNEL(BASE, PIN)                         ((BASE)[1 << (PIN)] >> (PIN))

#define DIO_MEMMAP_START_SEC_CODE
#include "Dio_MemMap.h"

/******************************************************************
 *  Global Functions.
 *******************************************************************/

/******************************************************************
 * Function Name: <Dio_ReadChannel>
 * Param (IN) : Dio_ChannelType ChannelId
 * Param (OUT): None
 * Return: Dio_LevelType
 * Description: Returns the value of the specified DIO channel.
 ****************************************************************/
Dio_LevelType Dio_ReadChannel(Dio_ChannelType ChannelId)
{
   Dio_LevelType level = DIO_LREAD_CHANNEL(DIO_GET_CHANNEL_DATA_REG(ChannelId),  /*Return the actual data..*/ 
                                           DIO_GET_CHANNEL_IDX(ChannelId));
   return level;
}/*Dio_ReadChannel*/

/******************************************************************
 * Function Name: <Dio_ReadChannel>
 * Param (IN) : Dio_ChannelType ChannelId, Dio_LevelType Level
 * Param (OUT): None
 * Return: None
 * Description: Service to set a level of a channel.
 ****************************************************************/
void Dio_WriteChannel(Dio_ChannelType ChannelId, Dio_LevelType Level)
{
   DIO_LWRITE_CHANNEL(DIO_GET_CHANNEL_DATA_REG(ChannelId),  /*Wrtie Channel..*/ 
                      DIO_GET_CHANNEL_IDX(ChannelId),
                      Level);
}/*Dio_WriteChannel*/

/*********************************************************************************************
 * Function Name: <Dio_FlipChannel>
 * Param (IN) : Dio_ChannelType ChannelId
 * Param (OUT): None
 * Return: Dio_LevelType
 * Description: Service to flip (change from 1 to 0 or from 0 to 1) 
 *              the level of a channel and return the level of the channel after flip.
 ********************************************************************************************/
Dio_LevelType Dio_FlipChannel(Dio_ChannelType ChannelId)
{
   Dio_LevelType level = DIO_LREAD_CHANNEL(DIO_GET_CHANNEL_DATA_REG(ChannelId),  /*Return the actual data..*/ 
                                           DIO_GET_CHANNEL_IDX(ChannelId));
   
   DIO_LWRITE_CHANNEL(DIO_GET_CHANNEL_DATA_REG(ChannelId),  /*Flip 'change from 1 to 0 or from 0 to 1'.*/
                      DIO_GET_CHANNEL_IDX(ChannelId),
                      ~level);

   return level; /*return the level of the channel after flip.*/
}

#define DIO_MEMMAP_STOP_SEC_CODE
#include <MCAL/Dio/Dio_MemMap.h>
