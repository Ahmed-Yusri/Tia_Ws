/*
 * Hmi_Fsm_IdleState.c
 *
 *  Created on: Jan 25, 2021
 *      Author: Ahmed Yusri Mohamed
 */

#include "Hmi_Priv_Fsm.h"

/**
 * @brief Local Idle state handlers.
 * 
 * @param msg 
 * @param ctxRef 
 */

/*************************************************************************
 *                            <Hmi_IdleState_RxEvHdl>
 * In : Hmi_FsmCtxRefType *ctxRef)
 * Out: Hmi_FsmCtxRefType  *ctxRef
 * return: None
 * description: idle state revieved data handler.
 ***************************************************************************/
static void Hmi_IdleState_RxEvHdl(Hmi_FsmCtxRefType *ctxRef);

/*************************************************************************
 *                            <Hmi_IdleState_TxEvHdl>
 * In : Hmi_FsmCtxRefType *ctxRef)
 * Out: Hmi_FsmCtxRefType  *ctxRef
 * return: None
 * description: idle state transmited data handler.
 ***************************************************************************/
static void Hmi_IdleState_TxEvHdl(Hmi_FsmCtxRefType *ctxRef);

/*************************************************************************
 *                            <Hmi_IdleState_TxEvHdl>
 * In : Hmi_FsmCtxRefType *ctxRef)
 * Out: Hmi_FsmCtxRefType  *ctxRef
 * return: None
 * description: idle state timeout event handler.
 ***************************************************************************/
static void Hmi_IdleState_TimeOutEvHdl(Hmi_FsmCtxRefType *ctxRef);

/*************************************************************************
 *                            <Hmi_IdleState_TxEvHdl>
 * In : Hmi_FsmCtxRefType *ctxRef)
 * Out: Hmi_FsmCtxRefType  *ctxRef
 * return: None
 * description: idle state new request event handler.
 ***************************************************************************/
static Std_ReturnType Hmi_IdleState_newReqEvHdl(Hmi_FsmCtxRefType *ctxRef);

/**
 * @brief Local Idle state class.
 * Used to handle the incoming events in the idle state.
 */
static Hmi_FsmCtxType Hmi_IdleState =\
{
   Hmi_IdleState_RxEvHdl, Hmi_IdleState_TxEvHdl, Hmi_IdleState_TimeOutEvHdl, Hmi_IdleState_newReqEvHdl,
   .timeOut = 0xFFFFFFFF
};

enum
{
   E_HMI_IDLE_WAITING_FOR_TOUCH_RESPONSE,
   E_HMI_IDLE_WAITING_FOR_THE_COMPLETE_TOUCH_RESPONSE
}Hmi_idleStateInternalState = E_HMI_IDLE_WAITING_FOR_TOUCH_RESPONSE;

static Hmi_GenericNfcRespType Hmi_nfcResp;

/*****************************************************************************************
 *  Local Functions
 *****************************************************************************************/

/*************************************************************************
 *                            <Hmi_SetupNfcJob>
 * In : None
 * Out: None
 * return: Std_ReturnType
 * description: setup the dma channel to collect the rx data 
 *              'Mainly the touch response/Nfc Jobs in general'.
 ***************************************************************************/
static inline Std_ReturnType Hmi_SetupNfcJob(uint8_t *buffer, uint8_t length)
{
   Std_ReturnType result = E_NOT_OK;

   Dma_Channel_Cfg_t rx_ch_cfg = {
      .src_end_address = HMI_SCREEN_CFG_UART_RX_BUFFER_ADDRESS,
      .des_end_address = &buffer[length- 1],
      .ctl = {1, 0, length - 1, 1, 0, 0, 3, 0, 0},
      .unused_spare = 0x00
   };

   if(E_DMA_IDLE == Dma_getChannelState(HMI_SCREEN_CFG_DMA_RX_CH_ID))   /*the setup will be allowed only if the Dma is in idle state.*/
   {
      result = Dma_Setup_PrimaryChannel(HMI_SCREEN_CFG_DMA_RX_CH_ID,
                                       &rx_ch_cfg);
   }

   return result;
}/*Hmi_SetupNfcJob*/

