/*
 * Dio.h
 *
 *  Created on: Oct 23, 2020
 *      Author: Ahmed Yusri Mohamed Ezzat.
 *      File Revision:
 *                     Oct 23, 2020   Ahmed Yusri  v0.1 Initial Version
 */

#ifndef MCAL_DIO_DIO_H_
#define MCAL_DIO_DIO_H_

#include "Std_Types.h"
#include "Dio_Cfg.h"

#define DIO_INVALID_CHANNEL_ID              ((Dio_ChannelType)0XFF)

/************************************************************************************************
 * Dio Global Functions.
 ***********************************************************************************************/
/***********************************************************************************************
 * Function Name: <Dio_ReadChannel>
 * Param (IN) : Dio_ChannelType chId
 * Param (OUT): None
 * Return: Dio_LevelType
 * Description: Returns the value of the specified DIO channel.
 ***********************************************************************************************/
Dio_LevelType Dio_ReadChannel(Dio_ChannelType ChannelId);

/********************************************************************************************
 * Function Name: <Dio_ReadChannel>
 * Param (IN) : Dio_ChannelType ChannelId, Dio_LevelType Level
 * Param (OUT): None
 * Return: None
 * Description: Service to set a level of a channel.
 ********************************************************************************************/
void Dio_WriteChannel(Dio_ChannelType ChannelId, Dio_LevelType Level);

/*********************************************************************************************
 * Function Name: <Dio_FlipChannel>
 * Param (IN) : Dio_ChannelType ChannelId
 * Param (OUT): None
 * Return: Dio_LevelType
 * Description: Service to flip (change from 1 to 0 or from 0 to 1) 
 *              the level of a channel and return the level of the channel after flip.
 ********************************************************************************************/
Dio_LevelType Dio_FlipChannel(Dio_ChannelType ChannelId);

#endif /* MCAL_DIO_DIO_H_ */
