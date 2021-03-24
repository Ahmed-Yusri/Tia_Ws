/*
 * hw_dma.h
 *
 *  Created on: Aug 10, 2020
 *      Author: Ahmed Yusri Mohamed
 */

#ifndef HAL_DMA_HW_DMA_H_
#define HAL_DMA_HW_DMA_H_

#define DMACFG          0x400FF004UL
#define DMACTLBASE      0x400FF008UL
#define DMAENACLR       0x400FF02CUL
#define DMAENASET       0x400FF028UL
#define DMAPRIOSET      0x400FF038UL
#define DMAPRIOCLR      0x400FF03CUL
#define DMAUSEBURSTCLR  0x400FF01CUL
#define DMAALTCLR       0x400FF034UL
#define DMAREQMASKCLR   0x400FF024UL
#define DMACHMAP0       0x400FF510UL
#define DMACHASGN       0x400FF500UL

#endif /* HAL_DMA_HW_DMA_H_ */
