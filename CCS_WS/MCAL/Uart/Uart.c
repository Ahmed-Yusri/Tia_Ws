/*
 * Uart.c
 *
 *  Created on: 11/02/2020
 *      Author: Ahmed Yusri Mohammed Ezzat.
 */

#include "Std_Types.h"
#include "TM4C129E_HwUartInfo.h"
#include "Uart.h"

/*******************************************************************
 *  Global Functions.
 *******************************************************************/
/******************************************************************
 * Function Name: <Serial_getError>
 * Param (IN) : Uart_ChannelIdType uartId
 * Param (OUT): None
 * Return: Uart_ErrorType
 * Description: Returns the uart error status.
 ****************************************************************/
Uart_ErrorType Serial_getError(Uart_ChannelIdType uartId)
{
   return E_UART_NO_ERROR; /*TODO:*/
}/*Serial_getError*/

/******************************************************************
 * Function Name: <Serial_readByte>
 * Param (IN) : Uart_ChannelIdType uartId
 * Param (OUT): None
 * Return: sint8
 * Description: Returns a uart data byte.
 ****************************************************************/
sint8 Serial_readByte(Uart_ChannelIdType uartId)
{
   return GET_UART_REG_INFO(uartId).UARTDR;
}/*Serial_readByte*/

/******************************************************************
 * Function Name: <Serial_sendByte>
 * Param (IN) : Uart_ChannelIdType uartId, sint8 dataByte
 * Param (OUT): None
 * Return: None
 * Description: Send a uart data byte.
 ****************************************************************/
void Serial_sendByte(Uart_ChannelIdType uartId, sint8 dataByte)
{
   GET_UART_REG_INFO(uartId).UARTDR = dataByte;
}/*Serial_sendByte*/

/******************************************************************
 * Function Name: <Serial_sendString>
 * Param (IN) : Uart_ChannelIdType uartId, char *str
 * Param (OUT): None
 * Return: None
 * Description: Send a string.
 ****************************************************************/
void Serial_sendString(Uart_ChannelIdType uartId, char *str)
{
   while(*str)
   {
      GET_UART_REG_INFO(uartId).UARTDR = *str;
      while((GET_UART_REG_INFO(uartId).UARTFR & (1 << 5))); /*Wait..*/
      ++str;
   }
}/*Serial_sendString*/

/******************************************************************
 * Function Name: <Serial_SyncTransmit>
 * Param (IN) : (Uart_ChannelIdType uartId, sint8 data[], uint32 len
 * Param (OUT): None
 * Return: None
 * Description: Send data packet.
 ****************************************************************/
void Serial_SyncTransmit(Uart_ChannelIdType uartId, sint8 data[], const uint32 len)
{
   uint32 dataIdx;
   
   for(dataIdx = 0;dataIdx < len; ++dataIdx)
   {
       GET_UART_REG_INFO(uartId).UARTDR = data[dataIdx];
       while(!(GET_UART_REG_INFO(uartId).UARTFR & 5)); /*Wait..*/
   }
}/*Serial_SyncTransmit*/

/******************************************************************
 * Function Name: <Serial_isDataReady>
 * Param (IN) : Uart_ChannelIdType uartId
 * Param (OUT): None
 * Return: Uart_ErrorType
 * Description: Check if the UART is ready "Data is recived".
 ****************************************************************/
boolean Serial_isDataReady(Uart_ChannelIdType uartId)
{
   return ((GET_UART_REG_INFO(uartId).UARTFR & 0X10) == 0);
}/*Serial_isDataReady*/

/******************************************************************
 * Function Name: <Serial_Init>
 * Param (IN) : Uart_ChannelIdType uartId, uint32 baudRate, 
 * 				Uart_ParityBitType parituBit
 * Param (OUT): None
 * Return: None
 * Description: Set the UART baud rate.
 ****************************************************************/
