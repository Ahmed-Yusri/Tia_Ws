/*
 * dma.c
 *
 *  Created on: Aug 10, 2020
 *      Author: Ahmed Yusri Mohamed
 */

#include <DMA/Dma.h>

static struct
{
   const Dma_Cfg_t *cfg_ptr;
   enum
   {
      E_DMA_MODULE_IDLE,
      E_DMA_MODULE_BUSY
   }state;
}Dma_Global = {
   .cfg_ptr = NULL_PTR,
   .state = E_DMA_MODULE_IDLE
};

/*************************************************************************
 *        <Dma_Init>
 * In : Dma_Cfg_t *dma_cfg
 * Out: None
 * return: Std_ReturnType
 * description: Initialize the Dma Module.
 ***************************************************************************/
Std_ReturnType Dma_Init(const Dma_Cfg_t *dma_cfg)
{
   Std_ReturnType result = E_OK;
#ifdef IN_DEV_MODE
   if(dma_cfg->base_ptr % 1024) /*The base address must be aligned on a 1024-byte boundary.*/
   {
      result = E_NOT_OK;
   }
   else
#endif
   {
      uint8_t cfg_idx;
      volatile uint32_t *dma_ch_map = (volatile uint32_t*)DMACHMAP0;

      SET_BIT(DMACFG, 0); /*Enables μDMA controller.*/

      WRITE_REG_32BIT(DMACTLBASE,  /*Program the location of the channel control table*/
                     (uint32_t)dma_cfg->base_ptr);

      for(cfg_idx = 0; cfg_idx < DMA_CFG_CHANNELS_NUM; cfg_idx++)
      {
         uint32_t ch_offset = dma_cfg->ch[cfg_idx].id / 8; /* >> 3..*/                            /*    << 3*/
         dma_ch_map[ch_offset] |= (dma_cfg->ch[cfg_idx].assignment << ((dma_cfg->ch[cfg_idx].id-(ch_offset*8))<<2));
      }
      Dma_Global.cfg_ptr = dma_cfg;
   }
   return result;
}/*Dma_Init*/

/*************************************************************************
 *        <Dma_Setup_PrimaryChannel>
 * In : Dma_Ch_Id_t idx, Dma_Channel_Cfg_t *ch_cfg
 * Out: None
 * return: Std_ReturnType
 * description: Setup the Dma channel.
 ***************************************************************************/
Std_ReturnType Dma_Setup_PrimaryChannel(Dma_Ch_Id_t idx, Dma_Channel_Cfg_t *ch_cfg)
{
   Std_ReturnType result = E_OK;
#ifdef IN_DEV_MODE
   if((NULL_PTR == ch_cfg)||
      (idx > 31))
   {
      result = E_NOT_OK;
   }
   else
#endif
   {
      SET_BIT(DMAPRIOCLR,  /*DMA Channel Default priority..*/
              idx);

      SET_BIT(DMAALTCLR,   /*Select the primary channel control structure for this transfer...*/
              idx);

      SET_BIT(DMAUSEBURSTCLR, /*Allow the uDMA controller to respond to single and burst requests.*/
              idx);

      SET_BIT(DMAREQMASKCLR, /* Allow the uDMA controller to recognize requests for this channel.*/
              idx);

      Dma_Global.cfg_ptr->base_ptr[idx] = *ch_cfg; /*Update the channel configuration..*/
      
      SET_BIT(DMAENASET, idx); /*DMA Channel [idx] is enabled.*/
   }
   return result;
}/*Dma_Setup_PrimaryChannel*/

/*************************************************************************
 *        <Dma_Deactivate_Channel>
 * In : Dma_Ch_Id_t idx
 * Out: None
 * return: Std_ReturnType
 * description: Stop a dma channel.
 ***************************************************************************/
Std_ReturnType Dma_Deactivate_PrimaryChannel(Dma_Ch_Id_t idx)
{
   Std_ReturnType result = E_OK;
#ifdef IN_DEV_MODE
   if(idx > 31)
   {
      result = E_NOT_OK;
   }
   else
#endif
   {
      SET_BIT(DMAENACLR, idx); /*Setting a bit clears the corresponding SET[idx] bit in the
                                 DMAENASET register meaning that channel [idx] is disabled for uDMA transfers*/
   }
   return result;
}/*Dma_Deactivate_PrimaryChannel*/

/*************************************************************************
 *        <Dma_getChannelState>
 * In : Dma_Ch_Id_t idx
 * Out: None
 * return: Dma_State_t
 * description: get DMA channel STATE.
 ***************************************************************************/
Dma_State_t Dma_getChannelState(Dma_Ch_Id_t idx)
{
   Dma_State_t state = E_DMA_IDLE;
   if(Dma_Global.cfg_ptr->base_ptr[idx].ctl.XFERMODE)
   {
      state = E_DMA_RUNNING;
   }

   return state;
}/*Dma_getChannelState*/
