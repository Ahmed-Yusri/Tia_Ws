/*
 * Os_Hal_Fault.h
 *
 *  Created on: Dec 1, 2020
 *      Author: Ahmed Yusri Moahmed Ezzat.
 */

#ifndef TIA_RTOS_OS_HAL_FAULT_H_
#define TIA_RTOS_OS_HAL_FAULT_H_


#define M4_FAULT_STATE_BASE          ((OsFaultCtxRefType)0xE000ED28) /*Base Address of the fault state.*/

typedef struct
{
   union
   {
      struct
      {
         uint32_t IERR  :1;      /*Instruction Access Violation*/
         uint32_t DERR  :1;      /*Data Access Violation*/
         uint32_t resB2 :1;      /*reserved*/
         uint32_t MUSTKE:1;      /*Unstack Access Violation*/
         uint32_t MSTKE:1;       /*Stack Access Violation*/
         uint32_t MLSPERR:1;     /*Memory Management Fault on Floating-Point Lazy State Preservation.*/
         uint32_t resB6 :1;      /*reserved*/
         uint32_t MMARV:1;       /*Memory Management Fault Address Register Valid.*/
         uint32_t IBUS:1;        /*Instruction Bus Error.*/
         uint32_t PRECISE:1;     /*Precise Data Bus Error.*/
         uint32_t IMPRE:1;       /*Imprecise Data Bus Error.*/
         uint32_t BUSTKE:1;      /*Unstack Bus Fault*/
         uint32_t BSTKE:1;       /*Stack Bus Fault*/
         uint32_t BLSPERR:1;     /*Bus Fault on Floating-Point Lazy State Preservation*/
         uint32_t resB14:1;      /*reserved*/
         uint32_t BFARV:1;       /*Bus Fault Address Register Valid*/
         uint32_t UNDEF:1;       /*Undefined Instruction Usage Fault*/
         uint32_t INVSTAT:1;     /*Invalid State Usage Fault*/
         uint32_t INVPC:1;       /*Invalid PC Load Usage Fault*/
         uint32_t NOCP:1;        /*No Coprocessor Usage Fault*/
         uint32_t resB20_B23:4;  /*reserved*/
         uint32_t UNALIGN:1;     /*Unaligned Access Usage Fault*/
         uint32_t DIV0:1;        /*Divide-by-Zero Usage Fault*/
         uint32_t resB26_B31:6;  /*reserved*/
      }b;
      uint32_t u32;
   }fault_state;
   union
   {
      struct
      {
         uint32_t resB0:1;       /*reserved*/
         uint32_t VECT:1;        /*Vector Table Read Fault*/
         uint32_t resB2_B29:28;  /*reserved*/
         uint32_t FORCED:1;      /*Forced Hard Fault*/
         uint32_t DBG:1;         /*Debug Event*/
      }b;
      uint32_t u32;
   }hw_fault_state;
   uint32_t spare_reg;
   uint32_t MM_address;
   uint32_t bus_address;
}OsFaultCtxType;

typedef volatile OsFaultCtxType * volatile OsFaultCtxRefType;



#endif /* TIA_RTOS_OS_HAL_FAULT_H_ */
