/*
 * Buttons.c
 *
 *  Created on: Oct 25, 2020
 *      Author: Ahmed Yusri Mohammed Ezzat.
 */

#include "Dio.h"
#include "Buttons.h" 


#define NUM_OF_CFG_BUTTONS              2
#define INVALID_BUTTON_INDEX            NUM_OF_CFG_BUTTONS
#define BUTTON_CONFIRMATION_CNT         2
#define MAX_VIR_BUTTONS_PER_REL_BUTTON  2

typedef struct
{
   const Dio_ChannelType chlst[NUM_OF_CFG_BUTTONS];
   Command_Type cmdLst[NUM_OF_CFG_BUTTONS][MAX_VIR_BUTTONS_PER_REL_BUTTON];
   const uint32 confirmationCnt[NUM_OF_CFG_BUTTONS][MAX_VIR_BUTTONS_PER_REL_BUTTON];
   uint32 cnt;
   Dio_ChannelType actIdx;
}ButtonProcType;

static ButtonProcType buttonProc =\
{
   { DIO_USER_BUTTON1_PIN_ID, DIO_USER_BUTTON2_PIN_ID},
   { {COMMAND_NO_COMMAND_ID, COMMAND_NO_COMMAND_ID}, {COMMAND_NO_COMMAND_ID, COMMAND_NO_COMMAND_ID} },
   { {2, 10}, {2, 10} },
   0, INVALID_BUTTON_INDEX
};

/***********************************************************************
 * Function Name: <SetCommand>
 * Param (IN) : uint8 inputIdx, Command_Type commandId
 * Param (OUT): None
 * Return: None
 * Description: Load the input tbl with the appropriate command Id.
 **********************************************************************/
void SetCommand(uint8 inputIdx, Command_Type commandId)
{
   uint8 buttonIdx = inputIdx >> 1;
   inputIdx = inputIdx & 1;

   buttonProc.cmdLst[buttonIdx][inputIdx] = commandId;
}/*SetCommand*/

void Buttons_MainHdl(void)
{
   uint8 buttonIdx;
   boolean newRequest = FALSE;

   if((INVALID_BUTTON_INDEX != buttonProc.actIdx) &&
      (STD_LOW == Dio_ReadChannel(buttonProc.chlst[buttonProc.actIdx])))
   {
      ++buttonProc.cnt;
   }
   else
   {
      if(INVALID_BUTTON_INDEX != buttonProc.actIdx)
      {
         for(buttonIdx = 1; buttonIdx < MAX_VIR_BUTTONS_PER_REL_BUTTON; ++buttonIdx)
         {
            if((buttonProc.cnt >= buttonProc.confirmationCnt[buttonProc.actIdx][buttonIdx - 1]) && 
               (buttonProc.cnt <  buttonProc.confirmationCnt[buttonProc.actIdx][buttonIdx]))
            {
               newRequest = TRUE;
               break;
            }
         }

         if(FALSE == newRequest) /*Now we need to check if the last event is good "MAX_VIR_BUTTONS_PER_REL_BUTTON == buttonIdx".*/
         {
            if(buttonProc.cnt >= buttonProc.confirmationCnt[buttonProc.actIdx][buttonIdx - 1])
            {
               newRequest = TRUE;
            }
         }
         
         if(TRUE == newRequest)
         {
            if(COMMAND_NO_COMMAND_ID == buttonProc.cmdLst[buttonProc.actIdx][buttonIdx - 1])
            {
               Undo_Command();
            }
            else
            {
               Command_Dispatch(buttonProc.cmdLst[buttonProc.actIdx][buttonIdx - 1]);
            }
         }

         buttonProc.actIdx = INVALID_BUTTON_INDEX;
      }

      for(buttonIdx = 0;buttonIdx < NUM_OF_CFG_BUTTONS; ++buttonIdx)
      {
         if(STD_LOW == Dio_ReadChannel(buttonProc.chlst[buttonIdx]))
         {
            buttonProc.actIdx = buttonIdx;
            buttonProc.cnt = 1;
            break;
         }
      }
   }
}

