/*
 * Hmi_DataLoggerMgr.c
 *
 *  Created on: Jan 26, 2021
 *      Author: Ahmed Yusri Mohamed
 */

#include "Hmi_dataHandler.h"

/**
 * @brief Main data structure for the Hmi data.
 */
static struct
{
   Hmi_machineControlDataType mainControl;
   Hmi_machineStateDataType machineState;
   Hmi_moldDataType moldSetup;
   Hmi_blowingDataType blowing;
   Hmi_tempDataType tempControl;
}Hmi_data;

/**********************************************************************
 * Lower layer job end callback functions.
 **********************************************************************/
void Hmi_writeJobEndCbk(Std_ReturnType jobResult)
{

}/*Hmi_writeJobEndCbk*/

void Hmi_readJobEndCbk(Std_ReturnType jobResult)
{

}/*Hmi_readJobEndCbk*/

void Hmi_userInterventionJobEndCbk(Hmi_AddressType address, Hmi_DataRefType dataPtr)
{

    Os_SetEvent(OS_LED_TASK,
                OS_EVENT_OS_LED_TASK_UART_DATA_READY);
}/*Hmi_userInterventionJobEndCbk*/

/*********************************************************************
 * Main Funcitons.
 *********************************************************************/
