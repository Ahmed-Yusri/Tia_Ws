/*
 * Port.c
 *
 *  Created on: Oct 23, 2020
 *      Author: Ahmed Yusri Mohamed Ezzat.
 */

#include"Port.h"
#include "TM4C129E_HwGpioInfo.h"

/********************************************************
 * PRIV MACROS.
 *******************************************************/
#define PORT_GET_PIN_ID(CFG_PTR)                     ((CFG_PTR)->pinId)
#define PORT_GET_PIN_DIRECTION(CFG_PTR)              ((CFG_PTR)->pinDirection)
#define PORT_GET_PIN_MODE(CFG_PTR)                   ((CFG_PTR)->pinMode)
#define PORT_GET_PIN_DRV_STRENGTH(CFG_PTR)           ((CFG_PTR)->pinDriveStrength)
#define PORT_GET_PIN_INTERN_RES(CFG_PTR)             ((CFG_PTR)->pinRes)
#define PORT_GET_PIN_SLEW_RATE(CFG_PTR)              ((CFG_PTR)->pinSlewRate)
#define PORT_PIN_IS_DIGITAL(CFG_PTR)                 ((CFG_PTR)->pinIsDigital)
#define PORT_GET_PIN_INIT_VAL(CFG_PTR)               ((CFG_PTR)->pinInitVal)
#define PORT_PIN_IS_MODE_CHANGABLE(CFG_PTR)          ((CFG_PTR)->pinModeChangable)
#define PORT_PIN_IS_DIR_CHANGABLE(CFG_PTR)           ((CFG_PTR)->pinDirectionChangable)

/*******************************************************
 * Internal Data types.
 *******************************************************/
typedef enum
{
   E_PORT_UN_INIT,
   E_PORT_INIT_PENDING,
   E_PORT_INIT_DONE
}Port_InternalStateType;

struct Port_InternalVarType
{
   const Port_ConfigType *cfgPtr; /*Point on the active configuration.[Support the Post-Build Cfg]*/
   Port_InternalStateType state; /*global module state..*/
};

#define PORT_MEMMAP_START_SEC_INIT_ZERO_VAR
#include <MCAL/Port/Port_MemMap.h>

static struct Port_InternalVarType Port_InternalVar = {NULL_PTR, E_PORT_UN_INIT};  /*Port Internal Variable.*/

#define PORT_MEMMAP_STOP_SEC_INIT_ZERO_VAR
#include <MCAL/Port/Port_MemMap.h>

#define PORT_MEMMAP_START_SEC_CODE
#include <MCAL/Port/Port_MemMap.h>

/******************************************************************
 * Local Functions
*******************************************************************/

/**************************************************************************************
 * Function Name: <Gpio_GetPortAddrById>
 * Description: return the base address of the requested port.
 * CAUTION:
 *         The caller shall check the portId before using this local function.
 *************************************************************************************/
LOCAL_INLINE Gpio_HwRegisterRefType Gpio_GetPortAddrById(uint8 portId)
{
   const Gpio_HwRegisterRefType gpio_base_lst[] = { GPIO_PORTA_REG_INFO, GPIO_PORTB_REG_INFO, GPIO_PORTC_REG_INFO,
                                                    GPIO_PORTD_REG_INFO, GPIO_PORTE_REG_INFO, GPIO_PORTF_REG_INFO,
                                                    GPIO_PORTG_REG_INFO, GPIO_PORTH_REG_INFO, GPIO_PORTJ_REG_INFO,
                                                    GPIO_PORTK_REG_INFO, GPIO_PORTL_REG_INFO, GPIO_PORTM_REG_INFO,
                                                    GPIO_PORTN_REG_INFO, GPIO_PORTP_REG_INFO, GPIO_PORTQ_REG_INFO
                                                   };
   return gpio_base_lst[portId]; /*Starts from 0.*/
}/*Gpio_SelectPortById*/

/**************************************************************************************
 * Function Name: <Port_Get_CfgInstByPinId>
 * Description: return the configuration refrence.
 * CAUTION:
 *         The caller shall check if the return value is NULL_PTR.
 *************************************************************************************/
