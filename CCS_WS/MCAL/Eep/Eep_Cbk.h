/*
 * Eep_Cbk.h
 *
 *  Created on: Nov 4, 2020
 *      Author: Ahmed Yusri Mohammed Ezzat.
 */

#ifndef MCAL_EEP_EEP_CBK_H_
#define MCAL_EEP_EEP_CBK_H_


/******************************************************************
 * Function Name: <Eep_JobErrorNotification>
 * Param (IN) : None
 * Param (OUT): None
 * Return:      None
 * Description: Callback routine provided by the user 
 *              to notify the caller that a job has been cancelled 
 *              or aborted with a negative result.
 ****************************************************************/
void Eep_JobErrorNotification(void);

/******************************************************************
 * Function Name: <Eep_JobEndNotification>
 * Param (IN) : None
 * Param (OUT): None
 * Return:      None
 * Description: Callback routine provided by the user to notify 
 *              the caller that a job has been completed with 
 *              a positive result.
 ****************************************************************/
void Eep_JobEndNotification(void);

#endif /* MCAL_EEP_EEP_CBK_H_ */
