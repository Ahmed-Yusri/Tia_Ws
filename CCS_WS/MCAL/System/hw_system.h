/*
 * hw_system.h
 *
 *  Created on: Aug ??, 2017
 *      Author: Ahmed Yusri Mohammed
 */

#ifndef HW_SYSTEM_H_
#define HW_SYSTEM_H_

#define SYS_BASE 0X400FE000UL

#define SRGPIO      0X400FE508UL       /*GPIO software reset */
#define RCGCGPIO    0X400FE608UL 		/* GPIO run mode clock gating control*/
#define SCGCGPIO    0X400FE708UL	    /*GPIO sleep mode clock gating control*/
#define DCGCGPIO    0X400FE808UL 	    /*GPIO deep sleep mode clock gating control*/
#define PRGPIO      0X400FEA08UL 		/*GPIO peripheral ready*/
#define RCC 	    0X400FE060UL 		/*Run-Mode Clock Configuration*/
#define RCC2	    0X400FE070UL		/*Run-Mode Clock Configuration 2*/
#define MOSCCTL     0X400FE07CUL		/*Main Oscillator Control*/
#define GPIOHBCTL   0X400FE06CUL		/*GPIO High-Performance Bus Control*/
#define DSLPCLKCFG  0X400FE144UL        /* Deep Sleep Clock Configuration*/
#define RCGCUART	0X618UL				/* UART clock gating controll */
#define RCGCTIMER	0x604UL				/* GPTM 16 / 32 clock gating control */
#define RCGCWTIMER	0x65CUL				/* GPTM 32 / 64 clock gating control */
#define RCGCQEI  	0x644UL				/* QEI  clock gating control */
#define RCGC0       0X100UL             /*Run Mode Clock Gating Control Register 0 (RCGC0)*/
#define RCGCDMA     0x60CUL             /*Micro Direct Memory Access Run Mode Clock Gating Control*/

#define RCGCPWM     0x640UL             /*Pulse Width Modulator Run Mode Clock Gating Control*/

#define RCGCEEPROM  (0x658UL)          /*EEPROM Run Mode Clock Gating Control*/

#define RCGC0_PWM_BIT   20

/*============================================================================================================
 * This Register can only be accessed from privileged mode
 *============================================================================================================
 */
#define ENB0     0XE000E100UL /*Enable the interrupt*/
#define DIS0     0XE000E180UL /*Disable the interrupt*/
#define PEND0    0XE000E200UL /*Force the interrupt into the pending state and show which interrupt are pending*/
#define UNPEND0  0XE000E280UL /*Force the interrupt into the unpending state*/
#define ACTIVE0  0XE000E300UL /*indicate which interrupt are active */
#define PRI0     0XE000E400UL /*Interrupt Priority 0-> is highest one */
#define APINT    0XE000ED0CUL /*Application interrupt && >>>System reset request Bit 2 <<<<< */
/*==========================================================================================================*/

#endif /* HW_SYSTEM_H_ */
