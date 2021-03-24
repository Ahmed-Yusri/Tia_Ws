/*
 * HW_TIMER.h
 *
 *  Created on: ??þ/??þ/????
 *      Author: Ahmed Yusri Mohammed
 * */

#ifndef HW_HAL_HW_TIMER_H_
#define HW_HAL_HW_TIMER_H_

/*********************************
 * GPTM Base Adress .
 ********************************/

#define TIMER0_16_32      0x40030000UL
#define TIMER0_32_64      0x40036000UL
#define TIMER1_16_32      0x40031000UL
#define TIMER1_32_64      0x40037000UL
#define TIMER2_16_32      0x40032000UL
#define TIMER2_32_64      0x4004C000UL
#define TIMER3_16_32      0x40033000UL
#define TIMER3_32_64      0x4004D000UL
#define TIMER4_16_32      0x40034000UL
#define TIMER4_32_64      0x4004E000UL
#define TIMER5_16_32      0x40035000UL
#define TIMER5_32_64      0x4004F000UL

#define GPTMCFG  		  0x000UL 		 // GPTM Configuration
#define GPTMTAMR		  0x004UL		 // GPTM Timer A Mode
#define GPTMTBMR		  0x008UL		 //	GPTM Timer B Mode
#define GPTMCTL			  0x00CUL		 // GPTM Control
#define GPTMSYNC		  0x010UL		 // GPTM Synchronize
#define GPTMIMR			  0x018UL		 // GPTM Interrupt Mask
#define GPTMRIS			  0x01CUL		 // GPTM Raw Interrupt Status
#define GPTMMIS			  0x020UL		 // GPTM Masked Interrupt Status
#define GPTMICR			  0x024UL		 // GPTM Interrupt Clear
#define GPTMTAILR		  0x028UL		 // GPTM Timer A Interval Load
#define GPTMTBILR		  0x02CUL		 // GPTM Timer B Interval Load
#define GPTMTAMATCHR	  0x030UL		 // GPTM Timer A Match
#define GPTMTBMATCHR	  0x034UL		 //	GPTM Timer B Match
#define GPTMTAPR		  0x038UL		 // GPTM Timer A Prescale
#define GPTMTBPR		  0x03CUL		 // GPTM Timer B Prescale
#define GPTMTAPMR  		  0x040UL		 // GPTM TimerA Prescale Match
#define GPTMTBPMR		  0x044UL  	 	 // GPTM TimerB Prescale Match
#define GPTMTAR    		  0x048UL		 // GPTM Timer A
#define GPTMTBR		      0x04CUL		 // GPTM Timer B
#define GPTMTAV			  0x050UL		 // GPTM Timer A Value
#define GPTMTBV			  0x054UL		 // GPTM Timer B Value
#define GPTMRTCPD		  0x058UL		 // GPTM RTC Predivide
#define GPTMTAPS		  0x05CUL		 // GPTM Timer A Prescale Snapshot
#define GPTMTBPS		  0x060UL		 // GPTM Timer B Prescale Snapshot
#define GPTMTAPV		  0x064UL		 // GPTM Timer A Prescale Value
#define GPTMTBPV		  0x068UL		 // GPTM Timer B Prescale Value
#define GPTMPP			  0xFC0UL		 // GPTM Peripheral Properties

#endif /* HW_HAL_HW_TIMER_H_ */
