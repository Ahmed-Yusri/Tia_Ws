/*
 * Port_Cfg.c
 *
 *  Created on: Oct 23, 2020
 *      Author: Ahmed Yusri Mohamed Ezzat.
 */
#include "Port.h"

#define PORT_MEMMAP_START_CONST_VAR
#include "Port_MemMap.h"

const Port_ConfigType Port_PinCfg[PORT_NUM_OF_CFG_PINS] =\
{    /*pinId ,pinMode ,pinDirection ,pinInitVal ,pinModeChangable ,pinDirectionChangable ,pinIsDigital ,pinSlewRate ,pinRes, pinDriveStrength*/
   { USER_SWITCH1_PIN_ID, PORT_PIN_MODE_DIO,  PORT_PIN_IN,  STD_LOW, FALSE, FALSE, TRUE, FALSE, PORT_PIN_PULL_UP_RES, PORT_PIN_DRIVE_02MA}, /*User Switch 1.*/
   { USER_SWITCH2_PIN_ID, PORT_PIN_MODE_DIO,  PORT_PIN_IN,  STD_LOW, FALSE, FALSE, TRUE, FALSE, PORT_PIN_PULL_UP_RES, PORT_PIN_DRIVE_02MA}, /*User Switch 2.*/
   { USER_LED0_PIN_ID,    PORT_PIN_MODE_DIO,  PORT_PIN_OUT, STD_LOW, FALSE, FALSE, TRUE, FALSE, PORT_PIN_INTER_RES_NONE, PORT_PIN_DRIVE_08MA}, /*User LED 0.*/   
   { USER_LED1_PIN_ID,    PORT_PIN_MODE_DIO,  PORT_PIN_OUT, STD_LOW, FALSE, FALSE, TRUE, FALSE, PORT_PIN_INTER_RES_NONE, PORT_PIN_DRIVE_08MA}, /*User LED 1.*/   
   { USER_LED2_PIN_ID,    PORT_PIN_MODE_DIO,  PORT_PIN_OUT, STD_LOW, FALSE, FALSE, TRUE, FALSE, PORT_PIN_INTER_RES_NONE, PORT_PIN_DRIVE_08MA}, /*User LED 2.*/   
   { USER_LED3_PIN_ID,    PORT_PIN_MODE_DIO,  PORT_PIN_OUT, STD_LOW, FALSE, FALSE, TRUE, FALSE, PORT_PIN_INTER_RES_NONE, PORT_PIN_DRIVE_08MA},  /*User LED 3.*/   
   { USER_UART0_RX_ID,    PORT_PIN_MODE_UART, PORT_PIN_IN,  STD_LOW, FALSE, FALSE, TRUE, FALSE, PORT_PIN_INTER_RES_NONE, PORT_PIN_DRIVE_08MA},  /*User UART 0.*/   
   { USER_UART0_TX_ID,    PORT_PIN_MODE_UART, PORT_PIN_OUT, STD_LOW, FALSE, FALSE, TRUE, FALSE, PORT_PIN_INTER_RES_NONE, PORT_PIN_DRIVE_08MA}, /*User UART 0.*/
   { USER_TESTP_PIN_ID,   PORT_PIN_MODE_DIO,  PORT_PIN_OUT, STD_LOW, FALSE, FALSE, TRUE, FALSE, PORT_PIN_INTER_RES_NONE, PORT_PIN_DRIVE_08MA},  /*User test pin.*/
   { USER_PWM02_PIN_ID,   PORT_PIN_MODE_PWM,  PORT_PIN_OUT, STD_LOW, FALSE, FALSE, TRUE, FALSE, PORT_PIN_INTER_RES_NONE, PORT_PIN_DRIVE_08MA},  /*User PWM.*/
   { USER_UART4_RX_ID,    PORT_PIN_MODE_UART, PORT_PIN_IN,  STD_LOW, FALSE, FALSE, TRUE, FALSE, PORT_PIN_INTER_RES_NONE, PORT_PIN_DRIVE_08MA},  /*User UART 4.*/   
   { USER_UART4_TX_ID,    PORT_PIN_MODE_UART, PORT_PIN_OUT, STD_LOW, FALSE, FALSE, TRUE, FALSE, PORT_PIN_INTER_RES_NONE, PORT_PIN_DRIVE_08MA}   /*User UART 4.*/
}; /*Global Port Configuration Structure.*/

#define PORT_MEMMAP_STOP_SEC_CONST_VAR
#include <MCAL/Port/Port_MemMap.h>


