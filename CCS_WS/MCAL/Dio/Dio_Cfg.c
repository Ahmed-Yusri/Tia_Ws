/*
 * Dio_Cfg.c
 *
 *  Created on: Oct 25, 2020
 *      Author: Ahmed Yusri Mohamed Ezzat.
 */



#include "Dio.h"
#include "TM4C129E_HwGpioInfo.h"

#define DIO_MEMMAP_START_CONST_VAR
#include "Dio_MemMap.h"

const Dio_CfgType Dio_Cfg = /*Dio Main Configuration Container..*/
{
   {  /* dataReg, Channel Idx*/
      { (GpioHwRegisterRefType)GPIO_PORTJ_REG_INFO, (uint8)0X00 },
      { (GpioHwRegisterRefType)GPIO_PORTJ_REG_INFO, (uint8)0X01 },
      { (GpioHwRegisterRefType)GPIO_PORTN_REG_INFO, (uint8)0X01 },
      { (GpioHwRegisterRefType)GPIO_PORTN_REG_INFO, (uint8)0X00 },
      { (GpioHwRegisterRefType)GPIO_PORTF_REG_INFO, (uint8)0X04 },
      { (GpioHwRegisterRefType)GPIO_PORTF_REG_INFO, (uint8)0X00 },
      { (GpioHwRegisterRefType)GPIO_PORTF_REG_INFO, (uint8)0X01 }
   }
};

#define DIO_MEMMAP_STOP_SEC_CONST_VAR
#include <MCAL/Dio/Dio_MemMap.h>
