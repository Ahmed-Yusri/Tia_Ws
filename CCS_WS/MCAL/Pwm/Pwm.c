/*
 * Pwm.c
 *
 *  Created on: Jul 18, 2020
 *      Author: Ahmed Yusri Mohammed
 */

#include "Pwm.h"

/*******************************************************************************
 * Local MACRO
 *******************************************************************************/ 
#define PWM_HW_REG_RW(b,o)      ((volatile uint32_t*)((b)+(o)))

volatile static const uint32_t pwm_gen_offset[] = {
                                                   PWM_GEN0_OFFSET, PWM_GEN1_OFFSET,
                                                   PWM_GEN2_OFFSET, PWM_GEN3_OFFSET
                                                  };
/*******************************************************************************
 *  Global functions. >-PWM-<
 *******************************************************************************/
/*******************************************************************************
 *        <Pwm_Init>
 * In : module, channel, frequency
 * Out: None
 * return: Std_ReturnType
 * description:
 *             Initialize the Pwm Channel.
 * The frequency determines how fast the PWM completes a cycle
 * (i.e. 1000 Hz would be 1000 cycles per second),
 * and therefore how fast it switches between high and low states.
 *         (Count-Down mode)
 * Min Frequency: 31 HZ ..
 * Max Frequency: 100 KHZ.
 *****************************************************************************/
Std_ReturnType Pwm_Init(const Pwm_ChCfg_t channel, uint32_t frequency)
{
   Std_ReturnType result = E_OK;
   volatile uint32_t *gen_base;
   
#ifdef IN_DEV_MODE
   if((PWM_MODULE0 != channel.module)&& 
      (PWM_MODULE1 != channel.module))
   {
      result = E_NOT_OK;
   }
   else
#endif
   {
      gen_base = PWM_HW_REG_RW(channel.module, channel.gen_offset);
      
      SET_BIT(PWM_CLOCK_CONFIGURATION + PWM_MODULE0_BASE, 0);    /*Set PWM clock divisor to 8..*/
      SET_BIT(PWM_CLOCK_CONFIGURATION + PWM_MODULE0_BASE, 1);    /*Set PWM clock divisor to 8..*/
      CLEAR_BIT(PWM_CLOCK_CONFIGURATION + PWM_MODULE0_BASE, 2);    /*Set PWM clock divisor to 8..*/
      
      SET_BIT(PWM_CLOCK_CONFIGURATION + PWM_MODULE0_BASE, 8);     /*Enable the PWM Clock Divisor..*/
      
      gen_base[PWM_GEN_CTL_IDX] = 0x0;
      gen_base[channel.ab_idx] = 0xC8; /*. Low  -> Load.*/
                                       /*. High -> Compare.*/
      
      frequency = frequency < 16 ? 16 : frequency;
      frequency = frequency > 10000 ? 10000 : frequency;

      gen_base[PWM_GEN_LOAD_IDX] = (uint32_t)(1000000.0/frequency) - 1; /*Add the load value..*/
      
      if(channel.ab_idx == PWM_GEN_GENERATOR_A_CTL_IDX)
      {
         gen_base[PWM_GEN_COMPARE_A_IDX] = 0; /*Set Duty to 0%.*/
      }
      else
      {
         gen_base[PWM_GEN_COMPARE_B_IDX] = 0; /*Set Duty to 0%.*/
      }

      gen_base[PWM_GEN_COUNTER_IDX] = 0;
      gen_base[PWM_GEN_CTL_IDX] |= (1 << 0); /*PWM Block Enable.*/
      
      CLEAR_BIT(channel.module + PWM_OUTPUT_ENABLE, channel.idx); /*Disable PWM Output.*/
   }
   
   return result;
}/*Pwm_Init*/

/*************************************************************************
 *        <Pwm_Set_DutyCycle>
 * In : module, channel, duty
 * Out: None
 * return: Std_ReturnType
 * description:
 * The duty cycle describes
 * the amount of time the signal is in a high (on) state
 * as a percentage of the total time of it takes to complete one cycle.
 ***************************************************************************/
