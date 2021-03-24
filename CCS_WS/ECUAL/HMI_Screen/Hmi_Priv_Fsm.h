/*
 * Hmi_Priv_Fsm.h
 *
 *  Created on: Jan 25, 2021
 *      Author: Ahmed Yusri Mohamed.
 */

#ifndef ECUAL_HMI_SCREEN_HMI_PRIV_FSM_H_
#define ECUAL_HMI_SCREEN_HMI_PRIV_FSM_H_

#include "Hmi_Screen_Cfg.h"

/*Hmi_AppRequestType*/
#define HMI_WRITE_REQUEST_ID           ((Hmi_AppRequestType)0X01)
#define HMI_READ_REQUEST_ID            ((Hmi_AppRequestType)0X02)

#define HMI_U32_ADDRESS_LIT2BIG_ENDIAN(SRC, DES)\
((uint8_t *)(DES))[0] = ((uint8_t *)(SRC))[3];\
((uint8_t *)(DES))[1] = ((uint8_t *)(SRC))[2];\
((uint8_t *)(DES))[2] = ((uint8_t *)(SRC))[1];\
((uint8_t *)(DES))[3] = ((uint8_t *)(SRC))[0]

typedef union
{
   struct
   {
      Hmi_PacketHeaderType header;
      Hmi_CmdType cmd;
      Hmi_AddressType address;
      Hmi_LengthType length;
      Hmi_PacketTailType tail;
   }__attribute__((packed)) elem;
   uint8 pdu[HMI_SCREEN_SUCCESSIVE_READ_PACKET_SIZE];
}Hmi_ComReadPacketType;

typedef union
{
   struct
   {
      Hmi_PacketHeaderType header;
      Hmi_CmdType cmd;
      Hmi_AddressType address;
      Hmi_LengthType length;
      uint8_t appData[HMI_SCREEN_CFG_WRITE_PACKET_DATA_LENGTH];
      Hmi_PacketTailType tail;
   }__attribute__((packed)) elem;
   uint8 pdu[HMI_SCREEN_SUCCESSIVE_WRITE_PACKET_SIZE];
}Hmi_ComWritePacketType;

typedef union
{
   struct
   {
      Hmi_PacketHeaderType header;
      Hmi_CmdType cmd;
      Hmi_AddressType address;
      Hmi_LengthType length;
      uint8_t appData[HMI_SCREEN_CFG_READ_PACKET_DATA_LENGTH];
      Hmi_PacketTailType tail;
      uint16_t confirmation;
   }__attribute__((packed)) elem;
   uint8 pdu[HMI_SCREEN_CFGSUCCESSIVE_READ_PACKET_SIZE];
}Hmi_ComReadResPacketType;

typedef uint8 Hmi_PacketAckType[2];

typedef union
{
   struct
   {
      Hmi_PacketHeaderType header;
      Hmi_CmdType cmd;
      uint8 dataTail[HMI_SCREEN_CFG_MAX_NFC_RESP_LENGTH - 2];
      uint8_t cmdIdx;
   }__attribute__((packed)) elem;
   uint8 pdu[HMI_SCREEN_CFG_MAX_NFC_RESP_LENGTH];
}Hmi_GenericNfcRespType;

typedef uint8_t Hmi_AppRequestType;

/**
 * state Ctx types.
 */

typedef struct
{
   void *appCbk;
   uint8_t *dataPtr;
   Hmi_AddressType address;
   Hmi_LengthType length;
   Hmi_AppRequestType requestId;
}Hmi_MsgCtxType;





/**
 * State interface/abstract class.
 */
typedef struct Hmi_FsmCtxSt *Hmi_FsmCtxRefType;

typedef struct Hmi_FsmCtxSt
{
   void (* const rxEv)(Hmi_FsmCtxRefType *);
   void (* const txEv)(Hmi_FsmCtxRefType *);
   void (* const timeOutEv)(Hmi_FsmCtxRefType *);
   Std_ReturnType (* const newReqEv)(Hmi_FsmCtxRefType *);
   Hmi_MsgCtxType msg;
   uint32_t timeOut;
}Hmi_FsmCtxType;

typedef Std_ReturnType (*Hmi_FsmHandlerType)(Hmi_FsmCtxRefType *);


static inline void Hmi_internalU8Copy(uint8_t *srcPtr, uint8_t *desPtr, uint16_t length, boolean rotate)
{
   uint8_t dataIdx;

   if(TRUE == rotate)
   {
     for(dataIdx = 0; dataIdx < length; ++dataIdx)
      {
         desPtr[dataIdx] = srcPtr[length - dataIdx - 1];
      }
   }
   else
   {
      while(length--)
      {
         desPtr[length] = srcPtr[length];
      }
   }
}/*Hmi_internalU8Copy*/

static inline  uint16_t Hmi_internalRealDataLength(Hmi_AddressType address, Hmi_LengthType length)
{
   uint16_t u8Datalength = length;

   /*TODO: REMOVE THE MAGIC NUMBERS..*/
   if((0x80000 <= address) && 
      (0x90000 > address))
   {
      u8Datalength *= 2;
   }
   else if((0x20000 <= address) && 
           (0x30000 > address))
   {
      u8Datalength *= 4;
   }
   else if((0x30000 <= address) && 
           (0x40000 > address))
   {
      u8Datalength *= 8;
   }
   else
   {
      /* Address out of range.*/
   }
      
   return (u8Datalength);
}/*Hmi_internalRealDataLength*/

/*************************************************************************
 *                            <Hmi_Fsm_Enter_IdleState>
 * In : Hmi_FsmCtxRefType  *ctxRef
 * Out: Hmi_FsmCtxRefType  *ctxRef
 * return: None
 * description: Init the internal HMI state machine.
 ***************************************************************************/
void Hmi_Fsm_Enter_IdleState(Hmi_FsmCtxRefType *ctxRef);

/*************************************************************************
 *                            <Hmi_Fsm_Enter_ReadState>
 * In : Hmi_FsmCtxRefType  *ctxRef, Hmi_MsgCtxType *msg
 * Out: Hmi_FsmCtxRefType  *ctxRef
 * return: Std_ReturnType
 * description: Enter the read state.
 ***************************************************************************/
Std_ReturnType Hmi_Fsm_Enter_ReadState(Hmi_FsmCtxRefType *ctxRef);

/*************************************************************************
 *                            <Hmi_Fsm_Enter_WriteState>
 * In : Hmi_FsmCtxRefType  *ctxRef, Hmi_MsgCtxType *msg
 * Out: Hmi_FsmCtxRefType  *ctxRef
 * return: Std_ReturnType
 * description: Enter the read state.
 ***************************************************************************/
Std_ReturnType Hmi_Fsm_Enter_WriteState(Hmi_FsmCtxRefType *ctxRef);

#endif /* ECUAL_HMI_SCREEN_HMI_PRIV_FSM_H_ */
