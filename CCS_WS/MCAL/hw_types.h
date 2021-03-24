/*
 * hw_types.h
 *
 *  Created on: Aug 5, 2017
 *      Author: Ahmed Yusri
 */

#ifndef HW_TYPES_H_
#define HW_TYPES_H_

#define DATA_SYNC_BARRIER()      __asm(" DSB \n")

#define DISABLE_GINT() __asm("    mrs     r0, PRIMASK\n"\
"    cpsid   i\n")

#define ENABLE_GINT()    __asm("    mrs     r0, PRIMASK\n"\
"    cpsie   i\n")

#define READ_REG_32BIT(add)                          (*(volatile unsigned  int *)(add))
#define WRITE_REG_32BIT(add , value)                 (*(volatile unsigned int *)(add) = (value))

#define WRITE_REG32_MASKED(add , value , mask)       WRITE_REG_32BIT(add,\
                                                                    (READ_REG_32BIT(add) & ~mask)|(value & mask))
#define READ_REG32_MASKED(add , mask)                (READ_REG_32BIT(add) & mask)

#define SET_BIT(REG,BIT_NUM)                        (*(volatile unsigned int *)(REG) |= (1 << (BIT_NUM)))
#define CLEAR_BIT(REG,BIT_NUM)                      (*(volatile unsigned int *)(REG) &= ~(1 << (BIT_NUM)))

#define TOGGLE_BIT(REG,BIT_NUM)                     (*(volatile unsigned int *)REG ^= (1<<(BIT_NUM)))

#define BIT_IS_SET(REG,BIT_NUM)                     ((*(volatile unsigned int *)REG & (1<<(BIT_NUM))) >>  (BIT_NUM))
#define BIT_IS_CLEAR(REG,BIT_NUM)                   ((*(volatile unsigned int *)REG & (1<<(BIT_NUM))) == 0)

#endif /* HW_TYPES_H_ */
