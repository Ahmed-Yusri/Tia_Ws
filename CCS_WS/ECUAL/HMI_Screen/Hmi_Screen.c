/*
 * Hmi_Screen.c
 *
 *  Created on: Jan 22, 2021
 *      Author: Ahmed Yusri Mohamed
 */

#include "system.h"
#include "Hmi_Priv_Fsm.h"
#include "Hmi_Screen_Cfg.h"

/*****************************************************************
 *          Local VARs 
 *****************************************************************/

static struct /*Global information*/
{
   Hmi_FsmCtxRefType FsmCtx;
   const Hmi_ScreenCfgType *cfgPtr;
}Hmi_globalInfo =\
{
   .FsmCtx = NULL_PTR
};


/****************************************************************************************
 *       Local  Functions.
 ****************************************************************************************/

/****************************************************************************************
 *       Global Functions.
 ****************************************************************************************/

/*************************************************************************
 *                            <Hmi_Init>
 * In : void
 * Out: None
 * return: Std_ReturnType
 * description: Init the Macl resourses for the Hmi screen.
 ***************************************************************************/
Std_ReturnType Hmi_Init(const Hmi_ScreenCfgType *Hmi_CfgPtr)
{
   Std_ReturnType result = E_OK;

   if(NULL_PTR == Hmi_globalInfo.FsmCtx)
   {
      Hmi_globalInfo.cfgPtr = Hmi_CfgPtr; /*capture the configuration parameters.*/

      /*Perform the needed Hw intialization..*/
      Serial_Init(HMI_SCREEN_CFG_UART_MODULE_ID, /*Intialize the Uart channel*/
                  HMI_SCREEN_CFG_UART_BAUDERATE, 
                  HMI_SCREEN_CFG_UART_PARITYBIT);  

      /*Setup the Uart Rx Hw buffer size..*/
      (void)Serial_setupRxHwBufferSize(HMI_SCREEN_CFG_UART_MODULE_ID, 2);

      /*Setup the Dma connection and interrupts.*/
      (void)Serial_setDmaConnection(HMI_SCREEN_CFG_UART_MODULE_ID);
      
      NVIC_Set_Interrupt_Priority(HMI_SCREEN_CFG_UART_INTERRUPT_ID,
                                  HMI_SCREEN_CFG_UART_INTERRUPT_PRIORITY);

      /*Enable the Uart interrupt..*/
      (void)NIVC_InterruptSetup(HMI_SCREEN_CFG_UART_INTERRUPT_ID);  

      Hmi_Fsm_Enter_IdleState(&Hmi_globalInfo.FsmCtx); /*Enter to the idle state.*/
   }
   else
   {
      result = E_NOT_OK; /*Module is already initialized.*/
   }
   
   return result;   
}/*Hmi_Init*/

/*************************************************************************
 *                         <Hmi_ReadMemoryRange>
 * In : void
 * Out: None
 * return: Std_ReturnType
 * description: Read a specific memory range.
 ***************************************************************************/
Std_ReturnType Hmi_ReadMemoryRange(uint8_t *desPtr, Hmi_AddressType address, Hmi_LengthType length)
{
   Std_ReturnType result = E_OK;

#ifdef IN_DEV_MODE
   if((desPtr == NULL_PTR) ||
      (NULL_PTR == appCbk))
   {
      result = E_NOT_OK;
   }
   else
#endif
   {
      /*Marshal the application request.*/
      Hmi_globalInfo.FsmCtx->msg.dataPtr = desPtr; 
      Hmi_globalInfo.FsmCtx->msg.length = length;
      Hmi_globalInfo.FsmCtx->msg.address = address;
      Hmi_globalInfo.FsmCtx->msg.appCbk = (void *)Hmi_globalInfo.cfgPtr->readJobEndCbk;
      Hmi_globalInfo.FsmCtx->msg.requestId = HMI_READ_REQUEST_ID;

      result = Hmi_globalInfo.FsmCtx->newReqEv(&Hmi_globalInfo.FsmCtx);
   }

   return result;
}/*Hmi_ReadMemoryRange*/

/*************************************************************************
 *                         <Hmi_WriteMemoryRange>
 * In : void
 * Out: None
 * return: Std_ReturnType
 * description: write a specific memory range.
 ***************************************************************************/
Std_ReturnType Hmi_WriteMemoryRange(uint8_t *srcPtr, Hmi_AddressType address, Hmi_LengthType length)
{
   Std_ReturnType result = E_OK;

#ifdef IN_DEV_MODE
   if((srcPtr == NULL_PTR) ||
      (NULL_PTR == appCbk))
   {
      result = E_NOT_OK;
   }
   else
#endif
   {
      /*Marshal the application request.*/
      Hmi_globalInfo.FsmCtx->msg.dataPtr = srcPtr; 
      Hmi_globalInfo.FsmCtx->msg.length = length;
      Hmi_globalInfo.FsmCtx->msg.address = address;
      Hmi_globalInfo.FsmCtx->msg.appCbk = (void *)Hmi_globalInfo.cfgPtr->writeJobEndCbk;
      Hmi_globalInfo.FsmCtx->msg.requestId = HMI_WRITE_REQUEST_ID;
      
      result = Hmi_globalInfo.FsmCtx->newReqEv(&Hmi_globalInfo.FsmCtx);
   }

   return result;
}/*Hmi_WriteMemoryRange*/

void Hmi_UART4_ISR(void)
{
   uint32_t UartInterruptStatus = Serial_getInterruptState(HMI_SCREEN_CFG_UART_MODULE_ID); /*Capture the current UART interrupt state..*/
   
   Serial_clearAllInterrupt(HMI_SCREEN_CFG_UART_MODULE_ID); /*Clear the interrupt flag.*/ 

   if(UartInterruptStatus & (1 << UART_RECEIVE_DMA_RAW_INTERRUPT_STATUS_BIT_NUM))
   {
      Hmi_globalInfo.FsmCtx->rxEv(&Hmi_globalInfo.FsmCtx);  /*Trigger the rx event handler of the current context state*/
   }

   if(UartInterruptStatus & (1 << UART_TRANSMIT_RAW_INTERRUPT_STATUS_BIT_NUM))
   {
      Hmi_globalInfo.FsmCtx->txEv(&Hmi_globalInfo.FsmCtx);  /*Trigger the tx event handler of the current context state*/
   }

}/*Hmi_UART4_ISR*/
