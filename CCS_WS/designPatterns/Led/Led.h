/*
 * Led.h
 *
 *  Created on: Oct 25, 2020
 *      Author: ahmed
 */

#ifndef STATEPATTERN_EXAMPLE_LED_H_
#define STATEPATTERN_EXAMPLE_LED_H_

#include "Std_Types.h"

typedef enum
{
   E_LED_OFF = 0,
   E_LED_SHIFT_LEFT = 1,
   E_LED_SHIFT_RIGHT = 2,
   E_LED_BLINKY = 3,
   E_LED_IS_IDLE
}Led_StatusType;

void Led_startSequence(Led_StatusType status);

void Led_Blinky(uint8 times);
Led_StatusType Led_getSequenceId(void);

void Led_MainHdl(void);


#endif /* STATEPATTERN_EXAMPLE_LED_H_ */
