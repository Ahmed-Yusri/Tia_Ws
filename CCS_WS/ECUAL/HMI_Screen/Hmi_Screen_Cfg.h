/*
 * Hmi_Screen_Cfg.h
 *
 *  Created on: Jan 25, 2021
 *      Author: Ahmeed Yusri Mohamed
 */

#ifndef ECUAL_HMI_SCREEN_HMI_SCREEN_CFG_H_
#define ECUAL_HMI_SCREEN_HMI_SCREEN_CFG_H_

#include "TM4C129E_HwUartInfo.h"
#include "Uart.h"
#include "Dma.h"
#include "Hmi_Screen.h"

#define HMI_SCREEN_READ_PACKET_USER_DATA1_LENGTH           ((Hmi_LengthType)0X02)

#define HMI_SCREEN_READ_PACKET_USER_DATA1_ADDRESS          ((Hmi_LengthType)0X00000800)

/*Uart Configuration parameters.*/
#define HMI_SCREEN_CFG_UART_MODULE_ID                      UART_CHANNEL4_ID
#define HMI_SCREEN_CFG_UART_BAUDERATE                      19200U    
#define HMI_SCREEN_CFG_UART_PARITYBIT                      E_UART_NONE_PARITY_BIT
#define HMI_SCREEN_CFG_UART_INTERRUPT_ID                   E_INTERRUPT_ID_UART4
#define HMI_SCREEN_CFG_UART_RX_BUFFER_ADDRESS              ((void*)&GET_UART_REG_INFO(UART4_HW_REG_ID).UARTDR)
#define HMI_SCREEN_CFG_UART_TX_BUFFER_ADDRESS              ((void*)&GET_UART_REG_INFO(UART4_HW_REG_ID).UARTDR)
#define HMI_SCREEN_CFG_UART_INTERRUPT_PRIORITY             ((uint8_t)0x03)

/*Dma Configuration parameters.*/
#define HMI_SCREEN_CFG_DMA_RX_CH_ID                      DMA_HMI_UART4_RX_CH
#define HMI_SCREEN_CFG_DMA_TX_CH_ID                      DMA_HMI_UART4_TX_CH


#define HMI_SCREEN_CFG_COM_PACKET_STRUCT_LENGTH          ((uint8_t)11)
#define HMI_SCREEN_CONFIRMATION_LENGTH                   ((uint8_t)0X02)

#define HMI_SCREEN_CFG_WRITE_PACKET_DATA_LENGTH          ((uint8_t)24)
#define HMI_SCREEN_SUCCESSIVE_WRITE_PACKET_SIZE          ((uint8)HMI_SCREEN_CFG_WRITE_PACKET_DATA_LENGTH + HMI_SCREEN_CFG_COM_PACKET_STRUCT_LENGTH)

#define HMI_SCREEN_CFG_READ_PACKET_DATA_LENGTH           ((uint8_t)24)
#define HMI_SCREEN_CFGSUCCESSIVE_READ_PACKET_SIZE        ((uint8)HMI_SCREEN_CFG_READ_PACKET_DATA_LENGTH + HMI_SCREEN_CFG_COM_PACKET_STRUCT_LENGTH + HMI_SCREEN_CONFIRMATION_LENGTH)

#define HMI_SCREEN_CFG_NUM_NFC_JOBS                      ((uint8_t)5)
#define HMI_SCREEN_CFG_MIN_NFC_RESP_LENGTH               ((uint8_t)0x09)
#define HMI_SCREEN_CFG_MAX_NFC_RESP_LENGTH               ((uint8_t)0x0C)

/**
 * @brief if the time out event happend 
 *      then the current state will notify the application
 *      and jump to the idle state.
 */
#define HMI_SCREEN_CFG_WRITE_TIMEOUT_CYCLES                 ((uint32_t)20)
#define HMI_SCREEN_CFG_READ_TIMEOUT_CYCLES                  ((uint32_t)20)
#define HMI_SCREEN_CFG_IDLE_TIMEOUT_CYCLES                  ((uint32_t)2000)

#define HMI_SCREEN_CFG_GET_NFC_COMMAND_TBL_ADDRESS()        ((uint8_t *)Hmi_ScreanCfg.nfcJobCfgPtr)
#define HMI_SCREEN_CFG_GET_NFC_APP_CBK_ADDRESS()            ((void *)Hmi_ScreanCfg.nfcJobEndCbk)

typedef struct Hmi_ScreenNfcJobSt
{
    uint8_t cmd;
    uint8_t cmdLength;
    boolean subscribe;
}Hmi_ScreenCfgNfcJobType[HMI_SCREEN_CFG_NUM_NFC_JOBS];

typedef struct Hmi_ScreenNfcJobSt *Hmi_ScreenCfgNfcJobRefType;

extern const Hmi_ScreenCfgType Hmi_ScreanCfg;

extern void Hmi_writeJobEndCbk(Std_ReturnType jobResult);
extern void Hmi_readJobEndCbk(Std_ReturnType jobResult);
extern void Hmi_userInterventionJobEndCbk(Hmi_AddressType address, Hmi_DataRefType dataPtr);

#endif /* ECUAL_HMI_SCREEN_HMI_SCREEN_CFG_H_ */