void Serial_Init(Uart_ChannelIdType uartId, uint32 baudRate, Uart_ParityBitType parituBit)
{
   uint32 intBaud; //Integer Baud Rate
   float floatBaud = (float)UART_SYS_CLK / (baudRate << 4); //Float Baud Rate
   intBaud = (uint32) floatBaud;
   floatBaud -= intBaud;
   floatBaud = ((floatBaud * 64) + 0.5);

   GET_UART_REG_INFO(uartId).UARTCTL &= ~(1 << 0);        //Disable UART

   GET_UART_REG_INFO(uartId).UARTIBRD = intBaud;	      //Integer Baud Rate
   
   GET_UART_REG_INFO(uartId).UARTFBRD = floatBaud;	      //Float Baud Rate 
   
   /* Number of data bits transmitted or received is 8-bits and Enable FIFO.*/
   if(E_UART_EVEN_PARITY_BIT == parituBit)
   {
      GET_UART_REG_INFO(uartId).UARTLCRH = 0x76;       /*Even parity bit.*/      
   }
   else if(E_UART_ODD_PARITY_BIT == parituBit)
   {
      GET_UART_REG_INFO(uartId).UARTLCRH = 0x72;       /*Odd parity bit.*/  
   }
   else
   {
      GET_UART_REG_INFO(uartId).UARTLCRH = 0x70;       /*None parity bit.*/  
   }
   
   GET_UART_REG_INFO(uartId).UARTCC =  0x0;	      /*System clock (based on clock source and divisor factor
                                                     programmed in RSCLKCFG register in the System Control Module)*/

   GET_UART_REG_INFO(uartId).UARTCTL |= 0x311;            //Set RXE, TXE and UARTEN EOF.
}/*Serial_Init*/

/******************************************************************
 * Function Name: <Serial_AsyncTransmit>
 * Param (IN) : (Uart_ChannelIdType uartId, sint8 data[], uint32 len
 * Param (OUT): None
 * Return: None
 * Description: Send data packet.
 ****************************************************************/
void Serial_AsyncTransmit(Uart_ChannelIdType uartId, sint8 data[], uint32 len)
{
   /*TODO: */
}/*Serial_AsyncTransmit*/

/******************************************************************
 * Function Name: <Serial_setupRxHwBufferSize>
 * Param (IN) : Uart_ChannelIdType uartId,  const uint8 bufferSize
 * Param (OUT): None
 * Return: Std_ReturnType
 * Description: setup the recive hw buffer size.
 ****************************************************************/
Std_ReturnType Serial_setupRxHwBufferSize(Uart_ChannelIdType uartId, const uint8 bufferSize)
{
	Std_ReturnType ret = E_OK;
   GET_UART_REG_INFO(uartId).UARTIFLS &= ~0x38; /*Clear the 2nd 3 bits 'Clear the old Cfg'.*/

   /*The UART buffer consists of 16 bytes,
      by defualt the Rx interrupt will be triggered only if we have more than 8 bytes.
      after clearing the hw buffer configuration the Rx interrupt will be triggered only if we have more than 2 bytes.
      Trigger the RX ISR in case of RX FIFO has more than 2/4/8/12/14 bytes.
     */

	switch(bufferSize)
	{
		case 4:
		{
         GET_UART_REG_INFO(uartId).UARTIFLS |= 0x08; /*Set the 1st bit.*/
			break;
		}
		case 8:
		{
         GET_UART_REG_INFO(uartId).UARTIFLS |= 0x10; /*Set the 1st bit.*/
			break;
		}
		case 12:
		{
			GET_UART_REG_INFO(uartId).UARTIFLS |= 0x18; /*Set the 1st and 2nd bit.*/
			break;
		}
      case 14:
		{
			GET_UART_REG_INFO(uartId).UARTIFLS |= 0x20; /*Set the 3rd bit.*/
			break;
		}
      case 2:
		default:
		{
         break;
		}
	}

	return ret;
}/*Serial_setupRxHwBufferSize*/

/******************************************************************
 * Function Name: <Serial_setupRxHwBufferSize>
 * Param (IN) : Uart_ChannelIdType uartId, const uint8 bufferSize
 * Param (OUT): None
 * Return: Std_ReturnType
 * Description: setup the transmit hw buffer size.
 ****************************************************************/
