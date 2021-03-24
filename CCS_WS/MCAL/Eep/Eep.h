/*
 * Eep.h
 *  Created on:  Nov 4, 2020
 *      Author: Ahmed Yusri Mohamed Ezzat.
 *
 *      File Revision:
 *                     Nov 4, 2020   Ahmed Yusri  v0.1 Initial Version
 */
#ifndef MCAL_EEP_EEP_H_
#define MCAL_EEP_EEP_H_

#include "Std_Types.h"
#include "MemIf_Types.h"
#include "Eep_Cfg.h"

/*****************************************************************************
 * Global Types.
 ******************************************************************************/

typedef uint32 Eep_AddressType; /*This value is used as address offset for accessing EEPROM data*/
typedef uint32 Eep_LengthType; /*Specifies the number of bytes to read/write/erase/compare.*/

/*****************************************************************************
 * Global Functions.
 ******************************************************************************/

/******************************************************************
 * Function Name: <Eep_Init>
 * Param (IN) : const Eep_ConfigType* ConfigPtr
 * Param (OUT): None
 * Return:      None
 * Description: Service for EEPROM initialization. 
 ****************************************************************/
void Eep_Init(const Eep_ConfigType* ConfigPtr);

/******************************************************************
 * Function Name: <Eep_Read>
 * Param (IN) : Eep_AddressType EepromAddress, 
 *              uint8* DataBufferPtr, 
 *              Eep_LengthType Length
 * Param (OUT): None
 * Return:      Std_ReturnType
 * Description: Reads from EEPROM. 
 ****************************************************************/
Std_ReturnType Eep_Read(Eep_AddressType EepromAddress, 
                        uint8* DataBufferPtr, 
                        Eep_LengthType Length);

/******************************************************************
 * Function Name: <Eep_Write>
 * Param (IN) : Eep_AddressType EepromAddress,  
 *              const uint8* DataBufferPtr
 *              Eep_LengthType Length
 * Param (OUT): None
 * Return:      Std_ReturnType
 * Description: Writes to EEPROM.. 
 ****************************************************************/
Std_ReturnType Eep_Write(Eep_AddressType EepromAddress, 
                         const uint8* DataBufferPtr, 
                         Eep_LengthType Length);

/******************************************************************
 * Function Name: <Eep_Erase>
 * Param (IN) : Eep_AddressType EepromAddress,
 *              Eep_LengthType Length
 * Param (OUT): None
 * Return:      Std_ReturnType
 * Description: Service for erasing EEPROM sections.
 ****************************************************************/
Std_ReturnType Eep_Erase(Eep_AddressType EepromAddress, 
                         Eep_LengthType Length);

/******************************************************************
 * Function Name: <Eep_Compare>
 * Param (IN) : Eep_AddressType EepromAddress,  
 *              const uint8* DataBufferPtr
 *              Eep_LengthType Length
 * Param (OUT): None
 * Return:      Std_ReturnType
 * Description: Compares a data block in EEPROM with an 
 *              EEPROM block in the memory.
 ****************************************************************/
Std_ReturnType Eep_Compare(Eep_AddressType EepromAddress, 
                           const uint8* DataBufferPtr, 
                           Eep_LengthType Length);

/******************************************************************
 * Function Name: <Eep_Cancel>
 * Param (IN) : None
 * Param (OUT): None
 * Return:      None
 * Description: Cancels a running job.
 ****************************************************************/
void Eep_Cancel(void);

/******************************************************************
 * Function Name: <Eep_GetStatus>
 * Param (IN) : None
 * Param (OUT): None
 * Return:      MemIf_StatusType
 * Description: Returns the EEPROM status.
 ****************************************************************/
MemIf_StatusType Eep_GetStatus(void);

/******************************************************************
 * Function Name: <Eep_GetJobResult>
 * Param (IN) : None
 * Param (OUT): None
 * Return:      MemIf_JobResultType
 * Description: This service returns the result of the last job.
 ****************************************************************/
MemIf_JobResultType Eep_GetJobResult(void);

#endif /* MCAL_EEP_EEP_H_ */
