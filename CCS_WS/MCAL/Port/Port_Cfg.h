/*
 * Port_Cfg.h
 *
 *  Created on: Oct 23, 2020
 *      Author: Ahmed Yusri Mohamed Ezzat.
 */

#ifndef MCAL_PORT_PORT_CFG_H_
#define MCAL_PORT_PORT_CFG_H_

#define USER_SWITCH1_PIN_ID                  ((uint8)0x40)
#define USER_SWITCH2_PIN_ID                  ((uint8)0x41)
#define USER_LED0_PIN_ID                     ((uint8)0x61)
#define USER_LED1_PIN_ID                     ((uint8)0x60)
#define USER_LED2_PIN_ID                     ((uint8)0x28)
#define USER_TESTP_PIN_ID                    ((uint8)0x29)
#define USER_PWM02_PIN_ID                    ((uint8)0x2A)

#define USER_LED3_PIN_ID                     ((uint8)0x2C)
#define USER_UART0_RX_ID                     ((uint8)0x00)
#define USER_UART0_TX_ID                     ((uint8)0x01)

#define USER_UART4_RX_ID                     ((uint8)0x48)  /*PK0*/
#define USER_UART4_TX_ID                     ((uint8)0x49)  /*PK1*/

#define PORT_NUM_OF_CFG_PINS                 ((uint8)12)

/*Port_PinInterResType*/
#define PORT_PIN_PULL_UP_RES                 ((Port_PinInterResType)0x0)
#define PORT_PIN_PULL_DOWN_RES               ((Port_PinInterResType)0x1)
#define PORT_PIN_OPEN_DRAIN_RES              ((Port_PinInterResType)0x2)
#define PORT_PIN_INTER_RES_NONE              ((Port_PinInterResType)0x3)

/*Port_PinModeType*/
#define PORT_PIN_MODE_DIO                    ((Port_PinModeType)0X0)
#define PORT_PIN_MODE_UART                   ((Port_PinModeType)0X1)
#define PORT_PIN_MODE_SPI                    ((Port_PinModeType)0X0F)
#define PORT_PIN_MODE_PWM                    ((Port_PinModeType)0X06)

/*Port_PinDirectionType*/
#define PORT_PIN_IN                          ((Port_PinDirectionType)0)
#define PORT_PIN_OUT                         ((Port_PinDirectionType)1)

/*Port_PinDriveStrengthType*/
#define PORT_PIN_DRIVE_02MA                  ((Port_PinDriveStrengthType)0x00)
#define PORT_PIN_DRIVE_04MA                  ((Port_PinDriveStrengthType)0x01)
#define PORT_PIN_DRIVE_06MA                  ((Port_PinDriveStrengthType)0x02)
#define PORT_PIN_DRIVE_08MA                  ((Port_PinDriveStrengthType)0x03)
#define PORT_PIN_DRIVE_10MA                  ((Port_PinDriveStrengthType)0x06)
#define PORT_PIN_DRIVE_12MA                  ((Port_PinDriveStrengthType)0x07)

#define PORT_PIN_DRIVE_STRENGTH_PC_EDM0      ((uint32)0x3)

typedef uint8 Port_PinType; /*Port Pin ID.*/

typedef uint8 Port_PinDirectionType; /*Port Pin Direction.*/

typedef uint8 Port_PinInitValueType; /*Port Pin Init Value.*/

typedef boolean Port_PinSlewRateType; /*Port Pin Slew Rate Configuration ON/OFF.*/

typedef uint8 Port_PinInterResType;  /*Port Pin Internal Resistor PULL_UP/PULL_DOWN.*/

typedef uint8 Port_PinModeType;   /*Port Pin Mode.*/

typedef uint8 Port_PinDriveStrengthType;  /*Port Pin Drive Strength 2/4/.. 12mA.*/

typedef struct /*Type of the external data structure containing the initialization data for this module.*/
{
   Port_PinType pinId;
   Port_PinModeType pinMode;
   Port_PinDirectionType pinDirection;
   Port_PinInitValueType pinInitVal;
   boolean pinModeChangable;
   boolean pinDirectionChangable;
   boolean pinIsDigital;
   Port_PinSlewRateType pinSlewRate;
   Port_PinInterResType pinRes;
   Port_PinDriveStrengthType pinDriveStrength;
}Port_ConfigType;

/*Global Port Configuration Structure.*/
extern const Port_ConfigType Port_PinCfg[PORT_NUM_OF_CFG_PINS];

#endif /* MCAL_PORT_PORT_CFG_H_ */
