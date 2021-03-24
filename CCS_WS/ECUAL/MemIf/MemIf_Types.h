/*
 * MemIf_Types.h
 *
 *  Created on: Nov 4, 2020
 *      Author: Ahmed Yusri Mohammed Ezzat.
 */

#ifndef ECUAL_MEMIF_TYPES_H_
#define ECUAL_MEMIF_TYPES_H_

/*MemIf_StatusType*/
#define MEMIF_UNINIT                ((MemIf_StatusType)0)
#define MEMIF_IDLE                  ((MemIf_StatusType)1)
#define MEMIF_BUSY                  ((MemIf_StatusType)2)
#define MEMIF_BUSY_INTERNAL         ((MemIf_StatusType)3)


/*MemIf_JobResultType*/
#define MEMIF_JOB_OK                ((MemIf_JobResultType)0)
#define MEMIF_JOB_FAILED            ((MemIf_JobResultType)1)
#define MEMIF_JOB_PENDING           ((MemIf_JobResultType)2)
#define MEMIF_JOB_CANCELLED         ((MemIf_JobResultType)3)
#define MEMIF_BLOCK_INCONSISTENT    ((MemIf_JobResultType)4)
#define MEMIF_BLOCK_INVALID         ((MemIf_JobResultType)5)


typedef uint8 MemIf_StatusType;
typedef uint8 MemIf_JobResultType;

#endif /* ECUAL_MEMIF_TYPES_H_ */
