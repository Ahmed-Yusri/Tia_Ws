/*
 * dma.h
 *
 *  Created on: Aug 10, 2020
 *      Author: Ahmed Yusri Mohamed
 */

#ifndef HAL_DMA_DMA_H_
#define HAL_DMA_DMA_H_

#include "Dma_Cfg.h"
#include "hw_dma.h"
#include "hw_types.h"
#include "types.h"

typedef enum
{
    E_DMA_IDLE,
    E_DMA_RUNNING
}Dma_State_t;

/*************************************************************************
 *        <Dma_Init>
 * In : Dma_Cfg_t *dma_cfg
 * Out: None
 * return: Std_ReturnType
 * description: Initialize the Dma Module.
 ***************************************************************************/
Std_ReturnType Dma_Init(const Dma_Cfg_t *dma_cfg);

/*************************************************************************
 *        <Dma_Setup_PrimaryChannel>
 * In : Dma_Ch_Id_t idx, Dma_Channel_Cfg_t ch_cfg
 * Out: None
 * return: Std_ReturnType
 * description: Setup the Dma channel.
 ***************************************************************************/
Std_ReturnType Dma_Setup_PrimaryChannel(Dma_Ch_Id_t idx, Dma_Channel_Cfg_t *ch_cfg);

/*************************************************************************
 *        <Dma_Deactivate_PrimaryChannel>
 * In : Dma_Ch_Id_t idx
 * Out: None
 * return: Std_ReturnType
 * description: Stop a dma channel.
 ***************************************************************************/
Std_ReturnType Dma_Deactivate_PrimaryChannel(Dma_Ch_Id_t idx);

/*************************************************************************
 *        <Dma_getChannelState>
 * In : Dma_Ch_Id_t idx
 * Out: None
 * return: Dma_State_t
 * description: Stop a dma channel.
 ***************************************************************************/
Dma_State_t Dma_getChannelState(Dma_Ch_Id_t idx);

#endif /* HAL_DMA_DMA_H_ */
