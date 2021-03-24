/*******************************************
 * QEncoder.c
 *
 *  Created on: Jul 13, 2020
 *      Author: Ahmed Yusri Mohamed
 ******************************************/

#include "QEncoder.h"

#define QEI_CH0_GPIO_PORT                    GPIO_PORTD_REG_INFO
#define QEI_CH0_INDEX_PIN                    3 
#define QEI_CH0_PHASEA_PIN                   6
#define QEI_CH0_PHASEB_PIN                   7

#define QEI_CH1_GPIO_PORT                    GPIO_PORTC_REG_INFO
#define QEI_CH1_INDEX0_PIN                   4 
#define QEI_CH1_PHASEA_PIN                   5
#define QEI_CH1_PHASEB_PIN                   6

#define QEI_GET_CH0_INDEX_GPIO_INFO()        QEI_CH0_GPIO_PORT, QEI_CH0_INDEX_PIN , 6
#define QEI_GET_CH0_PHASEA_GPIO_INFO()       QEI_CH0_GPIO_PORT, QEI_CH0_PHASEA_PIN, 6
#define QEI_GET_CH0_PHASEB_GPIO_INFO()       QEI_CH0_GPIO_PORT, QEI_CH0_PHASEB_PIN, 6

#define QEI_GET_CH1_INDEX_GPIO_INFO()        QEI_CH1_GPIO_PORT, QEI_CH1_INDEX0_PIN, 6
#define QEI_GET_CH1_PHASEA_GPIO_INFO()       QEI_CH1_GPIO_PORT, QEI_CH1_PHASEA_PIN, 6
#define QEI_GET_CH1_PHASEB_GPIO_INFO()       QEI_CH1_GPIO_PORT, QEI_CH1_PHASEB_PIN, 6

/*****************************************
 *        <QEncoder_Init>
 * In : id, period, max_pos
 * Out: None
 * return: Std_ReturnType
 * description:
 *             Initialize the QEI module
 *****************************************/
Std_ReturnType QEncoder_Init(QEncoderId_t id, uint32_t period, uint32_t max_pos)
{
   const uint32_t qei_base[2] = {QEI0_BASE_ADDRESS, QEI1_BASE_ADDRESS};

   WRITE_REG_32BIT(qei_base[id]+
                   QEI_CONTROL_OFFSET ,
                    0x38);

   WRITE_REG_32BIT(qei_base[id]+
                   QEI_MAX_POSITION_OFFSET,
                    max_pos);

   WRITE_REG_32BIT(qei_base[id]+
                   QEI_TIMER_LOAD_OFFSET,
                   period-1);

   SET_BIT(qei_base[id]+QEI_INTERRUPT_EN_OFFSET,  /*Enable timer expire interrupt..*/
           1); 

   SET_BIT(qei_base[id]+QEI_CONTROL_OFFSET,  /*Enable quadrature encoder..*/
          0);

   return E_OK;
}/*QEncoder_Init*/

/*****************************************
 *        <QEncoder_Read_Position>
 * In : QEncoderId_t, period
 * Out: None
 * return: Std_ReturnType
 * description:
 *  Read the encoder position by 
 *  reading the QEI Position (QEIPOS) register value
 *****************************************/
uint32_t QEncoder_Read_Position(const uint32_t qei_ch)
{
   return READ_REG_32BIT(qei_ch + QEI_POSITION_OFFSET);
}/*QEncoder_Read_Position*/