LOCAL_INLINE const Port_ConfigType *Port_Get_CfgInstByPinId(Port_PinModeType pinId)
{
   const Port_ConfigType* thisObj = NULL_PTR;
   uint8 pinIdx = 0;

   for(pinIdx = 0; pinIdx < PORT_NUM_OF_CFG_PINS; ++pinIdx)
   {
      if(pinId == PORT_GET_PIN_ID(&Port_InternalVar.cfgPtr[pinIdx]))
      {
         thisObj = &Port_InternalVar.cfgPtr[pinIdx]; /*Capture the correct instance..*/
         break;
      }
   }

   return thisObj;
}/*Port_Get_CfgInstByPinId*/

/**************************************************************************************
 * Function Name: <Port_SetPinFunctionality>
 * Description: Enable this pin digital/analog function & Set the init value.
 *************************************************************************************/
LOCAL_INLINE void Port_SetPinFunctionality(Gpio_HwRegisterRefType gpioBase, uint8 pin, Port_PinInitValueType pinInit, boolean pinIsDigital)
{
   if(TRUE == pinIsDigital)
   {
      if((gpioBase->GPIODIR & pin) == (PORT_PIN_OUT << pin)) /*if this pin is configured as output..*/
      {
          ((uint32 *)gpioBase)[1 << pin] = (pinInit << pin); /*Set the pin to the init val before enabling it.*/
      }
      gpioBase->GPIODEN |= (1 << pin); /*Enable GPIO pins as digital I/Os*/
   }
   else
   {
      /*If any pin is to be used as an ADC input, the appropriate bit in GPIOAMSEL must be
         set to disable the analog isolation circuit.*/

      gpioBase->GPIOAMSEL |= (1 << pin); /*Enable GPIO pins to their analog function.*/
   }
}/*Port_SetPinFunctionality*/

/**************************************************************************************
 * Function Name: <Port_SetPinSlewRate>
 * Description: Enable Pin Slew Rate if req.
 * Remark: Slew rate control is only available when using 
 *         the 8-mA, 10-mA or 12-mA drive strength option.
 *************************************************************************************/
LOCAL_INLINE void Port_SetPinSlewRate(Gpio_HwRegisterRefType gpioBase, uint8 pin, Port_PinSlewRateType pinSlewRate)
{
   if(TRUE == pinSlewRate)
   {
      gpioBase->GPIOSLR |= (1 << pin); /*Set the corsponding bit.*/
   }
   else
   {
      gpioBase->GPIOSLR &= ~(1 << pin); /*Clear the corsponding bit.*/
   }
}/*Port_EnableSlewRate*/

/**************************************************************************************
 * Function Name: <Port_SetPinInternRes>
 * Description: Set pin internal resistor.
 * Remark: Setting a bit in the GPIOPDR register clears the corresponding bit in
 *         the GPIOPUR register. The change is effective on the next clock cycle
 *************************************************************************************/
LOCAL_INLINE void Port_SetPinInternRes(Gpio_HwRegisterRefType gpioBase, uint8 pin,Port_PinInterResType pinRes)
{
   if(PORT_PIN_PULL_UP_RES == pinRes)
   {
      gpioBase->GPIOPUR |= (1 << pin);  /*Set the GPIO Pull-Up register*/
   }
   else if(PORT_PIN_PULL_DOWN_RES == pinRes)
   {
      gpioBase->GPIOPDR |= (1 << pin); /*Set the GPIO Pull-Down register*/
   }
   else if(PORT_PIN_OPEN_DRAIN_RES == pinRes)
   {
      gpioBase->GPIOODR |= (1 << pin); /*Set the GPIO Open Drian register*/
   }
   else
   {
      /* Intended..*/
   }
   
}/*Port_SetPinInternRes*/

/**************************************************************************************
 * Function Name: <Port_SetPinDriveStrength>
 * Description: Configure pin drive Strength.
 *************************************************************************************/
