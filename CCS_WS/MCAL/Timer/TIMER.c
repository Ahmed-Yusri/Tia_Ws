/*
 * TIMER.c
 *
 *  Created on: Nov 9, 2017
 *      Author: Ahmed Yusri Mohammed
 */

#include  "system.h"
#include "TIMER.h"

/*
 * By defualt The timer interupt is enable .
 * if you need to change -> Comment line 26.
 */
Std_ReturnType Periodic_Timer_Init(Timer_t timer,Time_t time)
{
	Std_ReturnType ret = E_OK;
	CLEAR_BIT(timer + GPTMCTL, 0);
	WRITE_REG_32BIT(timer + GPTMCFG,0x00000000);
	SET_BIT(timer + GPTMTAMR,1);
	CLEAR_BIT(timer + GPTMTAMR,0);

	if(time > 134217728){
		ret = E_NOT_OK;
	}else{
		time *= 16;
		WRITE_REG_32BIT(timer + GPTMTAILR, time);
		//Enable Timer interrupt.
		SET_BIT(timer + GPTMIMR,0);
		SET_BIT(timer + GPTMCTL,0);
	}
	return ret;
}/*Periodic_Timer_Init*/

Std_ReturnType OneShot_Timer_Init(Timer_t timer,Time_t time)
{
	Std_ReturnType ret = E_OK;

	CLEAR_BIT(timer + GPTMCTL, 0);
	WRITE_REG_32BIT(timer + GPTMCFG, 0);
	SET_BIT(timer + GPTMTAMR,0);
	CLEAR_BIT(timer + GPTMTAMR,1);

	if(time == 0 || time > 134217728)
	{
		ret = E_NOT_OK;
	}
	else
	{
		time *= 16;
		WRITE_REG_32BIT(timer + GPTMTAILR,time);
		//Enable Timer interrupt.
		SET_BIT(timer + GPTMIMR,0);
		SET_BIT(timer + GPTMCTL,0);
	}

	return ret;
}

void Timer_Disabel(Timer_t timer)
{
	//Disable Timer interrupt.
	CLEAR_BIT(timer + GPTMIMR,0);
	CLEAR_BIT(timer + GPTMCTL,0);
}/*Timer_Disabel*/

void Timer_InterrupetClear(Timer_t timer)
{
	WRITE_REG_32BIT(timer + GPTMICR, 1);
}/*Timer_InterrupetClear*/
