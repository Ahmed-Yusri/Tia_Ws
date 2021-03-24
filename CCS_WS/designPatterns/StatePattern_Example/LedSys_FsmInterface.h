/*
 * StatePattern.h
 *
 *  Created on: Oct 25, 2020
 *      Author: Ahmed Yusri Mohamed Ezzat.
 */

#ifndef STATEPATTERN_EXAMPLE_LEDSYS_FSMINTERFACE_H_
#define STATEPATTERN_EXAMPLE_LEDSYS_FSMINTERFACE_H_

#include "Led.h"

/******************************************************************
 *  Global types.
 *****************************************************************/
typedef enum
{
    E_SYSTEM_NO_EVENTS = 0,
    E_SYSTEM_EV1_IS_SET,
    E_SYSTEM_EV2_IS_SET
}SysEventType;

struct Fsm_Memento
{
   SysEventType state;
};

/******************************************************************
 * Global functions.
 ******************************************************************/

/******************************************************************
 * Function Name: <FSM_newEventReq>
 * Param (IN) : SysEventType ev
 * Param (OUT): None
 * Return: None
 * Description: register the new event.
 ****************************************************************/
void FSM_newEventReq(SysEventType ev);

/******************************************************************
 * Function Name: <FSM_cyclicProc>
 * Param (IN) : None
 * Param (OUT): None
 * Return: None
 * Description:Cyclic function to handle the internal state machine
 *******************************************************************/
void FSM_cyclicProc(void);

/******************************************************************
 * Function Name: <FSM_createMemento>
 * Param (IN) : None
 * Param (OUT): struct Fsm_Memento **me
 * Return: None
 * Description: Capture the current internal state.
 *******************************************************************/
void FSM_createMemento(struct Fsm_Memento **me);

/******************************************************************
 * Function Name: <FSM_setMemento>
 * Param (IN) : const struct Fsm_Memento *me
 * Param (OUT): None
 * Return: None
 * Description: Restore state.
 *******************************************************************/
void FSM_setMemento(const struct Fsm_Memento *me);

#endif /* STATEPATTERN_EXAMPLE_LEDSYS_FSMINTERFACE_H_ */