Std_ReturnType Pwm_Set_DutyCycle(uint32_t module, Pwm_Channel_t channel, uint8_t duty)
{
   Std_ReturnType result = E_OK;
   
#ifdef IN_DEV_MODE
   if(((PWM_MODULE0 != module)&& 
      (PWM_MODULE1 != module))||
      (PWM_CHANNEL_07 < channel)||
      (100 < duty))
   {
      result = E_NOT_OK;
   }
   else
#endif
   {
      volatile uint32_t *gen_base = PWM_HW_REG_RW(module, pwm_gen_offset[channel>>1]);
      
      if(channel&1)
      {
         gen_base[PWM_GEN_COMPARE_B_IDX] = ((float)((gen_base[PWM_GEN_LOAD_IDX] + 1.0) / 100.0) * duty) - 1;
      }
      else
      {
         gen_base[PWM_GEN_COMPARE_A_IDX] = ((float)((gen_base[PWM_GEN_LOAD_IDX] + 1.0) / 100.0) * duty) - 1;
      }
      SET_BIT(module + PWM_OUTPUT_ENABLE, channel); /*Enable PWM Output.*/
   }
   return result;
}/*Pwm_Set_DutyCycle*/

/*************************************************************************
 *        <Pwm_Interrupt_Enable>
 * In : module, genIdx
 * Out: None
 * return: Std_ReturnType
 * description: Enable the PWM interrupt trigger at the end of each period.
 ***************************************************************************/
Std_ReturnType Pwm_Interrupt_Enable(uint32_t module, Pwm_Channel_t channel)
{
   Std_ReturnType result = E_OK;
#ifdef IN_DEV_MODE
   if(((PWM_MODULE0 != module)&& 
      (PWM_MODULE1 != module))||
      (PWM_CHANNEL_07 < channel))
   {
      result = E_NOT_OK;
   }
   else
#endif
   {
      volatile uint32_t *gen_base = PWM_HW_REG_RW(module, pwm_gen_offset[channel>>1]);
      gen_base[PWM_GEN_INTERRUPT_AND_TRIG_EN_IDX] |= (1 << 0); /*Set Bit 0 "Counter=0 Interrupt"..*/
      SET_BIT(module + PWM_INTERRUPT_ENABLE, channel>>1); /*Enable the generator interrupt interrupt.*/
   }

   return result;
}/*Pwm_Interrupt_Enable*/

/*************************************************************************
 *        <Pwm_Interrupt_Disable>
 * In : module, genIdx
 * Out: None
 * return: Std_ReturnType
 * description: Disable the PWM interrupt trigger at the end of each period.
 ***************************************************************************/
Std_ReturnType Pwm_Interrupt_Disable(uint32_t module, Pwm_Channel_t channel)
{
   Std_ReturnType result = E_OK;
#ifdef IN_DEV_MODE
   if(((PWM_MODULE0 != module)&&
      (PWM_MODULE1 != module))||
      (PWM_CHANNEL_07 < channel))
   {
      result = E_NOT_OK;
   }
   else
#endif
   {
      volatile uint32_t *gen_base = PWM_HW_REG_RW(module, pwm_gen_offset[channel>>1]);
      gen_base[PWM_GEN_INTERRUPT_AND_TRIG_EN_IDX] &= ~(1 << 0); /*Clear Bit 0 "Counter=0 Interrupt"..*/
      CLEAR_BIT(module + PWM_INTERRUPT_ENABLE, channel>>1); /*Disable the generator interrupt interrupt.*/
   }

   return result;
}/*Pwm_Interrupt_Disable*/

/*************************************************************************
 *        <Pwm_Interrupt_Status>
 * In : module, channel
 * Out: None
 * return: uint32_t
 * description: get the raw interrupt status.
 ***************************************************************************/
uint32_t Pwm_Interrupt_Status(uint32_t module, Pwm_Channel_t channel)
{
   return PWM_HW_REG_RW(module, pwm_gen_offset[channel>>1])[PWM_GEN_RAW_INTERRUPT_STATUS_IDX];
}/*Pwm_Interrupt_Status*/

/*************************************************************************
 *        <Pwm_Interrupt_Clear>
 * In : module, channel
 * Out: None
 * return: None
 * description: Clear interrupt Flag.
 ***************************************************************************/
void Pwm_Interrupt_Clear(uint32_t module, Pwm_Channel_t channel)
{
   SET_BIT(module+PWM_INTERRUPT_STATUS_CLEAR, channel>>1);
   PWM_HW_REG_RW(module, pwm_gen_offset[channel>>1])[PWM_GEN_INTERRUPT_STATUS_CLR_IDX] |= 0X3F;
}/*Pwm_Interrupt_Clear*/