LOCAL_INLINE void Port_SetPinDriveStrength(Gpio_HwRegisterRefType gpioBase, uint8 pin,Port_PinDriveStrengthType pinDriveStrength)
{
   gpioBase->GPIOPC |= PORT_PIN_DRIVE_STRENGTH_PC_EDM0; /*Default is Zero and we are going to set a group of bits.*/
   gpioBase->GPIODR4R |= ((pinDriveStrength&0x1) << pin); /*Update the GPIO 04-mA Drive Select registers.*/
   gpioBase->GPIODR8R |= ((pinDriveStrength&0x2) << pin); /*Update the GPIO 08-mA Drive Select registers.*/
   gpioBase->GPIODR12R |= ((pinDriveStrength&0x4) << pin);/*Update the GPIO 12-mA Drive Select registers.*/
}/*Port_SetPinDriveStrength*/

/**************************************************************************************
 * Function Name: <Port_lSetPinMode>
 * Description: Configure pin mode 'GPIO/Peripheral signal'.
 * TODO: Cfg GPIOADCCTL and GPIODMACTL. 
 *************************************************************************************/
LOCAL_INLINE void Port_lSetPinMode(Gpio_HwRegisterRefType gpioBase, uint8 pin,Port_PinModeType pinMode)
{
   if(PORT_PIN_MODE_DIO == pinMode)
   {
      gpioBase->GPIOAFSEL &= ~(1 << pin); /*Controlled by the GPIO registers.*/
      gpioBase->GPIOPCTL &= ~(0x0F << (pin << 0x02)); /*Each pin hase 4 bits..*/
   }
   else 
   {
      gpioBase->GPIOAFSEL |= (1 << pin); /*Peripheral signal and is controlled by the alternate hardware function..*/
      gpioBase->GPIOPCTL |= (pinMode << (pin << 0x02)); /*Each pin hase 4 bits <Pin Mode = Pin Mux/Pin Assignment>..*/
   }
}/*Port_lSetPinMode*/

/**************************************************************************************
 * Function Name: <Port_lSetPinDirection>
 * Description: Set pin direction.
 *************************************************************************************/
LOCAL_INLINE void Port_lSetPinDirection(Gpio_HwRegisterRefType gpioBase, uint8 pin,Port_PinDirectionType pinDirection)
{
   if(PORT_PIN_IN == pinDirection)
   {
      gpioBase->GPIODIR &= ~(1 << pin);
   }
   else
   {
      gpioBase->GPIODIR |= (1 << pin);
   }
}/*Port_SetPinDir*/

/**************************************************************************************
 * Function Name: <Port_InternalInit>
 * Description: Initialization sequence.
 *************************************************************************************/
LOCAL_INLINE void Port_InternalInit(const Port_ConfigType* ConfigPtr)
{
   Gpio_HwRegisterRefType gpioBase = NULL_PTR;
   uint8 pinIdx = 0, portId = 0, pinId = 0;
   const Port_ConfigType* thisObj;

   for(pinIdx = 0;pinIdx < PORT_NUM_OF_CFG_PINS; ++pinIdx)
   {
      /*Unpack the pin information..*/
      thisObj = &ConfigPtr[pinIdx]; /*Capture the current pin cfg base address..*/
      portId = PORT_GET_PIN_ID(thisObj) >> GPIO_NUM_OF_PINS_PER_PORT_2POWER; /*Get the current port id.*/
      pinId = PORT_GET_PIN_ID(thisObj) % GPIO_NUM_OF_PINS_PER_PORT;  /*Get the current pin id.*/
      
      gpioBase = Gpio_GetPortAddrById(portId); /*Get the base address of the port registers..*/

      Port_lSetPinDirection(gpioBase, pinId, /*Step #1: Select the pin direction.*/
                            PORT_GET_PIN_DIRECTION(thisObj));

      Port_lSetPinMode(gpioBase, pinId, /*Step #2: Select the pin Mode.*/
                      PORT_GET_PIN_MODE(thisObj));

      Port_SetPinDriveStrength(gpioBase, pinId, /*Step #3: Select the pin drive Strength.*/
                               PORT_GET_PIN_DRV_STRENGTH(thisObj));

      Port_SetPinInternRes(gpioBase, pinId, /*Step #4: Configure the internal resistor.*/
                           PORT_GET_PIN_INTERN_RES(thisObj));
      
      Port_SetPinSlewRate(gpioBase, pinId, /*Step #5: Configure the Slew rate.*/
                          PORT_GET_PIN_SLEW_RATE(thisObj));
      
      Port_SetPinFunctionality(gpioBase, pinId, /*Step #5: Enable this pin.*/
                               PORT_GET_PIN_INIT_VAL(thisObj),
                               PORT_PIN_IS_DIGITAL(thisObj));
   }
}/*Port_InternalInit*/

