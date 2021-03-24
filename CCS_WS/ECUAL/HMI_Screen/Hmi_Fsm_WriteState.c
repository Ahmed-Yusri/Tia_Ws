/*
 * Hmi_Fsm_WriteState.c
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
 *                            <Hmi_WriteState_RxEvHdl>
 * In : Hmi_FsmCtxRefType *ctxRef)
 * Out: Hmi_FsmCtxRefType  *ctxRef
 * return: None
 * description: idle state revieved data handler.
 ***************************************************************************/
static void Hmi_WriteState_RxEvHdl(Hmi_FsmCtxRefType *ctxRef);

/*************************************************************************
 *                            <Hmi_WriteState_TxEvHdl>
 * In : Hmi_FsmCtxRefType *ctxRef)
 * Out: Hmi_FsmCtxRefType  *ctxRef
 * return: None
 * description: idle state transmited data handler.
 ***************************************************************************/
static void Hmi_WriteState_TxEvHdl(Hmi_FsmCtxRefType *ctxRef);

/*************************************************************************
 *                            <Hmi_WriteState_TxEvHdl>
 * In : Hmi_FsmCtxRefType *ctxRef)
 * Out: Hmi_FsmCtxRefType  *ctxRef
 * return: None
 * description: idle state timeout event handler.
 ***************************************************************************/
static void Hmi_WriteState_TimeOutEvHdl(Hmi_FsmCtxRefType *ctxRef);

/*************************************************************************
 *                            <Hmi_WriteState_TxEvHdl>
 * In : Hmi_FsmCtxRefType *ctxRef)
 * Out: Hmi_FsmCtxRefType  *ctxRef
 * return: None
 * description: idle state new request event handler.
 ***************************************************************************/
static Std_ReturnType Hmi_WriteState_newReqEvHdl(Hmi_FsmCtxRefType *ctxRef);

/**
 * @brief Local Idle state class.
 * Used to handle the incoming events in the idle state.
 */
static Hmi_FsmCtxType Hmi_WriteState =\
{
   Hmi_WriteState_RxEvHdl, Hmi_WriteState_TxEvHdl, Hmi_WriteState_TimeOutEvHdl, Hmi_WriteState_newReqEvHdl,
   .timeOut = 0xFFFFFFFF
};

volatile static union writeResponseUn
{
   uint8_t u8Response[2];
   uint16_t u16Response;
}writeResponse __attribute__ ((aligned(2)));

/*Global write request packet structure.*/
static Hmi_ComWritePacketType Hmi_writeRequest =\
{
   .elem.header  = HMI_SCREEN_PACKET_HEADER_VAL,
   .elem.cmd     = HMI_SCREEN_SUCCESSIVE_WRITE_CMD_VAL,
   .elem.tail    = HMI_SCREEN_PACKET_TAIL_VAL
};
/*****************************************************************************************
 *  Local Functions
 *****************************************************************************************/

/*************************************************************************
 *                            <Hmi_SetupStartOfRxData>
 * In : None
 * Out: None
 * return: Std_ReturnType
 * description: setup the dma channel to collect the rx data.
 ***************************************************************************/
static inline void Hmi_SetupWriteResponse(void)
{
   Dma_Channel_Cfg_t rx_ch_cfg = {
      .src_end_address = HMI_SCREEN_CFG_UART_RX_BUFFER_ADDRESS,
      .des_end_address = (void *)&writeResponse.u8Response[1],
      .ctl = {1, 0, 1, 1, 0, 0, 3, 0, 0},
      .unused_spare = 0x00
   };

   (void)Dma_Setup_PrimaryChannel(HMI_SCREEN_CFG_DMA_RX_CH_ID,
                                  &rx_ch_cfg);
}/*Hmi_SetupStartOfRxData*/

/*************************************************************************
 *                            <Hmi_WriteState_RxEvHdl>
 * In : Hmi_FsmCtxRefType *ctxRef)
 * Out: Hmi_FsmCtxRefType  *ctxRef
 * return: None
 * description: idle state revieved data handler.
 ***************************************************************************/
static void Hmi_WriteState_RxEvHdl(Hmi_FsmCtxRefType *ctxRef)
{
   if(HMI_SCREEN_COMMAND_EXECUTED == writeResponse.u16Response)
   {
      ((Hmi_WriteEndJobCbkType)(*ctxRef)->msg.appCbk)(HMI_SCREEN_NO_ERROR);    /*Notify the application with a positive response.*/
   }
   else
   {
      ((Hmi_WriteEndJobCbkType)(*ctxRef)->msg.appCbk)(HMI_SCREEN_WRITE_ERROR); /*Notify the application with the NRC.*/
   }

   Hmi_Fsm_Enter_IdleState(ctxRef);  /* > Reset the current state..
                                          Write -> Clear the Hw buffer -> setup the Dma -> Idle..*/
}/*Hmi_WriteState_RxEvHdl*/

/*************************************************************************
 *                            <Hmi_WriteState_TxEvHdl>
 * In : Hmi_FsmCtxRefType *ctxRef)
 * Out: Hmi_FsmCtxRefType  *ctxRef
 * return: None
 * description: idle state transmited data handler.
 ***************************************************************************/
