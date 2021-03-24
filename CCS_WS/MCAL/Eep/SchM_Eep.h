/*
 * SchM_Eep.h
 *
 *  Created on: Nov 4, 2020
 *      Author: Ahmed Yusri Mohammed Ezzat.
 */

#ifndef MCAL_EEP_SCHM_EEP_H_
#define MCAL_EEP_SCHM_EEP_H_

/******************************************************************
 * Function Name: <Eep_MainFunction>
 * Param (IN) : None
 * Param (OUT): None
 * Return:      None
 * Description: Service to perform the processing of the EEPROM jobs 
 *              (read/write/erase/compare).
 * 
 * Remark:   After a job cancelling, main function waits 
 *           until the EEPROM hardware becomes available 
 *           to perform the new job request).
 ****************************************************************/
void Eep_MainFunction(void);


#endif /* MCAL_EEP_SCHM_EEP_H_ */
