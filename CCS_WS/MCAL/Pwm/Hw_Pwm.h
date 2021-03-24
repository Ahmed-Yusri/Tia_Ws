/*
 * Hw_Pwm.h
 *
 *  Created on: Jul 17, 2020
 *      Author: Ahmed Yusri Mohammed
 */

#ifndef HW_HAL_HW_PWM_H_
#define HW_HAL_HW_PWM_H_

/*
 * PWM MODULES BASE ADDRESS..
 */
#define PWM_MODULE0_BASE                0x40028000UL
#define PWM_MODULE1_BASE                0x40029000UL

#define PWM_MASTER_CTL                  0X000UL

#define PWM_TIME_BASE_SYNC              0X004UL

#define PWM_OUTPUT_ENABLE               0X008UL
#define PWM_OUTPUT_INVERSION            0X00CUL
#define PWM_OUTPUT_FAULT                0X010UL

#define PWM_INTERRUPT_ENABLE            0X014UL
#define PWM_RAW_INTERRUPT_STATUS        0X018UL
#define PWM_INTERRUPT_STATUS_CLEAR      0X01CUL
#define PWM_STATUS                      0X020UL

#define PWM_FAULT_CONDITION             0X024UL
#define PWM_ENABLE_UPDATE               0X028UL


#define PWM_GEN0_OFFSET                    0X040UL
#define PWM_GEN1_OFFSET                    0X080UL
#define PWM_GEN2_OFFSET                    0X0C0UL
#define PWM_GEN3_OFFSET                    0X100UL

#define PWM_GEN_CTL_IDX                     0X00
#define PWM_GEN_INTERRUPT_AND_TRIG_EN_IDX   0X01
#define PWM_GEN_RAW_INTERRUPT_STATUS_IDX    0X02
#define PWM_GEN_INTERRUPT_STATUS_CLR_IDX    0X03
#define PWM_GEN_LOAD_IDX                    0X04
#define PWM_GEN_COUNTER_IDX                 0X05
#define PWM_GEN_COMPARE_A_IDX               0X06
#define PWM_GEN_COMPARE_B_IDX               0X07
#define PWM_GEN_GENERATOR_A_CTL_IDX         0X08
#define PWM_GEN_GENERATOR_B_CTL_IDX         0X09
#define PWM_GEN_DEAD_BAND_CTL_IDX           0X0A
#define PWM_GEN_DEAD_BAND_RASING_DELAY_IDX  0X0B
#define PWM_GEN_DEAD_BAND_FALLING_DELAY_IDX 0X0C
#define PWM_GEN_FAULT_SRC0_IDX              0X0D
#define PWM_GEN_FAULT_SRC1_IDX              0X0E
#define PWM_GEN_MIN_FAULT_PERIOD_IDX        0X0F

#define PWM_CH0_FAULT_PIN_SENSE         0X800UL
#define PWM_CH0_FAULT_STATUS0           0X804UL
#define PWM_CH0_FAULT_STATUS1           0X808UL

#define PWM_CH1_FAULT_PIN_SENSE         0X880UL
#define PWM_CH1_FAULT_STATUS0           0X884UL
#define PWM_CH1_FAULT_STATUS1           0X888UL

#define PWM_CH2_FAULT_STATUS0           0X904UL
#define PWM_CH2_FAULT_STATUS1           0X908UL

#define PWM_CH3_FAULT_STATUS0           0X984UL
#define PWM_CH3_FAULT_STATUS1           0X988UL

#define PWM_PERIPHERAL_PROPERTIES       0XFC0UL
#define PWM_CLOCK_CONFIGURATION         0xFC8UL

#endif /* HW_HAL_HW_PWM_H_ */
