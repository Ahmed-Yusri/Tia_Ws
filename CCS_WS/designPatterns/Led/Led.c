/*
 * Led.c
 *
 *  Created on: Oct 25, 2020
 *      Author: ahmed
 */

#include <MCAL/Dio/Dio.h>
#include "Led.h"

#define NUM_OF_CFG_LED                 0x3
#define LED_STEP_COUNT                 0X4                  

typedef void (*const Led_blinkyCbkType)(void);

struct ledprocstruct
{
   Led_StatusType newStatus;
   Led_StatusType curStatus;
   uint32 cnt;
   void (*const hdl[4])(struct ledprocstruct *);
   const Led_blinkyCbkType cbkLst[4];
   const Dio_ChannelType chLst[NUM_OF_CFG_LED];
   uint8 chIdx;
   uint8 btimes;
};

typedef struct ledprocstruct ledProcType;

typedef void (*Led_ProcHdlType)(ledProcType *);
static void Led_offProc(ledProcType *);
static void Led_leftProc(ledProcType *);
static void Led_rightProc(ledProcType *);
static void Led_blinkyProc(ledProcType *ctx);

extern void waitOnExit_isDone(void);
extern void waitOnEnter_isDone(void);

static ledProcType ledProcInfo = {
                                   E_LED_IS_IDLE, E_LED_IS_IDLE, 0,
                                   {
                                     &Led_offProc, &Led_leftProc, &Led_rightProc, &Led_blinkyProc
                                   },
                                   {
                                     waitOnExit_isDone, NULL_PTR, NULL_PTR, waitOnEnter_isDone
                                   },
                                   {
                                     DIO_USER_LED0_PIN_ID, DIO_USER_LED1_PIN_ID, DIO_USER_LED2_PIN_ID
                                   },
                                   0,
                                   0
                                 };


void Led_offProc(ledProcType *ctx)
{
   uint8 ledIdx = NUM_OF_CFG_LED;

   while(ledIdx--)
   {
      Dio_WriteChannel(ctx->chLst[ledIdx], 
                       STD_LOW);
   }

   ctx->newStatus = E_LED_IS_IDLE;
   ctx->cbkLst[E_LED_OFF]();
}/*Led_offProc*/

void Led_leftProc(ledProcType *ctx)
{
   if(!(ctx->cnt % LED_STEP_COUNT))
   {
      Dio_WriteChannel(ctx->chLst[ctx->chIdx], STD_LOW);

      ctx->chIdx = (ctx->chIdx + (NUM_OF_CFG_LED - 1)) % NUM_OF_CFG_LED; /*Go from the right to left.*/

      Dio_WriteChannel(ctx->chLst[ctx->chIdx], STD_HIGH);
   }

}/*Led_leftProc*/

void Led_rightProc(ledProcType *ctx)
{
   if(!(ctx->cnt % LED_STEP_COUNT))
   {
      Dio_WriteChannel(ctx->chLst[ctx->chIdx], STD_LOW);

      ctx->chIdx = (ctx->chIdx + 1) % NUM_OF_CFG_LED; /*Go from the left to right.*/

      Dio_WriteChannel(ctx->chLst[ctx->chIdx], STD_HIGH);
   }
}/*Led_rightProc*/

static void Led_blinkyProc(ledProcType *ctx)
{
   if(ctx->cnt <= ledProcInfo.btimes)
   {  
      if(ctx->cnt == ledProcInfo.btimes)
      {
         ctx->newStatus = E_LED_IS_IDLE;
         
         Dio_WriteChannel(DIO_USER_LED3_PIN_ID, STD_LOW);
         ctx->cbkLst[E_LED_BLINKY]();
      }
      else
      {
         Dio_FlipChannel(DIO_USER_LED3_PIN_ID);
      }
   }
}/*Led_blinkyProc*/

void Led_MainHdl(void)
{
   if(ledProcInfo.curStatus != ledProcInfo.newStatus)
   {
      ledProcInfo.curStatus = ledProcInfo.newStatus;
      /*reset the internal counters*/
      ledProcInfo.chIdx = 0;
      ledProcInfo.cnt = 0;
   }

   if(E_LED_IS_IDLE != ledProcInfo.curStatus)
   {
      ledProcInfo.hdl[ledProcInfo.curStatus](&ledProcInfo); /*Run the correct handler.*/
   }
   
   ++ledProcInfo.cnt; /*Increment the counter..*/
}

void Led_startSequence(Led_StatusType status)
{
   ledProcInfo.newStatus = status;
}/*Led_startSequence*/

void Led_Blinky(uint8 times)
{
   ledProcInfo.newStatus = E_LED_BLINKY;
   ledProcInfo.btimes = times;
}/*Led_startSequence*/

Led_StatusType Led_getSequenceId(void)
{
   return ledProcInfo.curStatus;
}/*Led_isIdle*/
