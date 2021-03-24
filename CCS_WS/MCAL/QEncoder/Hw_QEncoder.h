/**************************************
 * Hw_QEncoder.h
 *
 *  Created on: 07/13/2020
 *      Author: Ahmed Yusri Mohammed
 *************************************/

#ifndef HW_QENCODER_H_
#define HW_QENCODER_H_

#include "types.h"

#define QEI0_BASE_ADDRESS                  ((uint32_t)0x4002C000)
#define QEI1_BASE_ADDRESS                  ((uint32_t)0x4002D000)

#define QEI_CONTROL_OFFSET                 ((uint32_t)0x000)
#define QEI_STATUS_OFFSET                  ((uint32_t)0x004)

#define QEI_POSITION_OFFSET                ((uint32_t)0x008)
#define QEI_MAX_POSITION_OFFSET            ((uint32_t)0x00C)

#define QEI_TIMER_LOAD_OFFSET              ((uint32_t)0x010)
#define QEI_TIMER_OFFSET                   ((uint32_t)0x014)

#define QEI_VELOCITY_COUNTER_OFFSET        ((uint32_t)0x018)
#define QEI_VELOCITY_OFFSET                ((uint32_t)0x01C)

#define QEI_INTERRUPT_EN_OFFSET            ((uint32_t)0x020)
#define QEI_INTERRUPT_RAWS_OFFSET          ((uint32_t)0x024)
#define QEI_INTERRUPT_STATUS_CLEAR_OFFSET  ((uint32_t)0x028)

#endif
