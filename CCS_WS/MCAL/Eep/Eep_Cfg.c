/*
 * Eep_Cfg.c
 *
 *  Created on: Nov 4, 2020
 *      Author: Ahmed Yusri Mohammed Ezzat.
 */

#include "Eep.h"
#include "Eep_Cbk.h"

#define EEP_MEMMAP_START_CONST_VAR
#include "Eep_MemMap.h"

const Eep_ConfigType Eep_Cfg =\
{
   EEP_WORD_SIZE_IN_BYTES, EEP_BLOCK_SIZE_IN_BYTES, NULL_PTR, NULL_PTR
};

#define EEP_MEMMAP_STOP_SEC_CONST_VAR
#include "Eep_MemMap.h"
