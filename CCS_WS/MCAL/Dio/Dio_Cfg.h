/*
 * Dio_Cfg.h
 *
 *  Created on: Oct 25, 2020
 *      Author: Ahmed Yusri Mohamed Ezzat.
 */

#ifndef MCAL_DIO_DIO_CFG_H_
#define MCAL_DIO_DIO_CFG_H_

#include "TM4C129E_HwGpioInfo.h"

/**********************************************************************************************
 * Dio Configured Channels Id
**********************************************************************************************/

/*Dio_ChannelType*/
#define DIO_USER_BUTTON1_PIN_ID                  ((Dio_ChannelType)0x00)
#define DIO_USER_BUTTON2_PIN_ID                  ((Dio_ChannelType)0x01)
#define DIO_USER_LED0_PIN_ID                     ((Dio_ChannelType)0x02)
#define DIO_USER_LED1_PIN_ID                     ((Dio_ChannelType)0x03)
#define DIO_USER_LED2_PIN_ID                     ((Dio_ChannelType)0x04)
#define DIO_USER_LED3_PIN_ID                     ((Dio_ChannelType)0x05)
#define DIO_USER_TESTP_PIN_ID                    ((Dio_ChannelType)0x06)

#define DIO_NUM_CFG_CHANNELS                     ((Dio_ChannelType)0x07)

#define DIO_GET_CHANNEL_DATA_REG(ID)             (Dio_Cfg.chCfg[(ID)].dataReg)
#define DIO_GET_CHANNEL_IDX(ID)                  (Dio_Cfg.chCfg[(ID)].Idx)

/*********************************************************************************************
 *  Dio Configured global types.
 *********************************************************************************************/
typedef uint8 Dio_ChannelType;
typedef uint8 Dio_PortType;
typedef uint8 Dio_LevelType;


typedef struct
{
   GpioHwRegisterRefType dataReg; /*Pin Data register base.*/
   uint8 Idx;
}Dio_ChannelCfgType;

typedef struct
{
   Dio_ChannelCfgType chCfg[DIO_NUM_CFG_CHANNELS];
}Dio_CfgType;

extern const Dio_CfgType Dio_Cfg;

#endif /* MCAL_DIO_DIO_CFG_H_ */