Std_ReturnType Serial_setupTxHwBufferSize(Uart_ChannelIdType uartId, const uint8 bufferSize)
{
	Std_ReturnType ret = E_OK;
   GET_UART_REG_INFO(uartId).UARTIFLS &= ~7; /*Clear the first 3 bits 'Clear the old Cfg'.*/

   /*The UART buffer consists of 16 bytes,
      by defualt the Tx interrupt will be triggered only if we have more than 8 free bytes.
      after clearing the hw buffer configuration the Tx interrupt will be triggered only if we have more than 14 free bytes.
      for the first time we need to exceed the limit for one time then the FIFO interrupt will behave in the appropriate way.
      Trigger the TX ISR in case of TX FIFO has more than 14/12/8/4/2 empty spaces.
     */

	switch(bufferSize)
	{
		case 12:
		{
		   GET_UART_REG_INFO(uartId).UARTIFLS |= 1; /*Set the 1st bit.*/
			break;
		}
		case 8:
		{
		   GET_UART_REG_INFO(uartId).UARTIFLS |= 0x2; /*Set the 1st & 2nd bits.*/
			break;
		}
		case 4:
		{
			GET_UART_REG_INFO(uartId).UARTIFLS |= 3; /*Set the 2nd bit.*/
			break;
		}
      case 2:
		{
			GET_UART_REG_INFO(uartId).UARTIFLS |= 4; /*Set the 2nd bit.*/
			break;
		}
      case 14:
		default:
		{
         break;
		}
	}
	return ret;
}/*Serial_setupTxHwBufferSize*/

/******************************************************************
 * Function Name: <Serial_setDmaConnection>
 * Param (IN) : Uart_ChannelIdType uartId
 * Param (OUT): None
 * Return: Std_ReturnType
 * Description: enable the dma connection for this channel.
 ****************************************************************/
Std_ReturnType Serial_setDmaConnection(Uart_ChannelIdType uartId)
{
   GET_UART_REG_INFO(uartId).UARTDMACTL = 0x07;/*Enable Dma TX/Rx/Error bits..*/

   Serial_clearAllInterrupt(uartId); /*Clear the interrupt state.*/

   GET_UART_REG_INFO(uartId).UARTIM |= (1 << 5u) | (1 << 16u);  /*Enable the Dma Rx channel interrupt and the EOT interrupt.*/

   return E_OK;
}/*Serial_setDmaConnection*/

/******************************************************************
 * Function Name: <Serial_setInterruptMode>
 * Param (IN) : Uart_ChannelIdType uartId
 * Param (OUT): None
 * Return: Std_ReturnType
 * Description: Enable the interrupt for this channel.
 *               Prefered to be used without using a DMA connection. 
 ****************************************************************/
Std_ReturnType Serial_setInterruptMode(Uart_ChannelIdType uartId)
{
   Serial_clearAllInterrupt(uartId); /*Clear the interrupt state.*/

   GET_UART_REG_INFO(uartId).UARTIM |= (1 << 5) | (1 << 4);   /*Enable the end of transmition interrupt and the recive buffer level interrupt.*/
   
   return E_OK;
}/*Serial_setInterruptMode*/

/******************************************************************
 * Function Name: <Serial_clearAllInterrupt>
 * Param (IN) : Uart_ChannelIdType uartId
 * Param (OUT): None
 * Return: None
 * Description: clear the interrupt for this channel.
 ****************************************************************/
void Serial_clearAllInterrupt(Uart_ChannelIdType uartId)
{
   /*The UARTICR register is the interrupt clear register. On a write of 1, the corresponding interrupt
      (both raw interrupt and masked interrupt, if enabled) is cleared.*/
   GET_UART_REG_INFO(uartId).UARTICR = 0x31FFF;
}/*Serial_clearAllInterrupt*/

/******************************************************************
 * Function Name: <Serial_getInterruptState>
 * Param (IN) : Uart_ChannelIdType uartId
 * Param (OUT): None
 * Return: None
 * Description: get the interrupt state for this channel.
 ****************************************************************/
uint32_t Serial_getInterruptState(Uart_ChannelIdType uartId)
{
   return GET_UART_REG_INFO(uartId).UARTRIS;
}/*Serial_getInterruptState*/

/******************************************************************
 * Function Name: <Uart_Flush>
 * Param (IN) : Uart_ChannelIdType uartId
 * Param (OUT): None
 * Return: None
 * Description: clear the uart buffer.
 ****************************************************************/
void Serial_Flush(Uart_ChannelIdType uartId)
{
   while(Serial_isDataReady(uartId))
   {
      (void)Serial_readByte(uartId);
   }
}/*Modbus_Flush*/