/*************************************************************************
 *                            <Hmi_IdleState_RxEvHdl>
 * In : Hmi_FsmCtxRefType *ctxRef)
 * Out: Hmi_FsmCtxRefType  *ctxRef
 * return: None
 * description: idle state revieved data handler.
 ***************************************************************************/
static void Hmi_IdleState_RxEvHdl(Hmi_FsmCtxRefType *ctxRef)
{
   Hmi_FsmCtxRefType ctx = *ctxRef;
   Hmi_ScreenCfgNfcJobRefType NfcJobsCfg = (Hmi_ScreenCfgNfcJobRefType)ctx->msg.dataPtr;
   boolean goodCommand = FALSE;
   Hmi_AddressType address;
   uint8_t jobIdx;
   uint8_t localBuffer[8];
   uint16_t u8DataLength;

   if(E_HMI_IDLE_WAITING_FOR_TOUCH_RESPONSE == Hmi_idleStateInternalState)
   {
      for(jobIdx = 0; jobIdx < HMI_SCREEN_CFG_NUM_NFC_JOBS; ++jobIdx)
      {
         if(NfcJobsCfg[jobIdx].cmd == Hmi_nfcResp.elem.cmd)
         {
            Hmi_nfcResp.elem.cmdIdx = jobIdx;
            goodCommand = TRUE;
            break;
         }
      }

      if(goodCommand)
      {
         if(HMI_SCREEN_CFG_MIN_NFC_RESP_LENGTH < NfcJobsCfg[jobIdx].cmdLength)
         {
            Hmi_idleStateInternalState = E_HMI_IDLE_WAITING_FOR_THE_COMPLETE_TOUCH_RESPONSE;

            (void)Hmi_SetupNfcJob(&Hmi_nfcResp.pdu[HMI_SCREEN_CFG_MIN_NFC_RESP_LENGTH],  /*Wait for the Hmi complete notification.*/
                                  NfcJobsCfg[jobIdx].cmdLength - HMI_SCREEN_CFG_MIN_NFC_RESP_LENGTH);
         }
#if 0
TODO: Support the rest of command parsing
         else
         {
            if(TRUE == NfcJobsCfg[Hmi_nfcResp.elem.cmdIdx].subscribe)
            {
               HMI_U32_ADDRESS_LIT2BIG_ENDIAN(&Hmi_nfcResp.pdu[2], /*Rotate the address bytes.*/
                                              &address);

               ((Hmi_NfcEndJobCbkType)ctx->msg.appCbk)(address,   /*Notify the application with the revieved data.*/
                                                      &Hmi_nfcResp.pdu[6]);
            }
         }
#endif
      }
      else
      {
         Hmi_Fsm_Enter_IdleState(ctxRef);  /* > Reset the current state..
                                                Idle -> Clear the Hw buffer -> setup the Dma -> Idle..*/
      }
   }
   else
   {
      if(TRUE == NfcJobsCfg[Hmi_nfcResp.elem.cmdIdx].subscribe)
      {
         HMI_U32_ADDRESS_LIT2BIG_ENDIAN(&Hmi_nfcResp.pdu[2], /*Rotate the address Indian.*/
                                        &address);

         u8DataLength = Hmi_internalRealDataLength(address, 1);

         Hmi_internalU8Copy((uint8_t*)&Hmi_nfcResp.pdu[6],  /*Copy the application data*/
                            &localBuffer[0],
                            u8DataLength, TRUE);

         ((Hmi_NfcEndJobCbkType)ctx->msg.appCbk)(address,   /*Notify the application with the received data.*/
                                                 &localBuffer[0]);
      }

      Hmi_idleStateInternalState = E_HMI_IDLE_WAITING_FOR_TOUCH_RESPONSE;  /*Chnage the driver state. */
   }

   if(E_HMI_IDLE_WAITING_FOR_TOUCH_RESPONSE == Hmi_idleStateInternalState) /*Check if we need to setup the dma connection..*/
   {
      (void)Hmi_SetupNfcJob(&Hmi_nfcResp.pdu[0],  /*Wait for the Hmi notifications.*/
                            HMI_SCREEN_CFG_MIN_NFC_RESP_LENGTH);
   }
}/*Hmi_IdleState_RxEvHdl*/

