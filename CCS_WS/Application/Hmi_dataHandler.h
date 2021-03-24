/*
 * Hmi_dataHandler.h
 *
 *  Created on: Feb 1, 2021
 *      Author: Ahmed Yusri Mohamed
 */

#ifndef APPLICATION_HMI_DATAHANDLER_H_
#define APPLICATION_HMI_DATAHANDLER_H_

#include "Hmi_Screen_Cfg.h"
#include "OS_Cfg.h"
#include "OS.h"

#define MACHINE_TEMP_CONTROL_UNITS_COUNT                          ((uint8_t)12)
#define MACHINE_TEMP_DES_BASE_ADDRESS                             ((uint32_t)0x080000)
#define MACHINE_TEMP_REAL_BASE_ADDRESS                            ((uint32_t)0x080018)
#define MACHINE_TEMP_STATE_BASE_ADDRESS                           ((uint32_t)0x080030)

#define MACHINE_BLOWING_CONTROL_PARAM_COUNT                       ((uint8_t)5)
#define MACHINE_BLOWING_CONTROL_BASE_ADDRESS                      ((uint32_t)0x080048)

#define MACHINE_BLOWING_PIN_RAISING_AT_CLAMP_CLOSE                ((uint16_t)0X01)
#define MACHINE_BLOWING_PIN_RAISING_AT_CLAMP_OPEN                 ((uint16_t)0X00)


#define MACHINE_MOLD_CONTROL_PARAM_COUNT                          ((uint8_t)12)
#define MACHINE_MOLD_CONTROL_BASE_ADDRESS                         ((uint32_t)0x080052)

#define MACHINE_MACHINE_STATE_PARAM_COUNT                         ((uint8_t)3)
#define MACHINE_MACHINE_STATE_BASE_ADDRESS                        ((uint32_t)0x08006A)

#define MACHINE_MACHINE_MCONTROL_PARAM_COUNT                      ((uint8_t)6)
#define MACHINE_MACHINE_MCONTROL_BASE_ADDRESS                     ((uint32_t)0x080070)

#define MACHINE_HEATER_ON_VAL                                     ((uint16_t)48815)
#define MACHINE_HEATER_OFF_VAL                                    ((uint16_t)0)

#define MACHINE_EXTRUDER_ON_VAL                                   MACHINE_HEATER_ON_VAL
#define MACHINE_EXTRUDER_OFF_VAL                                  MACHINE_HEATER_OFF_VAL


#define MACHINE_MAIN_STAE_RESET_VAL                               ((uint16_t)21845)
#define MACHINE_MAIN_STAE_MANUAL_VAL                              ((uint16_t)43690)
#define MACHINE_MAIN_STAE_AUTO_VAL                                ((uint16_t)57005)

typedef union
{
   struct
   {
      uint16_t state[MACHINE_TEMP_CONTROL_UNITS_COUNT];
      uint16_t real[MACHINE_TEMP_CONTROL_UNITS_COUNT];
      uint16_t des[MACHINE_TEMP_CONTROL_UNITS_COUNT];
   }u16;
   uint8_t u8[3 * sizeof(uint16_t) * MACHINE_TEMP_CONTROL_UNITS_COUNT];
}Hmi_tempDataType;

typedef union
{
   struct
   {
      uint16_t delayBefore;
      uint16_t blowingTime;
      uint16_t exhaustTime;
      uint16_t delayAfter;
      uint16_t pinFunc;
   }u16;
   uint8_t u8[sizeof(uint16_t) * MACHINE_BLOWING_CONTROL_PARAM_COUNT];
}Hmi_blowingDataType;

typedef union
{
   struct
   {
      uint16_t carEndSpeed;
      uint16_t carMidSpeed;
      uint16_t carStartSpeed;
      uint16_t clampEndSpeed;
      uint16_t clampMidSpeed;
      uint16_t clampStartSpeed;
      uint16_t carEndPos;
      uint16_t carMidPos;
      uint16_t carStartPos;
      uint16_t clampEndPos;
      uint16_t clampMidPos;
      uint16_t clampStartPos;
   }u16;
   uint8_t u8[sizeof(uint16_t) * MACHINE_MOLD_CONTROL_PARAM_COUNT];
}Hmi_moldDataType;

typedef union
{
   struct
   {
      uint16_t carPosition;
      uint16_t clampPosition;
      uint16_t cycleCount;
   }u16;
   uint8_t u8[sizeof(uint16_t) * MACHINE_MACHINE_STATE_PARAM_COUNT];
}Hmi_machineStateDataType;

typedef union
{
   struct
   {
      uint16_t mainState;
      uint16_t extruderState;
      uint16_t heaterState;
      uint16_t carPos;
      uint16_t clampPos;
      uint16_t extruderSpeed;
   }u16;
   uint8_t u8[sizeof(uint16_t) * MACHINE_MACHINE_MCONTROL_PARAM_COUNT];
}Hmi_machineControlDataType;


#endif /* APPLICATION_HMI_DATAHANDLER_H_ */
