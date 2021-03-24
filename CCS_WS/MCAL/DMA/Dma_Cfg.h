/*
 * dma_cfg.h
 *
 *  Created on: Aug 11, 2020
 *      Author: ahmed
 */

#ifndef HAL_DMA_DMA_CFG_H_
#define HAL_DMA_DMA_CFG_H_

#include "types.h"

#define DMA_CHANNEL_CTL_STRUCT_SIZE (1024/sizeof(Dma_Channel_Cfg_t))
#define DMA_CFG_CHANNELS_NUM        (0x02)

#define DMA_HMI_UART4_RX_CH      18
#define DMA_HMI_UART4_TX_CH      19

typedef struct
{
   void *src_end_address;
   void *des_end_address;
   struct /*Channel Control Word Configuration*/
   {
      uint32_t XFERMODE:3;
      uint32_t NXTUSEBURST:1;
      uint32_t XFERSIZE:10;
      uint32_t ARBSIZE:4;
      uint32_t ctl_reserved:6;
      uint32_t SRCSIZE:2;
      uint32_t SRCINC:2;
      uint32_t DSTSIZE:2;
      uint32_t DSTINC:2;
   }ctl;
   uint32_t unused_spare;
}Dma_Channel_Cfg_t;

typedef Dma_Channel_Cfg_t *const Dma_Channel_Cfg_Ptr_t;

typedef uint8_t Dma_Ch_Id_t;

typedef struct
{
   Dma_Channel_Cfg_Ptr_t base_ptr; /*The base address must be aligned on a 1024-byte boundary.*/
   struct
   {
      Dma_Ch_Id_t id;
      uint32_t assignment;
   }ch[DMA_CFG_CHANNELS_NUM];
}Dma_Cfg_t;

extern const Dma_Cfg_t Dma_Cfg;

#endif /* HAL_DMA_DMA_CFG_H_ */
