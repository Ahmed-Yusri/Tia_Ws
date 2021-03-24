/*
 * System_Cfg.h
 *
 *  Created on: Jan 19, 2021
 *      Author: Ahmed Yusri Mohamed
 */

#ifndef MCAL_SYSTEM_SYSTEM_CFG_H_
#define MCAL_SYSTEM_SYSTEM_CFG_H_

#include "system.h"

#define SYSTEM_CFG_CGC_MODULES_NUM           ((uint8_t)0x5)
#define SYSTEM_CFG_USED_PORTS_NUM            ((uint8_t)0x6)

struct System_CfgSt
{
   struct System_ModuleClkCfg
   {
      System_CgcModuleType id;
      System_CgcArgType arg; 
   }module[SYSTEM_CFG_CGC_MODULES_NUM];
   System_GpioCgcPortType  gpioPort[SYSTEM_CFG_USED_PORTS_NUM];
};

extern const System_CfgType SystemCfg;
#endif /* MCAL_SYSTEM_SYSTEM_CFG_H_ */
