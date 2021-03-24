/*
 * Eep_Cfg.h
 *
 *  Created on: Nov 4, 2020
 *      Author: Ahmed Yusri Mohammed Ezzat.
 */

#ifndef MCAL_EEP_EEP_CFG_H_
#define MCAL_EEP_EEP_CFG_H_

/********************************************************************************************
 * Configurable data types.
 ********************************************************************************************/

#define EEP_WORD_SIZE_IN_BYTES                        ((uint8)4)
#define EEP_WORD_SIZE_P2_IN_BYTES                     ((uint8)2)
#define EEP_BLOCK_SIZE_IN_WORDS                       ((uint8)16)
#define EEP_BLOCK_SIZE_P2_IN_WORDS                    ((uint8)4)
#define EEP_BLOCK_SIZE_IN_BYTES                       ((uint8)64)
#define EEP_BLOCK_SIZE_MASK                           ((uint32)0X3F)
#define EEP_SIZE_IN_BYTES                             ((uint32)6144)

/*Enable the development errors detection..*/
//#define EEP_DEV_ERROR

/* Callback routine provided by the user 
   to notify the caller that a job has been completed with a positive result.
*/
typedef void (*Eep_JobEndNotificationType)(void);

/*Callback routine provided by the user 
  to notify the caller that a job has been cancelled or aborted with a negative result.
*/
typedef void (*Eep_JobErrorNotificationType)(void);

/*This is the type of the external data structure 
  containing the initialization data for the EEPROM driver
*/
typedef struct
{
   uint8 pageSize;    /*The EEPROM blocks are writable only on a word basis "Page".*/
   uint8 sectorSize;  /*The EEPROM blocks are eraseable only on a block basis "Sector".*/
   Eep_JobEndNotificationType EepJobEndNotification;
   Eep_JobErrorNotificationType EepJobErrorNotification;
}Eep_ConfigType;

/**************************************************************************************************
 * Extern VARs.
 **************************************************************************************************/
extern const Eep_ConfigType Eep_Cfg;

#endif /* MCAL_EEP_EEP_CFG_H_ */
