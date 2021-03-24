/************************************************************
 * Date: 07/21/2020.
 * Author: Ahmed Yusri Mohammed.
 * Project: Design Pattern:  'Demo'.
 *          State Pattern in C.
 ***********************************************************/

#include "System_Cfg.h"
#include "system.h"
#include "Port.h"
#include "Uart.h"
#include "OS.h"
#include "Eep.h"
#include "SchM_Eep.h"
#include "Buttons.h"
#include "Pwm.h"

#include "Hmi_Screen_Cfg.h"
#include "Dma.h"

void main()
{
   StartOS(0); /*This function call shall not be returned to the caller..*/
}

/*
 * Os Startup Hook.
 **/
void MCU_Init(void)
{
   uint32 tempBuffer[5] = {0xDEADBEAF, 0xDEADBEAF, 0xDEADBEAF, 0xDEADBEAF, 0xDEADBEAF};

   System_Init(&SystemCfg);   /*Initialize the clock gating..*/

   Port_Init((const Port_ConfigType*)&Port_PinCfg); /*Initialize the Port Driver.*/
   
   (void)Dma_Init(&Dma_Cfg);

   Pwm_Init(PWM_MODULE0_CH02, 10000);
   
   Serial_Init(UART_CHANNEL0_ID, 9600, E_UART_EVEN_PARITY_BIT);
   
   (void)Hmi_Init(&Hmi_ScreanCfg);
   
   Eep_Init(&Eep_Cfg);

   Eep_Read(0, (uint8*)tempBuffer, 16);

   while(Eep_GetJobResult() == MEMIF_JOB_PENDING)
   {
      Eep_MainFunction();
   }

   RemoteLoader(tempBuffer);
}/*MCU_Init*/
