/*
 * Hmi_Screen.h
 *
 *  Created on: Jan 22, 2021
 *      Author: Ahmed Yusri Mohamed
 */

#ifndef ECUAL_HMI_SCREEN_HMI_SCREEN_H_
#define ECUAL_HMI_SCREEN_HMI_SCREEN_H_

#include "types.h"

#define HMI_SCREEN_SUCCESSIVE_READ_PACKET_SIZE             ((uint8)11)

#define HMI_SCREEN_START_OF_PACKET_SIZE                    ((uint8)2)

/*Hmi_CmdType*/
#define HMI_SCREEN_SUCCESSIVE_READ_CMD_VAL                  0X83
#define HMI_SCREEN_SUCCESSIVE_WRITE_CMD_VAL                 0X82
#define HMI_SCREEN_TOUCH_KEY_ADD_VAL_CMD_VAL                0X77

#define HMI_SCREEN_TIMEOUT_ERROR                            ((Std_ReturnType)0xAF)
#define HMI_SCREEN_READ_ERROR                               ((Std_ReturnType)0xBF)
#define HMI_SCREEN_WRITE_ERROR                              ((Std_ReturnType)0xCF)
#define HMI_SCREEN_NO_ERROR                                 ((Std_ReturnType)0x55)

/*Hmi_PacketHeaderType*/
#define HMI_SCREEN_PACKET_HEADER_VAL                        ((Hmi_PacketHeaderType)0XAA)

/*Hmi_PacketTailType*/
#define HMI_SCREEN_PACKET_TAIL_VAL                          ((Hmi_PacketTailType)0X3CC333CC)

#define HMI_SCREEN_COMMAND_EXECUTED                         ((uint16_t)0x3e3a)
#define HMI_SCREEN_COMMAND_ERROR                            ((uint16_t)0x213a)

typedef uint8  Hmi_CmdType;
typedef uint32 Hmi_AddressType;
typedef uint8  Hmi_LengthType;
typedef uint32 Hmi_PacketTailType;
typedef uint8  Hmi_PacketHeaderType;
typedef uint8 *Hmi_DataRefType;

/**
 * @brief User intervention callback notification.
 * In: DataAddress, dataPtr
 */
typedef void (*Hmi_NfcEndJobCbkType)(Hmi_AddressType, Hmi_DataRefType);

/**
 * @brief Read callback notification.
 * In: Result/ One job at a time so the user knows the data address / length.
 */
typedef void (*Hmi_ReadEndJobCbkType)(Std_ReturnType);

/**
 * @brief Write callback notification.
 * In: Result
 */
typedef void (*Hmi_WriteEndJobCbkType)(Std_ReturnType);

typedef struct
{
   Hmi_NfcEndJobCbkType   nfcJobEndCbk;
   Hmi_ReadEndJobCbkType  readJobEndCbk;
   Hmi_WriteEndJobCbkType writeJobEndCbk;
   const struct Hmi_ScreenNfcJobSt *nfcJobCfgPtr;
}Hmi_ScreenCfgType;

/*************************************************************************
 *                            <Hmi_Init>
 * In : Hmi_ScreenCfgType *Hmi_CfgPtr
 * Out: None
 * return: Std_ReturnType
 * description: Init the Macl resourses for the Hmi screen.
 ***************************************************************************/
Std_ReturnType Hmi_Init(const Hmi_ScreenCfgType *Hmi_CfgPtr);

/*************************************************************************
 *                         <Hmi_ReadMemoryRange>
 * In : void
 * Out: None
 * return: Std_ReturnType
 * description: Read a specific memory range.
 ***************************************************************************/
Std_ReturnType Hmi_ReadMemoryRange(uint8_t *desPtr, Hmi_AddressType address, Hmi_LengthType length);

/*************************************************************************
 *                         <Hmi_WriteMemoryRange>
 * In : void
 * Out: None
 * return: Std_ReturnType
 * description: write a specific memory range.
 ***************************************************************************/
Std_ReturnType Hmi_WriteMemoryRange(uint8_t *srcPtr, Hmi_AddressType address, Hmi_LengthType length);

#endif /* ECUAL_HMI_SCREEN_HMI_SCREEN_H_ */
