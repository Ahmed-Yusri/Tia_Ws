/*
 * Dio_MemMap.h
 *
 *  Created on: Oct 25, 2020
 *      Author: Ahmed Yusri Mohamed Ezzat.
 */


/*DIO Driver data section.*/
#ifdef DIO_MEMMAP_START_SEC_VAR
#undef DIO_MEMMAP_START_SEC_VAR
#pragma SET_DATA_SECTION(".DIODataSection")
#endif

#ifdef DIO_MEMMAP_STOP_SEC_VAR
#undef DIO_MEMMAP_STOP_SEC_VAR
#pragma SET_DATA_SECTION()
#endif

/*DIO Driver data section.*/
#ifdef DIO_MEMMAP_START_SEC_INIT_ZERO_VAR
#undef DIO_MEMMAP_START_SEC_INIT_ZERO_VAR
#pragma SET_DATA_SECTION(".DIOBSSSection")
#endif

#ifdef DIO_MEMMAP_STOP_SEC_INIT_ZERO_VAR
#undef DIO_MEMMAP_STOP_SEC_INIT_ZERO_VAR
#pragma SET_DATA_SECTION()
#endif

/*DIO Driver code section.*/
#ifdef DIO_MEMMAP_START_SEC_CODE
#undef DIO_MEMMAP_START_SEC_CODE
#pragma SET_CODE_SECTION(".DIOCodeSection")
#endif

#ifdef DIO_MEMMAP_STOP_SEC_CODE
#undef DIO_MEMMAP_STOP_SEC_CODE
#pragma SET_CODE_SECTION()
#endif

/*DIO Driver const section.*/
#ifdef DIO_MEMMAP_START_CONST_VAR
#undef DIO_MEMMAP_START_CONST_VAR
#pragma SET_DATA_SECTION(".DIOConstDataSection")
#endif

#ifdef DIO_MEMMAP_STOP_SEC_CONST_VAR
#undef DIO_MEMMAP_STOP_SEC_CONST_VAR
#pragma SET_DATA_SECTION()
#endif

