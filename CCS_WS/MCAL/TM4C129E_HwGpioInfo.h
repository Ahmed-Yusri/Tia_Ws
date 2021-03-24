/*
 * TM4C129E_HwGpioInfo.h
 *
 *  Created on: Oct 23, 2020
 *      Author: Ahmed Yusri Mohamed Ezzat
 */

#ifndef MCAL_TM4C129E_HWGPIOINFO_H_
#define MCAL_TM4C129E_HWGPIOINFO_H_

#define GPIO_NUM_OF_PORTS                                      ((uint8)15)
#define GPIO_NUM_OF_PINS_PER_PORT_2POWER                       ((uint8)3)
#define GPIO_NUM_OF_PINS_PER_PORT                              ((uint8)8)

#define GPIO_PORTA_REG_INFO         ((Gpio_HwRegisterRefType)0x40058000)
#define GPIO_PORTB_REG_INFO         ((Gpio_HwRegisterRefType)0x40059000)
#define GPIO_PORTC_REG_INFO         ((Gpio_HwRegisterRefType)0x4005A000)
#define GPIO_PORTD_REG_INFO         ((Gpio_HwRegisterRefType)0x4005B000)
#define GPIO_PORTE_REG_INFO         ((Gpio_HwRegisterRefType)0x4005C000)
#define GPIO_PORTF_REG_INFO         ((Gpio_HwRegisterRefType)0x4005D000)
#define GPIO_PORTG_REG_INFO         ((Gpio_HwRegisterRefType)0x4005E000)
#define GPIO_PORTH_REG_INFO         ((Gpio_HwRegisterRefType)0x4005F000)
#define GPIO_PORTJ_REG_INFO         ((Gpio_HwRegisterRefType)0x40060000)
#define GPIO_PORTK_REG_INFO         ((Gpio_HwRegisterRefType)0x40061000)
#define GPIO_PORTL_REG_INFO         ((Gpio_HwRegisterRefType)0x40062000)
#define GPIO_PORTM_REG_INFO         ((Gpio_HwRegisterRefType)0x40063000)
#define GPIO_PORTN_REG_INFO         ((Gpio_HwRegisterRefType)0x40064000)
#define GPIO_PORTP_REG_INFO         ((Gpio_HwRegisterRefType)0x40065000)
#define GPIO_PORTQ_REG_INFO         ((Gpio_HwRegisterRefType)0x40066000)


typedef volatile uint32 GpioHwRegisterType;
typedef GpioHwRegisterType *GpioHwRegisterRefType;

typedef struct
{
   GpioHwRegisterType GPIODATA;
   GpioHwRegisterType gpio_res0[255];
   GpioHwRegisterType GPIODIR;
   GpioHwRegisterType GPIOIS;
   GpioHwRegisterType GPIOIBE;
   GpioHwRegisterType GPIOIEV;
   GpioHwRegisterType GPIOIM;
   GpioHwRegisterType GPIORIS;
   GpioHwRegisterType GPIOMIS;
   GpioHwRegisterType GPIOICR;
   GpioHwRegisterType GPIOAFSEL;
   GpioHwRegisterType gpio_res1[55];
   GpioHwRegisterType GPIODR2R;
   GpioHwRegisterType GPIODR4R;
   GpioHwRegisterType GPIODR8R;
   GpioHwRegisterType GPIOODR;
   GpioHwRegisterType GPIOPUR;
   GpioHwRegisterType GPIOPDR;
   GpioHwRegisterType GPIOSLR;
   GpioHwRegisterType GPIODEN;
   GpioHwRegisterType GPIOLOCK;
   GpioHwRegisterType GPIOCR;
   GpioHwRegisterType GPIOAMSEL;
   GpioHwRegisterType GPIOPCTL;
   GpioHwRegisterType GPIOADCCTL;
   GpioHwRegisterType GPIODMACTL;
   GpioHwRegisterType GPIOSI;
   GpioHwRegisterType GPIODR12R;
   GpioHwRegisterType GPIOWAKEPEN;
   GpioHwRegisterType GPIOWAKELVL;
   GpioHwRegisterType GPIOWAKESTAT;
   GpioHwRegisterType gpio_res2[669];
   GpioHwRegisterType GPIOPP;
   GpioHwRegisterType GPIOPC;
   GpioHwRegisterType GPIOPeriphID4;
}Gpio_HwRegisterType;

typedef Gpio_HwRegisterType *Gpio_HwRegisterRefType;

#endif /* MCAL_TM4C129E_HWGPIOINFO_H_ */
