/*
 * Port_MemMap.h
 *
 *  Created on: Oct 23, 2020
 *      Author: Ahmed Yusri Mohamed Ezzat.
 */

/*Port Driver data section.*/
#ifdef PORT_MEMMAP_START_SEC_VAR
#undef PORT_MEMMAP_START_SEC_VAR
#pragma SET_DATA_SECTION(".PortDataSection")
#endif

#ifdef PORT_MEMMAP_STOP_SEC_VAR
#undef PORT_MEMMAP_STOP_SEC_VAR
#pragma SET_DATA_SECTION()
#endif

/*Port Driver data section.*/
#ifdef PORT_MEMMAP_START_SEC_INIT_ZERO_VAR
#undef PORT_MEMMAP_START_SEC_INIT_ZERO_VAR
#pragma SET_DATA_SECTION(".PortBSSSection")
#endif

#ifdef PORT_MEMMAP_STOP_SEC_INIT_ZERO_VAR
#undef PORT_MEMMAP_STOP_SEC_INIT_ZERO_VAR
#pragma SET_DATA_SECTION()
#endif

/*Port Driver code section.*/
#ifdef PORT_MEMMAP_START_SEC_CODE
#undef PORT_MEMMAP_START_SEC_CODE
#pragma SET_CODE_SECTION(".PortCodeSection")
#endif

#ifdef PORT_MEMMAP_STOP_SEC_CODE
#undef PORT_MEMMAP_STOP_SEC_CODE
#pragma SET_CODE_SECTION()
#endif

/*Port Driver const section.*/
#ifdef PORT_MEMMAP_START_CONST_VAR
#undef PORT_MEMMAP_START_CONST_VAR
#pragma SET_DATA_SECTION(".PortConstDataSection")
#endif

#ifdef PORT_MEMMAP_STOP_SEC_CONST_VAR
#undef PORT_MEMMAP_STOP_SEC_CONST_VAR
#pragma SET_DATA_SECTION()
#endif

