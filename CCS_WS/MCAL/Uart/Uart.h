/*
 * Uart.h
 *
 *  Created on: Nov 02, 2020
 *      Author: Ahmed Yusri Mohamed Ezzat.
 *      File Revision:
 *                     Nov 02, 2020   Ahmed Yusri  v0.1 Initial Version
 */

#ifndef MCAL_UART_UART_H_
#define MCAL_UART_UART_H_

#include "types.h"

#define UART_SYS_CLK 							  16000000UL

/*Uart_ChannelIdType*/
#define UART_CHANNEL0_ID                          ((Uart_ChannelIdType)0)
#define UART_CHANNEL1_ID                          ((Uart_ChannelIdType)1)
#define UART_CHANNEL2_ID                          ((Uart_ChannelIdType)2)
#define UART_CHANNEL3_ID                          ((Uart_ChannelIdType)3)
#define UART_CHANNEL4_ID                          ((Uart_ChannelIdType)4)
#define UART_CHANNEL5_ID                          ((Uart_ChannelIdType)5)
#define UART_CHANNEL6_ID                          ((Uart_ChannelIdType)6)
#define UART_CHANNEL7_ID                          ((Uart_ChannelIdType)7)

/****************************************************************************
 * Global Types.
 ***************************************************************************/
typedef enum
{
	E_UART_OVERRUN_ERROR,
	E_UART_BREAK_ERROR,
	E_UART_PARITY_ERROR,
	E_UART_FRAMING_ERROR,
	E_UART_RECEIVE_TIMEOUT,
	E_UART_INIT_ERROR,
	E_UART_NO_ERROR
}Uart_ErrorType;

typedef enum
{
	E_UART_EVEN_PARITY_BIT,
	E_UART_ODD_PARITY_BIT,
	E_UART_NONE_PARITY_BIT
}Uart_ParityBitType;

typedef uint8 Uart_ChannelIdType;

/******************************************************************
 *  Global Functions.
 *******************************************************************/

/******************************************************************
 * Function Name: <Serial_getError>
 * Param (IN) : Uart_ChannelIdType uartId
 * Param (OUT): None
 * Return: Uart_ErrorType
 * Description: Returns the uart error status.
 ****************************************************************/
Uart_ErrorType Serial_getError(Uart_ChannelIdType uartId);

/******************************************************************
 * Function Name: <Serial_readByte>
 * Param (IN) : Uart_ChannelIdType uartId
 * Param (OUT): None
 * Return: sint8
 * Description: Returns a uart data byte.
 ****************************************************************/
sint8 Serial_readByte(Uart_ChannelIdType uartId);

/******************************************************************
 * Function Name: <Serial_sendByte>
 * Param (IN) : Uart_ChannelIdType uartId, sint8 dataByte
 * Param (OUT): None
 * Return: None
 * Description: Send a uart data byte.
 ****************************************************************/
void Serial_sendByte(Uart_ChannelIdType uartId, sint8 dataByte);

/******************************************************************
 * Function Name: <Serial_sendString>
 * Param (IN) : Uart_ChannelIdType uartId, char *str
 * Param (OUT): None
 * Return: None
 * Description: Send a string.
 ****************************************************************/
void Serial_sendString(Uart_ChannelIdType uartId, char *str);

/******************************************************************
 * Function Name: <Serial_SyncTransmit>
 * Param (IN) : (Uart_ChannelIdType uartId, sint8 data[], uint32 len
 * Param (OUT): None
 * Return: None
 * Description: Send data packet.
 ****************************************************************/
void Serial_SyncTransmit(Uart_ChannelIdType uartId, sint8 data[], const uint32 len);

/******************************************************************
 * Function Name: <Serial_isDataReady>
 * Param (IN) : Uart_ChannelIdType uartId
 * Param (OUT): None
 * Return: Uart_ErrorType
 * Description: Check if the UART is ready.
 ****************************************************************/
boolean Serial_isDataReady(Uart_ChannelIdType uartId);

/******************************************************************
 * Function Name: <Serial_Init>
 * Param (IN) : Uart_ChannelIdType uartId, uint32 baudRate, 
 * 				Uart_ParityBitType parituBit
 * Param (OUT): None
 * Return: None
 * Description: Set the UART baud rate.
 ****************************************************************/
void Serial_Init(Uart_ChannelIdType uartId, uint32 baudRate, Uart_ParityBitType parituBit);

/******************************************************************
 * Function Name: <Serial_AsyncTransmit>
 * Param (IN) : (Uart_ChannelIdType uartId, sint8 data[], uint32 len
 * Param (OUT): None
 * Return: None
 * Description: Send data packet.
 ****************************************************************/
void Serial_AsyncTransmit(Uart_ChannelIdType uartId, sint8 data[], uint32 len);

/******************************************************************
 * Function Name: <Serial_setupRxHwBufferSize>
 * Param (IN) : Uart_ChannelIdType uartId,  const uint8 bufferSize
 * Param (OUT): None
 * Return: Std_ReturnType
 * Description: setup the recive hw buffer size.
 ****************************************************************/
Std_ReturnType Serial_setupRxHwBufferSize(Uart_ChannelIdType uartId, const uint8 bufferSize);

/******************************************************************
 * Function Name: <Serial_setupTxHwBufferSize>
 * Param (IN) : Uart_ChannelIdType uartId, const uint8 bufferSize
 * Param (OUT): None
 * Return: Std_ReturnType
 * Description: setup the transmit hw buffer size.
 ****************************************************************/
Std_ReturnType Serial_setupTxHwBufferSize(Uart_ChannelIdType uartId, const uint8 bufferSize);

/******************************************************************
 * Function Name: <Serial_setDmaConnection>
 * Param (IN) : Uart_ChannelIdType uartId
 * Param (OUT): None
 * Return: Std_ReturnType
 * Description: enable the dma connection for this channel.
 ****************************************************************/
Std_ReturnType Serial_setDmaConnection(Uart_ChannelIdType uartId);

/******************************************************************
 * Function Name: <Serial_setInterruptMode>
 * Param (IN) : Uart_ChannelIdType uartId
 * Param (OUT): None
 * Return: Std_ReturnType
 * Description: enable the interrupt for this channel.
 ****************************************************************/
Std_ReturnType Serial_setInterruptMode(Uart_ChannelIdType uartId);

/******************************************************************
 * Function Name: <Serial_clearAllInterrupt>
 * Param (IN) : Uart_ChannelIdType uartId
 * Param (OUT): None
 * Return: None
 * Description: clear the interrupt for this channel.
 ****************************************************************/
void Serial_clearAllInterrupt(Uart_ChannelIdType uartId);

/******************************************************************
 * Function Name: <Uart_Flush>
 * Param (IN) : Uart_ChannelIdType uartId
 * Param (OUT): None
 * Return: None
 * Description: clear the uart buffer.
 ****************************************************************/
void Serial_Flush(Uart_ChannelIdType uartId);

/******************************************************************
 * Function Name: <Serial_getInterruptState>
 * Param (IN) : Uart_ChannelIdType uartId
 * Param (OUT): None
 * Return: None
 * Description: get the interrupt state for this channel.
 ****************************************************************/
uint32_t Serial_getInterruptState(Uart_ChannelIdType uartId);

#endif /* MCAL_UART_UART_H_ */
