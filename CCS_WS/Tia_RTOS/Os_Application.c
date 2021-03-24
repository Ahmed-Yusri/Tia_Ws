/*
 * Os_Application.c
 *
 *  Created on: Aug 26, 2020
 *      Author: ahmed
 */

#include "LedSys_FsmInterface.h"
#include "Dio.h"
#include "Uart.h"
#include "Eep.h"
#include "SchM_Eep.h"

#include "Os_Application.h"

#include "Buttons.h"
#include "Led.h"

#include "Pwm.h"

#include "Hmi_Screen_Cfg.h"


static uint32 ButtonsConfiguration_RamBlock[5] = {0};
static char uartData[6] = {0};
static uint8 dataIdx = 0;

const char *const inputs[5] =
{
   "Button 1 short time configuration", "Button 1 long time configuration",
   "Button 2 short time configuration", "Button 2 long time configuration", "Exit"
};

OS_TASK(OS_LED_TASK)
{
    EventMaskType event;

    while(1)
    {
       Os_WaitEvent(OS_EVENT_STATE_PATTERN_TASK_P50MS_BIT_MASK | OS_EVENT_RED_LED_P300MS_BIT_MASK | OS_EVENT_OS_LED_TASK_UART_DATA_READY);
       Os_GetEvent(&event);
       Os_ClearEvent(event);

       if(event & OS_EVENT_STATE_PATTERN_TASK_P50MS_BIT_MASK)
       {
           Eep_MainFunction();

           Buttons_MainHdl();
           
           FSM_cyclicProc();
           
           Led_MainHdl();
       }

       if(event & OS_EVENT_OS_LED_TASK_UART_DATA_READY)
       {
          //Pwm_Set_DutyCycle(PWM_MODULE0_BASE, 2, 99);
       }
    }
}

OS_TASK(OS_STATE_PATTERN_TASK)
{
    EventMaskType event;

    while(1)
    {
        Os_WaitEvent(OS_EVENT_GREAN_LED_P10MS_BIT_MASK|OS_EVENT_GREAN_LED_P500MS_BIT_MASK);
        Os_GetEvent(&event);
        Os_ClearEvent(event);

        volatile union
        {
           struct
           {
               uint16_t Heater_Zone3;
               uint16_t Heater_Zone2;
               uint16_t Heater_Zone1;
           }u16Heaters;
           uint8_t u8Heaters[6];
        }Hmi_Heaters;

        if(event & OS_EVENT_GREAN_LED_P10MS_BIT_MASK)
        {
            /**
             * UART & EEPROM Test Code..
             */
             if(Serial_isDataReady(UART_CHANNEL0_ID))
             {
               char byte = Serial_readByte(UART_CHANNEL0_ID);
               uint8 inputIdx = 0;

               switch(byte)
               {
                   case 'X':
                   {
                      Serial_sendString(UART_CHANNEL0_ID, "OK");
                      while(inputIdx < 4)
                      {
                         Serial_sendString(UART_CHANNEL0_ID, (char*)inputs[inputIdx]);

                         while(1)
                         {
                             if(Serial_isDataReady(UART_CHANNEL0_ID))
                             {
                                uartData[dataIdx] = Serial_readByte(UART_CHANNEL0_ID);
                                dataIdx++;
                             }

                             if(uartData[dataIdx - 1] =='.')
                             {
                               if(uartData[0] == 'M' && uartData[1] == 'o' && uartData[2] == 'd' && uartData[3] == 'e')
                               {
                                  if(uartData[4] == '1')
                                  {
                                     ButtonsConfiguration_RamBlock[inputIdx] = LED_SYS_MODE_1_COMMAND_ID;
                                  }
                                  else if(uartData[4] == '2')
                                  {
                                     ButtonsConfiguration_RamBlock[inputIdx] = LED_SYS_MODE_2_COMMAND_ID;
                                  }
                                  else{}
                               }
                               else if(uartData[0] == 'U' && uartData[1] == 'n' && uartData[2] == 'd' && uartData[3] == 'o')
                               {
                                   ButtonsConfiguration_RamBlock[inputIdx] = COMMAND_NO_COMMAND_ID;
                               }
                               else if(uartData[0] == 'O' && uartData[1] == 'p' && uartData[2] == 'e' && uartData[3] == 'n')
                               {
                                   ButtonsConfiguration_RamBlock[inputIdx] = DOOR_SYS_OPEN_COMMAND_ID;
                               }
                               else if(uartData[0] == 'C' && uartData[1] == 'l' && uartData[2] == 'o' && uartData[3] == 's' && uartData[4] == 'e')
                               {
                                   ButtonsConfiguration_RamBlock[inputIdx] = DOOR_SYS_CLOSE_COMMAND_ID;
                               }
                               else{}

                               break;
                            }
                         }
                         dataIdx = 0;
                         inputIdx++;
                      }

                      Eep_Write(0, (uint8*)ButtonsConfiguration_RamBlock, 16);

                      Serial_sendString(UART_CHANNEL0_ID, (char*)inputs[inputIdx]);
                      break;
                   }
                   case '1':
                   {
                       (void)Hmi_ReadMemoryRange((uint8_t *)&Hmi_Heaters, 0X080014, 3);  /*Send read command..*/
                       break;
                   }
                   case '2':
                   {
                       Hmi_WriteMemoryRange((uint8_t *)&Hmi_Heaters, 0X080006, 3);
                       break;
                   }
                   default:
                   {
                       break;
                   }
               }
            }
        }

        if(event & OS_EVENT_GREAN_LED_P500MS_BIT_MASK)
        {

        }
    }
}

OS_TASK(OS_IDLE_TASK_ID)
{
   while(1)
   {
      Os_EnterIdleState(); /*Jump to wait on interrupt mode..*/
   }
}
