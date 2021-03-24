/*
 * Hmi_Fsm_ReadState.c
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
 *                            <Hmi_ReadState_RxEvHdl>
 * In : Hmi_FsmCtxRefType *ctxRef)
 * Out: Hmi_FsmCtxRefType  *ctxRef
 * return: None
 * description: idle state revieved data handler.
 ***************************************************************************/
static void Hmi_ReadState_RxEvHdl(Hmi_FsmCtxRefType *ctxRef);

/*************************************************************************
 *                            <Hmi_ReadState_TxEvHdl>
 * In : Hmi_FsmCtxRefType *ctxRef)
 * Out: Hmi_FsmCtxRefType  *ctxRef
 * return: None
 * description: idle state transmited data handler.
 ***************************************************************************/
static void Hmi_ReadState_TxEvHdl(Hmi_FsmCtxRefType *ctxRef);

/*************************************************************************
 *                            <Hmi_ReadState_TxEvHdl>
 * In : Hmi_FsmCtxRefType *ctxRef)
 * Out: Hmi_FsmCtxRefType  *ctxRef
 * return: None
 * description: idle state timeout event handler.
 ***************************************************************************/
static void Hmi_ReadState_TimeOutEvHdl(Hmi_FsmCtxRefType *ctxRef);

/*************************************************************************
 *                            <Hmi_ReadState_TxEvHdl>
 * In : Hmi_FsmCtxRefType *ctxRef)
 * Out: Hmi_FsmCtxRefType  *ctxRef
 * return: None
 * description: idle state new request event handler.
 ***************************************************************************/
static Std_ReturnType Hmi_ReadState_newReqEvHdl(Hmi_FsmCtxRefType *ctxRef);

/**
 * @brief Local Idle state class.
 * Used to handle the incoming events in the idle state.
 */
static Hmi_FsmCtxType Hmi_ReadState =\
{
   Hmi_ReadState_RxEvHdl, Hmi_ReadState_TxEvHdl, Hmi_ReadState_TimeOutEvHdl, Hmi_ReadState_newReqEvHdl,
   .timeOut = 0xFFFFFFFF
};

/*Global read request packet structure.*/
static Hmi_ComReadPacketType Hmi_readRequest =\
{
   .elem.header  = HMI_SCREEN_PACKET_HEADER_VAL,
   .elem.cmd     = HMI_SCREEN_SUCCESSIVE_READ_CMD_VAL,
   .elem.address = HMI_SCREEN_READ_PACKET_USER_DATA1_ADDRESS,
   .elem.length  = HMI_SCREEN_READ_PACKET_USER_DATA1_LENGTH,
   .elem.tail    = HMI_SCREEN_PACKET_TAIL_VAL
};

volatile static Hmi_ComReadResPacketType Hmi_readResponse;

/*****************************************************************************************
 *  Local Functions
 *****************************************************************************************/

/*************************************************************************
 *                            <Hmi_ReadState_RxEvHdl>
 * In : Hmi_FsmCtxRefType *ctxRef)
 * Out: Hmi_FsmCtxRefType  *ctxRef
 * return: None
 * description: idle state revieved data handler.
 ***************************************************************************/
static void Hmi_ReadState_RxEvHdl(Hmi_FsmCtxRefType *ctxRef)
{
   const uint16_t u8DataLength = Hmi_internalRealDataLength((*ctxRef)->msg.address, 
                                                            (*ctxRef)->msg.length);

   if(HMI_SCREEN_COMMAND_EXECUTED == *(uint16_t*)&Hmi_readResponse.pdu[HMI_SCREEN_CFG_COM_PACKET_STRUCT_LENGTH + u8DataLength])
   {
      Hmi_internalU8Copy((uint8_t*)&Hmi_readResponse.elem.appData[0],  /*Copy the application data*/
                         &(*ctxRef)->msg.dataPtr[0],
                         u8DataLength, TRUE);

      ((Hmi_ReadEndJobCbkType)(*ctxRef)->msg.appCbk)(HMI_SCREEN_NO_ERROR);
   }
   else
   {
      /*Notify the application with the NRC.*/
      ((Hmi_ReadEndJobCbkType)(*ctxRef)->msg.appCbk)(HMI_SCREEN_READ_ERROR);
   }

   Hmi_Fsm_Enter_IdleState(ctxRef);  /* > Reset the current state..
                                          Read -> Clear the Hw buffer -> setup the Dma -> Idle..*/     
}/*Hmi_ReadState_RxEvHdl*/

/*************************************************************************
 *                            <Hmi_ReadState_TxEvHdl>
 * In : Hmi_FsmCtxRefType *ctxRef)
 * Out: Hmi_FsmCtxRefType  *ctxRef
 * return: None
 * description: idle state transmited data handler.
 ***************************************************************************/
