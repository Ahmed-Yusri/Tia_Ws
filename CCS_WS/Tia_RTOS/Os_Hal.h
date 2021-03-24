/*
 * Os_Hal.h
 *
 *  Created on: Aug 26, 2020
 *      Author: Ahmed Yusri Mohamed
 */

#ifndef ORTOS_OS_HAL_H_
#define ORTOS_OS_HAL_H_

#include <stdint.h>

#define OS_HAL_DISABLE_ALL_INTERRUPTS()                __asm(" CPSID I \n")
#define OS_HAL_ENABLE_ALL_INTERRUPTS()                 __asm(" CPSIE I \n")
#define ARM_M7_VECTOR_TABLE_OFFSET                       0xE000ED08
#define INTERRUPT_CONTROL_REG                           *((uint32_t volatile *)0xE000ED04)

#define SYSTEM_HANDLER_PRIORITY2_REG                    *((uint32_t volatile *)0xE000ED1C)
#define SYSTEM_HANDLER_PRIORITY3_REG                    *((uint32_t volatile *)0xE000ED20)

#define SYSTICK_CONTROL_REG                             *((uint32_t volatile *)0xE000E010)
#define SYSTICK_RELOAD_VALUE_REG                        *((uint32_t volatile *)0xE000E014)

#define SYSTICK_CONTROL_ENABLE_VAL                      ((uint32_t)0x07) /*CLK_SRC->System clock, INTEN->Enabled,
                                                                           ENABLE->Enables SysTick to operate in a multi-shot way. */


#define LOW_INTERRUPT_PRIORITY_LEVEL_VAL                 0xFFU /*ARM Cortex M4 uses 3 bits only..*/
#define HIGH_SYS_INTERRUPT_PRIORITY_LEVEL_VAL            0x40U /*ARM Cortex M4 uses 3 bits only..*/

#define SVCALL_PRIORITY_LEVEL_OFFSET                     0x1DU /*Bits 29, 30 and 31..*/
#define PENDSV_PRIORITY_LEVEL_OFFSET                     0x10U /*Bits 21, 22 and 23..*/
#define SYSTICK_PRIORITY_LEVEL_OFFSET                    0X18U /*Bits 21, 22 and 23..*/

#define PENDSV_INTERRUPT_CONTROL_OFFSET                  28U

#define OS_HAL_SYSTICK_CLK                                   ((uint32_t)16000000) /*16MhZ*/
#define OS_HAL_SYSTICK_TICK_TO_MS                            ((uint32_t)16000)    /*1 Ms*/


/*************************************************************************
 *                      <Os_Hal_DisableOsInterrupts>
 * In : None
 * Out: None
 * return: None
 * description: Any exception that has a programmable priority level with the same
 *              or lower priority as the value of this field is masked.
 ***************************************************************************/
LOCAL_INLINE void Os_Hal_DisableOsInterrupts(void)
{
__asm(" MOV R0, #0x40 \n"\
" MSR BASEPRI, R0 \n");
}/*Os_Hal_DisableOsInterrupts*/
/*************************************************************************
 *                      <Os_Hal_EnableOsInterrupts>
 * In : None
 * Out: None
 * return: None
 * description: Any exception that has a programmable priority level with the same
 *              or lower priority as the value of this field is unmasked.
 ***************************************************************************/
LOCAL_INLINE void Os_Hal_EnableOsInterrupts(void)
{
__asm(" MOV R0, #0 \n"\
" MSR BASEPRI, R0 \n");
}/*Os_Hal_EnableOsInterrupts*/

/*************************************************************************
 *                      <Os_Hal_WaitOnInterrupt>
 * In : None
 * Out: None
 * return: None
 * description: Set the PendSV interrupt priority to the lowest level.
 ***************************************************************************/
LOCAL_INLINE void Os_Hal_Init(void)
{
   SYSTEM_HANDLER_PRIORITY2_REG |= (HIGH_SYS_INTERRUPT_PRIORITY_LEVEL_VAL << SVCALL_PRIORITY_LEVEL_OFFSET);
   SYSTEM_HANDLER_PRIORITY3_REG |= (LOW_INTERRUPT_PRIORITY_LEVEL_VAL << PENDSV_PRIORITY_LEVEL_OFFSET);
   SYSTEM_HANDLER_PRIORITY3_REG |= (HIGH_SYS_INTERRUPT_PRIORITY_LEVEL_VAL << SYSTICK_PRIORITY_LEVEL_OFFSET);
}/*Os_Hal_Init*/

