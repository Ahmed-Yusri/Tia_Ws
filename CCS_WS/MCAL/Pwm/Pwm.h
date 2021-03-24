/*
 * Pwm.h
 *
 *  Created on: Jul 18, 2020
*      Author: Ahmed Yusri Mohammed
 */

#ifndef HAL_PWM_H_
#define HAL_PWM_H_

#include "types.h"
#include "TM4C129E_HwGpioInfo.h"
#include "hw_types.h"
#include "hw_Pwm.h"
#include "system.h"


/*PWM module type..*/
#define PWM_MODULE0             PWM_MODULE0_BASE
#define PWM_MODULE1             PWM_MODULE1_BASE

/*typedef uint8_t Pwm_Channel_t;*/
#define PWM_CHANNEL_00           ((uint8_t)0x00)
#define PWM_CHANNEL_01           ((uint8_t)0x01)
#define PWM_CHANNEL_02           ((uint8_t)0x02)
#define PWM_CHANNEL_03           ((uint8_t)0x03)
#define PWM_CHANNEL_04           ((uint8_t)0x04)
#define PWM_CHANNEL_05           ((uint8_t)0x05)
#define PWM_CHANNEL_06           ((uint8_t)0x06)
#define PWM_CHANNEL_07           ((uint8_t)0x07)

#define PWM_GEN_IDX0             ((uint8_t)0)
#define PWM_GEN_IDX1             ((uint8_t)1)
#define PWM_GEN_IDX2             ((uint8_t)2)
#define PWM_GEN_IDX3             ((uint8_t)3)

/*Pwm channel Cfg type..*/
#define PWM_MODULE0_CH00         (const Pwm_ChCfg_t){E_GPIO_CGC_PORT_F, GPIO_PORTF_REG_INFO, 0, 6, PWM_MODULE0_BASE, PWM_GEN0_OFFSET, PWM_GEN_GENERATOR_A_CTL_IDX, 0}
#define PWM_MODULE0_CH01         (const Pwm_ChCfg_t){E_GPIO_CGC_PORT_F, GPIO_PORTF_REG_INFO, 1, 6, PWM_MODULE0_BASE, PWM_GEN0_OFFSET, PWM_GEN_GENERATOR_B_CTL_IDX, 1}
#define PWM_MODULE0_CH02         (const Pwm_ChCfg_t){E_GPIO_CGC_PORT_F, GPIO_PORTF_REG_INFO, 2, 6, PWM_MODULE0_BASE, PWM_GEN1_OFFSET, PWM_GEN_GENERATOR_A_CTL_IDX, 2}
#define PWM_MODULE0_CH03         (const Pwm_ChCfg_t){E_GPIO_CGC_PORT_F, GPIO_PORTF_REG_INFO, 3, 6, PWM_MODULE0_BASE, PWM_GEN1_OFFSET, PWM_GEN_GENERATOR_B_CTL_IDX, 3}
#define PWM_MODULE0_CH04         (const Pwm_ChCfg_t){E_GPIO_CGC_PORT_G, GPIO_PORTG_REG_INFO, 0, 6, PWM_MODULE0_BASE, PWM_GEN2_OFFSET, PWM_GEN_GENERATOR_A_CTL_IDX, 4}
#define PWM_MODULE0_CH05         (const Pwm_ChCfg_t){E_GPIO_CGC_PORT_G, GPIO_PORTG_REG_INFO, 1, 6, PWM_MODULE0_BASE, PWM_GEN2_OFFSET, PWM_GEN_GENERATOR_B_CTL_IDX, 5}
#define PWM_MODULE0_CH06         (const Pwm_ChCfg_t){E_GPIO_CGC_PORT_K, GPIO_PORTK_REG_INFO, 4, 6, PWM_MODULE0_BASE, PWM_GEN3_OFFSET, PWM_GEN_GENERATOR_A_CTL_IDX, 6}
#define PWM_MODULE0_CH07         (const Pwm_ChCfg_t){E_GPIO_CGC_PORT_K, GPIO_PORTK_REG_INFO, 5, 6, PWM_MODULE0_BASE, PWM_GEN3_OFFSET, PWM_GEN_GENERATOR_B_CTL_IDX, 7}

typedef uint32_t Pwm_Module_t;

typedef struct Pwm
{
   System_GpioCgcPortType port;
   Gpio_HwRegisterRefType base;
   uint8_t pin;
   uint8_t mux;
   Pwm_Module_t module;
   uint32_t gen_offset;
   uint8_t ab_idx;
   uint8_t idx;
}Pwm_ChCfg_t;

typedef uint8_t Pwm_Channel_t;

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
 *****************************************************************************/
Std_ReturnType Pwm_Init(const Pwm_ChCfg_t channel, uint32_t frequency);

/*************************************************************************
 *        <Pwm_Set_DutyCycle>
 * In : module, channel, duty_prec
 * Out: None
 * return: Std_ReturnType
 * description:
 * The duty cycle describes
 * the amount of time the signal is in a high (on) state
 * as a percentage of the total time of it takes to complete one cycle.
 ***************************************************************************/
Std_ReturnType Pwm_Set_DutyCycle(uint32_t module, Pwm_Channel_t channel, uint8_t duty);

/*************************************************************************
 *        <Pwm_Interrupt_Enable>
 * In : module, genIdx
 * Out: None
 * return: Std_ReturnType
 * description: Enable the PWM interrupt trigger.
 ***************************************************************************/
Std_ReturnType Pwm_Interrupt_Enable(uint32_t module, Pwm_Channel_t channel);

/*************************************************************************
 *        <Pwm_Interrupt_Disable>
 * In : module, genIdx
 * Out: None
 * return: Std_ReturnType
 * description: Disable the PWM interrupt trigger at the end of each period.
 ***************************************************************************/
Std_ReturnType Pwm_Interrupt_Disable(uint32_t module, Pwm_Channel_t channel);

/*************************************************************************
 *        <Pwm_Interrupt_Status>
 * In : module, channel
 * Out: None
 * return: uint32_t
 * description: get the raw interrupt status.
 ***************************************************************************/
uint32_t Pwm_Interrupt_Status(uint32_t module, Pwm_Channel_t channel);

/*************************************************************************
 *        <Pwm_Interrupt_Clear>
 * In : module, channel
 * Out: None
 * return: None
 * description: Clear interrupt Flag.
 ***************************************************************************/
void Pwm_Interrupt_Clear(uint32_t module, Pwm_Channel_t channel);

#endif /* HAL_PWM_H_ */