static void Hmi_ReadState_TxEvHdl(Hmi_FsmCtxRefType *ctxRef)
{
   const uint16_t u8DataLength = Hmi_internalRealDataLength((*ctxRef)->msg.address, (*ctxRef)->msg.length);

   Dma_Channel_Cfg_t rx_ch_cfg = {
      .src_end_address = HMI_SCREEN_CFG_UART_RX_BUFFER_ADDRESS,
      .des_end_address = (void *)&Hmi_readResponse.pdu[(HMI_SCREEN_CFG_COM_PACKET_STRUCT_LENGTH + u8DataLength + HMI_SCREEN_CONFIRMATION_LENGTH) - 1],
      .ctl = {1, 0, (HMI_SCREEN_CFG_COM_PACKET_STRUCT_LENGTH + u8DataLength + HMI_SCREEN_CONFIRMATION_LENGTH) - 1, 1, 0, 0, 3, 0, 0},
      .unused_spare = 0x00
   };
   
   (void)Dma_Setup_PrimaryChannel(HMI_SCREEN_CFG_DMA_RX_CH_ID,
                                  &rx_ch_cfg);
   
   (*ctxRef)->timeOut = HMI_SCREEN_CFG_READ_TIMEOUT_CYCLES; /*Wait for the incoming data..*/
}/*Hmi_ReadState_TxEvHdl*/

/*************************************************************************
 *                            <Hmi_ReadState_TxEvHdl>
 * In : Hmi_FsmCtxRefType *ctxRef)
 * Out: Hmi_FsmCtxRefType  *ctxRef
 * return: None
 * description: idle state timeout event handler.
 ***************************************************************************/
static void Hmi_ReadState_TimeOutEvHdl(Hmi_FsmCtxRefType *ctxRef)
{
   /*Notify the application with the NRC.*/
   ((Hmi_ReadEndJobCbkType)(*ctxRef)->msg.appCbk)(HMI_SCREEN_TIMEOUT_ERROR);

   Hmi_Fsm_Enter_IdleState(ctxRef);  /* > Reset the current state..
                                          Read -> Clear the Hw buffer -> setup the Dma -> Idle..*/
}/*Hmi_ReadState_TimeOutEvHdl*/

/*************************************************************************
 *                            <Hmi_ReadState_TxEvHdl>
 * In : Hmi_FsmCtxRefType *ctxRef)
 * Out: Hmi_FsmCtxRefType  *ctxRef
 * return: None
 * description: idle state new request event handler.
 ***************************************************************************/
static Std_ReturnType Hmi_ReadState_newReqEvHdl(Hmi_FsmCtxRefType *ctxRef)
{
   /*The new request will be rejected as there is an on going operation.*/
   return E_NOT_OK;
}/*Hmi_ReadState_newReqEvHdl*/

/****************************************************************************************
 *       Global Functions.
 ****************************************************************************************/

/*************************************************************************
 *                            <Hmi_Fsm_Enter_ReadState>
 * In : Hmi_FsmCtxRefType  *ctxRef
 * Out: Hmi_FsmCtxRefType  *ctxRef
 * return: Std_ReturnType
 * description: Enter the read state.
 ***************************************************************************/
Std_ReturnType Hmi_Fsm_Enter_ReadState(Hmi_FsmCtxRefType *ctxRef)
{
   Std_ReturnType result = E_NOT_OK;
   Hmi_FsmCtxRefType lastCtxRef = *ctxRef;
   Dma_Channel_Cfg_t tx_ch_cfg =\
   {
      .src_end_address = &Hmi_readRequest.pdu[HMI_SCREEN_SUCCESSIVE_READ_PACKET_SIZE - 1],
      .des_end_address = HMI_SCREEN_CFG_UART_TX_BUFFER_ADDRESS,
      .ctl = {1, 0, HMI_SCREEN_SUCCESSIVE_READ_PACKET_SIZE - 1, 1, 0, 0, 0, 0, 3},
      .unused_spare = 0x00
   };
   
   /**
    * TODO: Check if the passed parameters are valid..
    */
   if(NULL_PTR != ctxRef)
   {
      *ctxRef = &Hmi_ReadState; /*Initialize the ctxRef with ReadState ctx.*/
      (*ctxRef)->msg = lastCtxRef->msg;

      /*Update the request packet with the new request info.*/
      HMI_U32_ADDRESS_LIT2BIG_ENDIAN(&(*ctxRef)->msg.address, 
                                     &Hmi_readRequest.elem.address);

      Hmi_readRequest.elem.length = (*ctxRef)->msg.length;

      /*Setup the Dma channel.*/
      result = Dma_Setup_PrimaryChannel(HMI_SCREEN_CFG_DMA_TX_CH_ID,
                                        &tx_ch_cfg);
                                           
      if(E_OK != result) /*if the job is not accepted.*/
      {
         Hmi_Fsm_Enter_IdleState(ctxRef); /*Enter to the Idle State.*/
      }
      else
      {
         (*ctxRef)->timeOut = HMI_SCREEN_CFG_READ_TIMEOUT_CYCLES; /*Wait for the incoming data..*/
      }
   }

   return result;
}/*Hmi_Fsm_Enter_ReadState*/