/*************************************************************************
 *                      <Os_Hal_SysTick_Init>
 * In : sysTick
 * Out: None
 * return: None
 * description: Init the System tick timer for the Os time events.
 ***************************************************************************/
LOCAL_INLINE void Os_Hal_SysTick_Init(TickType sysTick)
{
   if(sysTick < 186)
   {
      sysTick *= OS_HAL_SYSTICK_TICK_TO_MS;
      sysTick -= 1;
      SYSTICK_RELOAD_VALUE_REG = sysTick;
      SYSTICK_CONTROL_REG = SYSTICK_CONTROL_ENABLE_VAL;  /*CLK_SRC->System clock, INTEN->Enabled,
                                                           ENABLE->Enables SysTick to operate in a multi-shot way. */
   }
}/*Os_Hal_SysTick_Init*/

/*************************************************************************
 *                      <Os_Hal_StartOs>
 * In : sysTick
 * Out: None 
 * return: None
 * description: 
 *       Enable interrupts and make a supervisor call to trigger to
 *       access OS kernel functions 0->Start the first task.
 ***************************************************************************/
LOCAL_INLINE void Os_Hal_StartOs(void)
{
__asm volatile(\
" CPSIE I \n"\
" SVC #0 \n");
}/*Os_Hal_StartOs*/

/*************************************************************************
 *                      <Os_Hal_WaitOnInterrupt>
 * In : None
 * Out: None
 * return: None
 * description: This is a HW spasific API to be used by the Os.
 ***************************************************************************/
LOCAL_INLINE void Os_Hal_WaitOnInterrupt(void)
{
   //__asm(" WFI \n");
}/*Os_Hal_WaitOnInterrupt*/

/*************************************************************************
 *                      <Os_Hal_TaskStackConstruct>
 * In : uint32_t *sp, uint32_t stack_size
 * Out: None
 * return: sp after update..
 * description: This is a HW spasific init the task stack.
 ***************************************************************************/
LOCAL_INLINE uint32_t *Os_Hal_TaskStackConstruct(uint32_t *sp, uint32_t stack_size,void *thread, void *ret_address)
{
   sp = &sp[stack_size];
   /**********************************************************
    * Exception frame without
    * floating-point storage
    **********************************************************/
   *(--sp) = 0x01000000;             /*Program Status Register (PSR)*/
   *(--sp) = (uint32_t)thread;       /*PC*/
   *(--sp) = (uint32_t)ret_address;  /* LR  The task shall not return to any where after the first run.*/
   *(--sp) = 0x0;                    /* R12*/
   *(--sp) = 0x0;                    /* R03*/
   *(--sp) = 0x0;                    /* R02*/
   *(--sp) = 0x0;                    /* R01*/
   *(--sp) = 0x0;                    /* R00*/

   /*Un stacked General-Purpose Register...*/
   *(--sp) = 0x0;                    /* R04*/
   *(--sp) = 0x0;                    /* R05*/
   *(--sp) = 0x0;                    /* R06*/
   *(--sp) = 0x0;                    /* R07*/
   *(--sp) = 0x0;                    /* R08*/
   *(--sp) = 0x0;                    /* R09*/
   *(--sp) = 0x0;                    /* R10*/
   *(--sp) = 0x0;                    /* R11*/
   
   return sp;
}/*Os_Hal_TaskStackConstruct*/

/*************************************************************************
 *                      <Os_Hal_SysCall>
 * In : None
 * Out: None
 * return: NONE
 * description: This is a HW spasific to perform a "SysCAll".
 ***************************************************************************/
LOCAL_INLINE void Os_Hal_SysCall(void)
{
   /*The implementation of the sys call shall be changed to be a SvCall. "supervisor call" instead of using the PendSv.*/
   /*The current inline function name shall be changed to perform a context switch.. */

   INTERRUPT_CONTROL_REG |= (1U << PENDSV_INTERRUPT_CONTROL_OFFSET); /*Trigger SysCAll ..*/
   /*Add data and instruction barriers.*/
 __asm(" DSB \n"\
" ISB \n");
}/*Os_Hal_SysCall*/

#endif /* ORTOS_OS_HAL_H_ */
