/*
 * TM4C129E_HwUartInfo.h
 *
 *  Created on: Nov 2, 2020
 *      Author: Ahmed Yusri Mohammed Ezzat.
 */

#ifndef MCAL_TM4C129E_HWUARTINFO_H_
#define MCAL_TM4C129E_HWUARTINFO_H_

#include "types.h"

#define GET_UART_REG_INFO(IDX)                                          ((Uart_HwRegisterRefType)0x4000C000UL)[(IDX)]

/*Uart_HwRegisterIdType*/
#define UART0_HW_REG_ID                                                  ((Uart_HwRegisterIdType)0)
#define UART1_HW_REG_ID                                                  ((Uart_HwRegisterIdType)1)
#define UART2_HW_REG_ID                                                  ((Uart_HwRegisterIdType)2)
#define UART3_HW_REG_ID                                                  ((Uart_HwRegisterIdType)3)
#define UART4_HW_REG_ID                                                  ((Uart_HwRegisterIdType)4)
#define UART5_HW_REG_ID                                                  ((Uart_HwRegisterIdType)5)
#define UART6_HW_REG_ID                                                  ((Uart_HwRegisterIdType)6)
#define UART7_HW_REG_ID                                                  ((Uart_HwRegisterIdType)7)

#define UART_TRANSMIT_DMA_RAW_INTERRUPT_STATUS_BIT_NUM                   ((uint8_t)0x20)  
#define UART_RECEIVE_DMA_RAW_INTERRUPT_STATUS_BIT_NUM                    ((uint8_t)0x10)  
#define UART_9_BIT_MODE_RAW_INTERRUPT_STATUS_BIT_NUM                     ((uint8_t)0x0C)  
#define UART_END_OF_TRANSMISSION_RAW_INTERRUPT_STATUS_BIT_NUM            ((uint8_t)0x0B)        
#define UART_OVERRUN_ERROR_RAW_INTERRUPT_STATUS_BIT_NUM                  ((uint8_t)0x0A)
#define UART_BREAK_ERROR_RAW_INTERRUPT_STATUS_BIT_NUM                    ((uint8_t)0x09)      
#define UART_PARITY_ERROR_RAW_INTERRUPT_STATUS_BIT_NUM                   ((uint8_t)0x08)     
#define UART_FRAMING_ERROR_RAW_INTERRUPT_STATUS_BIT_NUM                  ((uint8_t)0x07)        
#define UART_RECEIVE_TIME_OUT_RAW_INTERRUPT_STATUS_BIT_NUM               ((uint8_t)0x06)           
#define UART_TRANSMIT_RAW_INTERRUPT_STATUS_BIT_NUM                       ((uint8_t)0x05)  
#define UART_RECEIVE_RAW_INTERRUPT_STATUS_BIT_NUM                        ((uint8_t)0x04)   
#define UART_DATA_SET_READY_MODEM_RAW_INTERRUPT_STATUS_BIT_NUM           ((uint8_t)0x03)              
#define UART_DATA_CARRIER_DETECT_MODEM_RAW_INTERRUPT_STATUS_BIT_NUM      ((uint8_t)0x02)                    
#define UART_CLEAR_TO_SEND_MODEM_RAW_INTERRUPT_STATUS_BIT_NUM            ((uint8_t)0x01)              
#define UART_RING_INDICATOR_MODEM_RAW_INTERRUPT_STATUS_BIT_NUM           ((uint8_t)0x00)              

typedef volatile uint32 UartHwRegisterType;
typedef UartHwRegisterType *volatile UartHwRegisterRefType;

typedef struct
{
   UartHwRegisterType UARTDR;
   UartHwRegisterType UARTRSR;
   UartHwRegisterType reser0[0x04];
   UartHwRegisterType UARTFR;
   UartHwRegisterType reser1;
   UartHwRegisterType UARTILPR;
   UartHwRegisterType UARTIBRD;
   UartHwRegisterType UARTFBRD;
   UartHwRegisterType UARTLCRH;
   UartHwRegisterType UARTCTL;
   UartHwRegisterType UARTIFLS;
   UartHwRegisterType UARTIM;
   UartHwRegisterType UARTRIS;
   UartHwRegisterType UARTMIS;
   UartHwRegisterType UARTICR;
   UartHwRegisterType UARTDMACTL;
   UartHwRegisterType reser2[0x16];
   UartHwRegisterType UART9BITADDR;
   UartHwRegisterType UART9BITAMASK;
   UartHwRegisterType reser3[0x3C5];
   UartHwRegisterType UARTPP;
   UartHwRegisterType reser4;
   UartHwRegisterType UARTCC;
   UartHwRegisterType reser5;
   UartHwRegisterType UARTPeriphID4;
   UartHwRegisterType UARTPeriphID5;
   UartHwRegisterType UARTPeriphID6;
   UartHwRegisterType UARTPeriphID7;
   UartHwRegisterType UARTPeriphID0;
   UartHwRegisterType UARTPeriphID1;
   UartHwRegisterType UARTPeriphID2;
   UartHwRegisterType UARTPeriphID3;
   UartHwRegisterType UARTPCellID0;
   UartHwRegisterType UARTPCellID1;
   UartHwRegisterType UARTPCellID2;
   UartHwRegisterType UARTPCellID3;
}Uart_HwRegisterType;

typedef Uart_HwRegisterType *Uart_HwRegisterRefType;
typedef uint8 Uart_HwRegisterIdType;

#endif /* MCAL_TM4C129E_HWUARTINFO_H_ */
