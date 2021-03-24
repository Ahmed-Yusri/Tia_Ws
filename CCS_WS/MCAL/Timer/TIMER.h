/*
 * TIMER.h
 *
 *  Created on: Nov 9, 2017
 *      Author: Ahmed Yusri Mohammed
 */

#ifndef HAL_TIMER_TIMER_H_
#define HAL_TIMER_TIMER_H_

#include "HW_TIMER.h"
#include "types.h"
#include "hw_types.h"

typedef uint32_t Timer_t;
typedef uint32_t Time_t;

Std_ReturnType Periodic_Timer_Init(Timer_t timer,Time_t time);
void Timer_InterrupetClear(Timer_t timer);
Std_ReturnType OneShot_Timer_Init(Timer_t timer,Time_t time);
void Timer_Disabel(Timer_t timer);

#endif /* HAL_TIMER_TIMER_H_ */
