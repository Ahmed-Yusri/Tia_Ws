/******************************************
 * QEncoder.h
 *
 *  Created on: Jul 13, 2020
 *      Author: Ahmed Yusri Moahmmed
 ***************************************/

#ifndef HAL_QENCODER_H_
#define HAL_QENCODER_H_

#include "types.h"
#include "TM4C129E_HwGpioInfo.h"
#include "Hw_QEncoder.h"
#include "hw_types.h"
#include "system.h"

typedef enum
{
    E_QENCODER_CHANNEL0,
    E_QENCODER_CHANNEL1,
    E_QENCODER_INVALID_CH
}QEncoderId_t;


inline void QEI_InterrupetClear(const uint32_t qei_ch)
{
    WRITE_REG_32BIT(qei_ch + QEI_INTERRUPT_STATUS_CLEAR_OFFSET, 0x0F);
}/*QEI_InterrupetClear*/

inline uint32_t QEI_Get_Speed(const uint32_t qei_ch)
{
    return READ_REG_32BIT(qei_ch+QEI_VELOCITY_OFFSET);
}/*QEI_Get_Speed*/

/*****************************************
 *        <QEncoder_Init>
 * In : id, period, max_pos
 * Out: None
 * return: Std_ReturnType
 * description:
 *             Initialize the QEI module
 *****************************************/
Std_ReturnType QEncoder_Init(QEncoderId_t id, uint32_t period, uint32_t max_pos);

/*****************************************
 *        <QEncoder_Read_Position>
 * In : QEncoderId_t, period
 * Out: None
 * return: Std_ReturnType
 * description:
 *  Read the encoder position by 
 *  reading the QEI Position (QEIPOS) register value
 *****************************************/
uint32_t QEncoder_Read_Position(const uint32_t qei_ch);

#endif /* HAL_QENCODER_H_ */
