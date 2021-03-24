/*
 * System_Cfg.c
 *
 *  Created on: Jan 19, 2021
 *      Author: Ahmed Yusri Mohamed
 */

#include "System_Cfg.h"

const System_CfgType SystemCfg =\
{
   {
      {PWM_CLOCK_GATING_ID,(System_CgcArgType)0x00}, 
      {DMA_CLOCK_GATING_ID, (System_CgcArgType)0x00}, 
      {UART_CLOCK_GATING_ID, (System_CgcArgType)UART0CGC},
      {UART_CLOCK_GATING_ID, (System_CgcArgType)UART4CGC},
      {EEPROM_CLOCK_GATING_ID, 0x0}
   },
   {
      E_GPIO_CGC_PORT_N, E_GPIO_CGC_PORT_B, E_GPIO_CGC_PORT_J, E_GPIO_CGC_PORT_F, E_GPIO_CGC_PORT_A, E_GPIO_CGC_PORT_K
   }
};
