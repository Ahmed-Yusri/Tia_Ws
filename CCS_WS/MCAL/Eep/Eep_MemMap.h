/*
 * Eep_MemMap.h
 *
 *  Created on: Nov 4, 2020
 *      Author: Ahmed Yusri Mohammed Ezzat.
 */

/*EEP Driver data section.*/
#ifdef EEP_MEMMAP_START_SEC_VAR
#undef EEP_MEMMAP_START_SEC_VAR
#pragma SET_DATA_SECTION(".EEPDataSection")
#endif

#ifdef EEP_MEMMAP_STOP_SEC_VAR
#undef EEP_MEMMAP_STOP_SEC_VAR
#pragma SET_DATA_SECTION()
#endif

/*EEP Driver data section.*/
#ifdef EEP_MEMMAP_START_SEC_INIT_ZERO_VAR
#undef EEP_MEMMAP_START_SEC_INIT_ZERO_VAR
#pragma SET_DATA_SECTION(".EEPBSSSection")
#endif

#ifdef EEP_MEMMAP_STOP_SEC_INIT_ZERO_VAR
#undef EEP_MEMMAP_STOP_SEC_INIT_ZERO_VAR
#pragma SET_DATA_SECTION()
#endif

/*EEP Driver code section.*/
#ifdef EEP_MEMMAP_START_SEC_CODE
#undef EEP_MEMMAP_START_SEC_CODE
#pragma SET_CODE_SECTION(".EEPCodeSection")
#endif

#ifdef EEP_MEMMAP_STOP_SEC_CODE
#undef EEP_MEMMAP_STOP_SEC_CODE
#pragma SET_CODE_SECTION()
#endif

/*EEP Driver const section.*/
#ifdef EEP_MEMMAP_START_CONST_VAR
#undef EEP_MEMMAP_START_CONST_VAR
#pragma SET_DATA_SECTION(".EEPConstDataSection")
#endif

#ifdef EEP_MEMMAP_STOP_SEC_CONST_VAR
#undef EEP_MEMMAP_STOP_SEC_CONST_VAR
#pragma SET_DATA_SECTION()
#endif
