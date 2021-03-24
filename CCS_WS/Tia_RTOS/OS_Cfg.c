/*
 * OS_Cfg.c
 *
 *  Created on: Aug 25, 2020
 *      Author: Ahmed Yusri Mohamed
 */


#include "OS.h"
#include "Os_Application.h"

/*****************************************************
 * Os Task Stack (shall be aligned on 8 bytes)
 ******************************************************/
static uint32 red_task_stack[128]  __attribute__ ((aligned(8)));
static uint32 grean_task_stack[128]__attribute__ ((aligned(8)));
static uint32 idle_task_stack[128] __attribute__ ((aligned(8)));

/*******************************************************************
 *  Os Task Configuration.
 ******************************************************************/
const OsTaskCfgType Os_TaskCfg[OS_CFG_NUMBER_OF_OS_TASKS] =\
{
   {
      { /*OS_LED_TASK*/
         &red_task_stack[0],  /*Task local stack start address..*/
         OS_TASK_ADDRESS(OS_LED_TASK), /*Thread Address..*/
         128, /*Task Stack size..*/
         0 /*Highst priority = 0, Lowest priority = 255 "Reserved for the Idle Task"..*/
      }
   },
   {
      { /*OS_STATE_PATTERN_TASK*/
         &grean_task_stack[0],   /*Task local stack start address..*/
         OS_TASK_ADDRESS(OS_STATE_PATTERN_TASK), /*Thread Address..*/
         128,   /*Task Stack size..*/
         1 /*Highst priority = 0, Lowest priority = 255 "Reserved for the Idle Task"..*/
      }
   },
   {
      { /*OS_IDLE_TASK_ID*/
         &idle_task_stack[0], /*Task local stack start address..*/
         OS_TASK_ADDRESS(OS_IDLE_TASK_ID), /*Thread Address..*/
         128, /*Task Stack size..*/
         255 /*Highst priority = 0, Lowest priority = 255 "Reserved for the Idle Task"..*/
      }
   }
};

const OsHooksType OsHooks_Cfg =\
{
   NULL_PTR,
   &MCU_Init,
   NULL_PTR,
   NULL_PTR
};

/*The timing events in ms..*/
const Os_TimingEventType Os_Timing_Event[OS_CFG_NUMBER_OF_TIMING_EVENTS] =\
{ /*Tms, EventId*/
   {50, 0},     /*1000 MS.*/
   {20000, 1},  /*2000 MS.*/
   {10, 8},    /*10 MS.*/
   {10000, 9}   /*300 MS.*/
};