static void Hmi_WriteState_TxEvHdl(Hmi_FsmCtxRefType *ctxRef)
{
   Hmi_SetupWriteResponse(); /*Setup the Dma rx channel 
                                 wait for the Hmi confirmation.*/

   (*ctxRef)->timeOut = HMI_SCREEN_CFG_WRITE_TIMEOUT_CYCLES; /*Wait for the incoming data..*/
}/*Hmi_WriteState_TxEvHdl*/

/*************************************************************************
 *                            <Hmi_WriteState_TxEvHdl>
 * In : Hmi_FsmCtxRefType *ctxRef)
 * Out: Hmi_FsmCtxRefType  *ctxRef
 * return: None
 * description: idle state timeout event handler.
 ***************************************************************************/
static void Hmi_WriteState_TimeOutEvHdl(Hmi_FsmCtxRefType *ctxRef)
{
   /*Notify the application with the NRC.*/
   ((Hmi_WriteEndJobCbkType)(*ctxRef)->msg.appCbk)(HMI_SCREEN_TIMEOUT_ERROR);

   Hmi_Fsm_Enter_IdleState(ctxRef);  /* > Reset the current state..
                                          Write -> Clear the Hw buffer -> setup the Dma -> Idle..*/
}/*Hmi_WriteState_TimeOutEvHdl*/

/*************************************************************************
 *                            <Hmi_WriteState_TxEvHdl>
 * In : Hmi_FsmCtxRefType *ctxRef)
 * Out: Hmi_FsmCtxRefType  *ctxRef
 * return: None
 * description: idle state new request event handler.
 ***************************************************************************/
static Std_ReturnType Hmi_WriteState_newReqEvHdl(Hmi_FsmCtxRefType *ctxRef)
{
   /*The new request will be rejected as there is an on going operation.*/
   return E_NOT_OK;
}/*Hmi_WriteState_newReqEvHdl*/

/****************************************************************************************
 *       Global Functions.
 ****************************************************************************************/

/*************************************************************************
 *                            <Hmi_Fsm_Enter_WriteState>
 * In : Hmi_FsmCtxRefType  *ctxRef, Hmi_MsgCtxType *msg
 * Out: Hmi_FsmCtxRefType  *ctxRef
 * return: Std_ReturnType
 * description: Enter the read state.
 ***************************************************************************/
Std_ReturnType Hmi_Fsm_Enter_WriteState(Hmi_FsmCtxRefType *ctxRef)
{
   Std_ReturnType result = E_NOT_OK;
   Hmi_FsmCtxRefType lastCtxRef = *ctxRef;
   const uint16_t u8DataLength = Hmi_internalRealDataLength((*ctxRef)->msg.address, (*ctxRef)->msg.length);
   Dma_Channel_Cfg_t tx_ch_cfg =\
   {
      .src_end_address = &Hmi_writeRequest.pdu[(HMI_SCREEN_CFG_COM_PACKET_STRUCT_LENGTH + u8DataLength) - 1],
      .des_end_address = HMI_SCREEN_CFG_UART_TX_BUFFER_ADDRESS,
      .ctl = {1, 0, (HMI_SCREEN_CFG_COM_PACKET_STRUCT_LENGTH + u8DataLength) - 1, 1, 0, 0, 0, 0, 3},
      .unused_spare = 0x00
   };

   /**
    * TODO: Check if the passed parameters are valid.. 
    */
   if(NULL_PTR != ctxRef)
   {
      *ctxRef = &Hmi_WriteState; /*Initialize the ctxRef with WriteState ctx.*/
      (*ctxRef)->msg = lastCtxRef->msg;
      
      (*ctxRef)->timeOut = HMI_SCREEN_CFG_WRITE_TIMEOUT_CYCLES;

      /*Update the request packet with the new request info.*/
      HMI_U32_ADDRESS_LIT2BIG_ENDIAN(&(*ctxRef)->msg.address, 
                                     &Hmi_writeRequest.elem.address);

      Hmi_writeRequest.elem.length = (*ctxRef)->msg.length;

      Hmi_internalU8Copy(&(*ctxRef)->msg.dataPtr[0],  /*Copy the application data*/
                         &Hmi_writeRequest.elem.appData[0], 
                         u8DataLength, TRUE);

      Hmi_internalU8Copy((uint8_t*)&Hmi_writeRequest.elem.tail,  /*Copy the communication packet tail..*/
                            &Hmi_writeRequest.elem.appData[u8DataLength], 
                            sizeof(Hmi_PacketTailType), FALSE);

      /*Setup the Dma tx channel.*/
      result = Dma_Setup_PrimaryChannel(HMI_SCREEN_CFG_DMA_TX_CH_ID,
                                        &tx_ch_cfg);
                                           
      if(E_OK != result) /*if the job is not accepted.*/
      {
         Hmi_Fsm_Enter_IdleState(ctxRef); /*Enter to the Idle State.*/
      }
   }

   return result;
}/*Hmi_Fsm_Enter_WriteState*/