/******************************************************************
 * Global Functions
*******************************************************************/
/******************************************************************
 * Function Name: <Port_Init>
 * Param (IN) : const Port_ConfigType* ConfigPtr
 * Param (OUT): None
 * Return: None
 * Description: Initializes the Port Driver module. 
 ****************************************************************/
void Port_Init(const Port_ConfigType* ConfigPtr)
{
   if((NULL_PTR != ConfigPtr) && /*PARAM != NULL ?*/
      (E_PORT_INIT_PENDING != Port_InternalVar.state))
   {
      Port_InternalVar.state = E_PORT_INIT_PENDING; /*Init pending !*/

      Port_InternalInit(ConfigPtr); /*Perform the actual initialization.*/

      Port_InternalVar.cfgPtr = ConfigPtr; /*Hold the most updatable Configuration.*/
      Port_InternalVar.state = E_PORT_INIT_DONE; /*Init is DONE !*/
   }
}/*Port_Init*/

/******************************************************************
 * Function Name: <Port_SetPinDirection>
 * Param (IN) : Port_PinType Pin, Port_PinDirectionType Direction
 * Param (OUT): None
 * Return: None
 * Description: Sets the port pin direction.
 ****************************************************************/
void Port_SetPinDirection(Port_PinType Pin, Port_PinDirectionType Direction)
{
   uint8 portId = 0, pinId = 0;
   const Port_ConfigType* thisObj;
   Gpio_HwRegisterRefType gpioBase;

   if(E_PORT_INIT_DONE == Port_InternalVar.state)
   {
      thisObj = Port_Get_CfgInstByPinId(Pin);
      
      if(NULL_PTR != thisObj) /*If we found the correct instance 'object'..*/
      {
         if(TRUE == PORT_PIN_IS_DIR_CHANGABLE(thisObj)) /*If this pin is changable.*/
         {
            /*Unback the pin information..*/
            portId = Pin >> GPIO_NUM_OF_PINS_PER_PORT_2POWER; /*Get the current port id.*/
            pinId = Pin % GPIO_NUM_OF_PINS_PER_PORT;  /*Get the current pin id.*/
            
            gpioBase = Gpio_GetPortAddrById(portId); /*Get the base address of the port registers..*/

            Port_lSetPinDirection(gpioBase, pinId, /* Select the pin direction.*/
                                  Direction);
         }
      }
   }
}/*Port_SetPinDirection*/

/******************************************************************
 * Function Name: <Port_SetPinMode>
 * Param (IN) : Port_PinType Pin, Port_PinModeType Mode
 * Param (OUT): None
 * Return: None
 * Description: Sets the port pin mode.
 ****************************************************************/
void Port_SetPinMode(Port_PinType Pin, Port_PinModeType Mode)
{
   uint8 portId = 0, pinId = 0;
   const Port_ConfigType* thisObj;
   Gpio_HwRegisterRefType gpioBase;
   
   if(E_PORT_INIT_DONE == Port_InternalVar.state)
   {
      thisObj = Port_Get_CfgInstByPinId(Pin);
      
      if(NULL_PTR != thisObj) /*If we found the correct instance 'object'..*/
      {
         if(TRUE == PORT_PIN_IS_MODE_CHANGABLE(thisObj)) /*If this pin is changable.*/
         {
            /*Unpack the pin information..*/
            portId = Pin >> GPIO_NUM_OF_PINS_PER_PORT_2POWER; /*Get the current port id.*/
            pinId = Pin % GPIO_NUM_OF_PINS_PER_PORT;  /*Get the current pin id.*/
            
            gpioBase = Gpio_GetPortAddrById(portId); /*Get the base address of the port registers..*/

            Port_lSetPinMode(gpioBase, pinId, /*Select the pin Mode.*/
                             Mode);
         }
      }
   }
}/*Port_SetPinMode*/

#define PORT_MEMMAP_STOP_SEC_CODE
#include <MCAL/Port/Port_MemMap.h>
