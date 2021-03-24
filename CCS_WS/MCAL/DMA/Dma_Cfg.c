/*
 * dma_cfg.c
 *
 *  Created on: Aug 11, 2020
 *      Author: Ahmed Yusri Mohamed
 */

#include "Dma_Cfg.h"

volatile static Dma_Channel_Cfg_t Dma_Channel_Ctl_Table[DMA_CHANNEL_CTL_STRUCT_SIZE] __attribute__ ((aligned(1024)));

const Dma_Cfg_t Dma_Cfg = /*Dma Configuration struct..*/
{
   .base_ptr = (Dma_Channel_Cfg_t*)&Dma_Channel_Ctl_Table,
   {/*uDMA Id,                assignment*/
      { DMA_HMI_UART4_RX_CH, 2},
      { DMA_HMI_UART4_TX_CH, 2}
   }
};

volatile static Dma_Channel_Cfg_t Dma_Channel_Ctl_Table[DMA_CHANNEL_CTL_STRUCT_SIZE] =\
{
   { /*Channel 0..*/
      .src_end_address = NULL_PTR,
      .des_end_address = NULL_PTR,
      .ctl = {0},
      .unused_spare = 0
   },
   { /*Channel 1..*/
      .src_end_address = NULL_PTR,
      .des_end_address = NULL_PTR,
      .ctl = {0},
      .unused_spare = 0
   },
   {
      .src_end_address = NULL_PTR,
      .des_end_address = NULL_PTR,
      .ctl = {0},
      .unused_spare = 0
   },
   {
      .src_end_address = NULL_PTR,
      .des_end_address = NULL_PTR,
      .ctl = {0},
      .unused_spare = 0
   },
   {
      .src_end_address = NULL_PTR,
      .des_end_address = NULL_PTR,
      .ctl = {0},
      .unused_spare = 0
   },
   {
      .src_end_address = NULL_PTR,
      .des_end_address = NULL_PTR,
      .ctl = {0},
      .unused_spare = 0
   },
   {
      .src_end_address = NULL_PTR,
      .des_end_address = NULL_PTR,
      .ctl = {0},
      .unused_spare = 0
   },
   {
      .src_end_address = NULL_PTR,
      .des_end_address = NULL_PTR,
      .ctl = {0},
      .unused_spare = 0
   },
   {
      .src_end_address = NULL_PTR,
      .des_end_address = NULL_PTR,
      .ctl = {0},
      .unused_spare = 0
   },
   {
      .src_end_address = NULL_PTR,
      .des_end_address = NULL_PTR,
      .ctl = {0},
      .unused_spare = 0
   },
   {
      .src_end_address = NULL_PTR,
      .des_end_address = NULL_PTR,
      .ctl = {0},
      .unused_spare = 0
   },
   {
      .src_end_address = NULL_PTR,
      .des_end_address = NULL_PTR,
      .ctl = {0},
      .unused_spare = 0
   },
   {
      .src_end_address = NULL_PTR,
      .des_end_address = NULL_PTR,
      .ctl = {0},
      .unused_spare = 0
   },
   {
      .src_end_address = NULL_PTR,
      .des_end_address = NULL_PTR,
      .ctl = {0},
      .unused_spare = 0
   },
   {
      .src_end_address = NULL_PTR,
      .des_end_address = NULL_PTR,
      .ctl = {0},
      .unused_spare = 0
   },
   {
      .src_end_address = NULL_PTR,
      .des_end_address = NULL_PTR,
      .ctl = {0},
      .unused_spare = 0
   },
   {
      .src_end_address = NULL_PTR,
      .des_end_address = NULL_PTR,
      .ctl = {0},
      .unused_spare = 0
   },
   {
      .src_end_address = NULL_PTR,
      .des_end_address = NULL_PTR,
      .ctl = {0},
      .unused_spare = 0
   },
   {
      .src_end_address = NULL_PTR,
      .des_end_address = NULL_PTR,
      .ctl = {0},
      .unused_spare = 0
   },
   {
      .src_end_address = NULL_PTR,
      .des_end_address = NULL_PTR,
      .ctl = {0},
      .unused_spare = 0
   },
   {
      .src_end_address = NULL_PTR,
      .des_end_address = NULL_PTR,
      .ctl = {0},
      .unused_spare = 0
   },
   {
      .src_end_address = NULL_PTR,
      .des_end_address = NULL_PTR,
      .ctl = {0},
      .unused_spare = 0
   },
   {
      .src_end_address = NULL_PTR,
      .des_end_address = NULL_PTR,
      .ctl = {0},
      .unused_spare = 0
   },
   {
      .src_end_address = NULL_PTR,
      .des_end_address = NULL_PTR,
      .ctl = {0},
      .unused_spare = 0
   },
   {
      .src_end_address = NULL_PTR,
      .des_end_address = NULL_PTR,
      .ctl = {0},
      .unused_spare = 0
   },
   {
      .src_end_address = NULL_PTR,
      .des_end_address = NULL_PTR,
      .ctl = {0},
      .unused_spare = 0
   },
   {
      .src_end_address = NULL_PTR,
      .des_end_address = NULL_PTR,
      .ctl = {0},
      .unused_spare = 0
   },
   {
      .src_end_address = NULL_PTR,
      .des_end_address = NULL_PTR,
      .ctl = {0},
      .unused_spare = 0
   },
   {
      .src_end_address = NULL_PTR,
      .des_end_address = NULL_PTR,
      .ctl = {0},
      .unused_spare = 0
   },
   {
      .src_end_address = NULL_PTR,
      .des_end_address = NULL_PTR,
      .ctl = {0},
      .unused_spare = 0
   },
   {
      .src_end_address = NULL_PTR,
      .des_end_address = NULL_PTR,
      .ctl = {0},
      .unused_spare = 0
   },
   {
      .src_end_address = NULL_PTR,
      .des_end_address = NULL_PTR,
      .ctl = {0},
      .unused_spare = 0
   },
   { /*Channel 0 Alt*/
      .src_end_address = NULL_PTR,
      .des_end_address = NULL_PTR,
      .ctl = {0},
      .unused_spare = 0
   },
   { /*Channel 1 Alt*/
      .src_end_address = NULL_PTR,
      .des_end_address = NULL_PTR,
      .ctl = {0},
      .unused_spare = 0
   },
   {
      .src_end_address = NULL_PTR,
      .des_end_address = NULL_PTR,
      .ctl = {0},
      .unused_spare = 0
   },
   {
      .src_end_address = NULL_PTR,
      .des_end_address = NULL_PTR,
      .ctl = {0},
      .unused_spare = 0
   },
   {
      .src_end_address = NULL_PTR,
      .des_end_address = NULL_PTR,
      .ctl = {0},
      .unused_spare = 0
   },
   {
      .src_end_address = NULL_PTR,
      .des_end_address = NULL_PTR,
      .ctl = {0},
      .unused_spare = 0
   },
   {
      .src_end_address = NULL_PTR,
      .des_end_address = NULL_PTR,
      .ctl = {0},
      .unused_spare = 0
   },
   {
      .src_end_address = NULL_PTR,
      .des_end_address = NULL_PTR,
      .ctl = {0},
      .unused_spare = 0
   },
   {
      .src_end_address = NULL_PTR,
      .des_end_address = NULL_PTR,
      .ctl = {0},
      .unused_spare = 0
   },
   {
      .src_end_address = NULL_PTR,
      .des_end_address = NULL_PTR,
      .ctl = {0},
      .unused_spare = 0
   },
   {
      .src_end_address = NULL_PTR,
      .des_end_address = NULL_PTR,
      .ctl = {0},
      .unused_spare = 0
   },
   {
      .src_end_address = NULL_PTR,
      .des_end_address = NULL_PTR,
      .ctl = {0},
      .unused_spare = 0
   },
   {
      .src_end_address = NULL_PTR,
      .des_end_address = NULL_PTR,
      .ctl = {0},
      .unused_spare = 0
   },
   {
      .src_end_address = NULL_PTR,
      .des_end_address = NULL_PTR,
      .ctl = {0},
      .unused_spare = 0
   },
   {
      .src_end_address = NULL_PTR,
      .des_end_address = NULL_PTR,
      .ctl = {0},
      .unused_spare = 0
   },
   {
      .src_end_address = NULL_PTR,
      .des_end_address = NULL_PTR,
      .ctl = {0},
      .unused_spare = 0
   },
   {
      .src_end_address = NULL_PTR,
      .des_end_address = NULL_PTR,
      .ctl = {0},
      .unused_spare = 0
   },
   {
      .src_end_address = NULL_PTR,
      .des_end_address = NULL_PTR,
      .ctl = {0},
      .unused_spare = 0
   },
   {
      .src_end_address = NULL_PTR,
      .des_end_address = NULL_PTR,
      .ctl = {0},
      .unused_spare = 0
   },
   {
      .src_end_address = NULL_PTR,
      .des_end_address = NULL_PTR,
      .ctl = {0},
      .unused_spare = 0
   },
   {
      .src_end_address = NULL_PTR,
      .des_end_address = NULL_PTR,
      .ctl = {0},
      .unused_spare = 0
   },
   {
      .src_end_address = NULL_PTR,
      .des_end_address = NULL_PTR,
      .ctl = {0},
      .unused_spare = 0
   },
   {
      .src_end_address = NULL_PTR,
      .des_end_address = NULL_PTR,
      .ctl = {0},
      .unused_spare = 0
   },
   {
      .src_end_address = NULL_PTR,
      .des_end_address = NULL_PTR,
      .ctl = {0},
      .unused_spare = 0
   },
   {
      .src_end_address = NULL_PTR,
      .des_end_address = NULL_PTR,
      .ctl = {0},
      .unused_spare = 0
   },
   {
      .src_end_address = NULL_PTR,
      .des_end_address = NULL_PTR,
      .ctl = {0},
      .unused_spare = 0
   },
   {
      .src_end_address = NULL_PTR,
      .des_end_address = NULL_PTR,
      .ctl = {0},
      .unused_spare = 0
   },
   {
      .src_end_address = NULL_PTR,
      .des_end_address = NULL_PTR,
      .ctl = {0},
      .unused_spare = 0
   },
   {
      .src_end_address = NULL_PTR,
      .des_end_address = NULL_PTR,
      .ctl = {0},
      .unused_spare = 0
   },
   {
      .src_end_address = NULL_PTR,
      .des_end_address = NULL_PTR,
      .ctl = {0},
      .unused_spare = 0
   },
   {
      .src_end_address = NULL_PTR,
      .des_end_address = NULL_PTR,
      .ctl = {0},
      .unused_spare = 0
   },
   {
      .src_end_address = NULL_PTR,
      .des_end_address = NULL_PTR,
      .ctl = {0},
      .unused_spare = 0
   }
};