/*************************************************************************
 *                            <Hmi_IdleState_TxEvHdl>
 * In : Hmi_FsmCtxRefType *ctxRef)
 * Out: Hmi_FsmCtxRefType  *ctxRef
 * return: None
 * description: idle state transmited data handler.
 ***************************************************************************/
static void Hmi_IdleState_TxEvHdl(Hmi_FsmCtxRefType *ctxRef)
{
   while(1);   /*Dev Code TODO: to be removed.*/

}/*Hmi_IdleState_TxEvHdl*/

/*************************************************************************
 *                            <Hmi_IdleState_TxEvHdl>
 * In : Hmi_FsmCtxRefType *ctxRef)
 * Out: Hmi_FsmCtxRefType  *ctxRef
 * return: None
 * description: idle state timeout event handler.
 ***************************************************************************/
static void Hmi_IdleState_TimeOutEvHdl(Hmi_FsmCtxRefType *ctxRef)
{
  Hmi_Fsm_Enter_IdleState(ctxRef);  /* > Reset the current state..
                                         Idle -> Clear the Hw buffer -> setup the Dma -> Idle..*/
}/*Hmi_IdleState_TimeOutEvHdl*/

/*************************************************************************
 *                            <Hmi_IdleState_TxEvHdl>
 * In : Hmi_FsmCtxRefType *ctxRef
 * Out: Hmi_FsmCtxRefType  *ctxRef
 * return: None
 * description: idle state new request event handler.
 ***************************************************************************/
static Std_ReturnType Hmi_IdleState_newReqEvHdl(Hmi_FsmCtxRefType *ctxRef)
{
   Std_ReturnType result = E_NOT_OK;

   if(HMI_READ_REQUEST_ID == (*ctxRef)->msg.requestId)
   {
      result = Hmi_Fsm_Enter_ReadState(ctxRef);
   }
   else if(HMI_WRITE_REQUEST_ID == (*ctxRef)->msg.requestId)
   {
      result = Hmi_Fsm_Enter_WriteState(ctxRef);
   }
   else
   {
      /*Undefined request id return E_NOT_OK */
   }
   
   return result;
}/*Hmi_IdleState_newReqEvHdl*/

/****************************************************************************************
 *       Global Functions.
 ****************************************************************************************/

/*************************************************************************
 *                            <Hmi_Fsm_Enter_IdleState>
 * In : Hmi_FsmCtxRefType  *ctxRef
 * Out: Hmi_FsmCtxRefType  *ctxRef
 * return: None
 * description: Init the internal HMI state machine.
 ***************************************************************************/
void Hmi_Fsm_Enter_IdleState(Hmi_FsmCtxRefType *ctxRef)
{
   if(NULL_PTR != ctxRef)
   {
      *ctxRef = &Hmi_IdleState; /*Initialize the ctxRef with idleState ctx.*/
      
      /*Fill the idle msg content. -> "Idle state Only"*/
      (*ctxRef)->msg.dataPtr = HMI_SCREEN_CFG_GET_NFC_COMMAND_TBL_ADDRESS();
      (*ctxRef)->msg.appCbk  = HMI_SCREEN_CFG_GET_NFC_APP_CBK_ADDRESS();

      (*ctxRef)->timeOut = HMI_SCREEN_CFG_IDLE_TIMEOUT_CYCLES;  /*reset the state in case of no user intervention.*/

      Serial_Flush(HMI_SCREEN_CFG_UART_MODULE_ID); /*Flush the uart channel..*/

      (void)Hmi_SetupNfcJob(&Hmi_nfcResp.pdu[0],  /*Wait for the Hmi notifications.*/
                            HMI_SCREEN_CFG_MIN_NFC_RESP_LENGTH);
   }
}/*Hmi_Fsm_Enter_IdleState*/




