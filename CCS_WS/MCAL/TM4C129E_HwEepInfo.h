/*
 * TM4C129E_HwEepInfo.h
 *
 *  Created on: Nov 4, 2020
 *      Author: Ahmed Yusri Mohamed Ezzat.
 */

#ifndef MCAL_TM4C129E_HWEEPINFO_H_
#define MCAL_TM4C129E_HWEEPINFO_H_

typedef volatile uint32 EepHwRegisterType;
typedef EepHwRegisterType *volatile EepHwRegisterRefType;

typedef struct
{
   EepHwRegisterType EESIZE;
   EepHwRegisterType EEBLOCK;
   EepHwRegisterType EEOFFSET;
   EepHwRegisterType eep_res0;
   EepHwRegisterType EERDWR;
   EepHwRegisterType EERDWRINC;
   EepHwRegisterType EEDONE;
   EepHwRegisterType EESUPP;
   EepHwRegisterType EEUNLOCK;
   EepHwRegisterType eep_res1[3];
   EepHwRegisterType EEPROT;
   EepHwRegisterType EEPASS0;
   EepHwRegisterType EEPASS1;
   EepHwRegisterType EEPASS2;
   EepHwRegisterType EEINT;
   EepHwRegisterType eep_res2[3];
   EepHwRegisterType EEHIDE0;
   EepHwRegisterType EEHIDE1;
   EepHwRegisterType EEHIDE2;
   EepHwRegisterType eep_res3[9];
   EepHwRegisterType EEDBGME;
   EepHwRegisterType eep_res4[0x3CF];
   EepHwRegisterType EEPROMPP;
}Eep_HwRegisterType;

typedef Eep_HwRegisterType *volatile Eep_HwRegisterRefType;

#define EEP_REG_INFO()                  ((Eep_HwRegisterRefType)0x400AF000)

#define SYS_EEP_HOLD_ON_RESET()  *((EepHwRegisterRefType)0x400FE558) |= (1 << 0)

#define SYS_EEP_RELEASE_RESET()  *((EepHwRegisterRefType)0x400FE558) &= ~(1 << 0)

#define SYS_NOOP_CYCLES()         __asm(" NOP \n"); __asm(" NOP \n"); __asm(" NOP \n"); __asm(" NOP \n"); __asm(" NOP \n"); __asm(" NOP \n")

#endif /* MCAL_TM4C129E_HWEEPINFO_H_ */
