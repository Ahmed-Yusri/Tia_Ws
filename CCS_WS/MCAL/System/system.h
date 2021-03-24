/*
 * system.h
 *
 *  Created on: Aug 5, 2017
 *      Author: Ahmed Yusri Mohamed.
 */

#ifndef HAL_SYSTEM_SYSTEM_H_
#define HAL_SYSTEM_SYSTEM_H_

#include "types.h"
#include "hw_system.h"
#include "hw_types.h"

#define SYSTEM_UNUSED_VAR(VAR)   ((void)VAR)

/*typedef uint8_t System_CgcModuleType*/
#define UART_CLOCK_GATING_ID             ((System_CgcModuleType)0x00) 
#define GPTM_CLOCK_GATING_ID             ((System_CgcModuleType)0x01) 
#define QENCODER_CLOCK_GATING_ID         ((System_CgcModuleType)0x02)
#define PWM_CLOCK_GATING_ID              ((System_CgcModuleType)0x03) 
#define DMA_CLOCK_GATING_ID              ((System_CgcModuleType)0x04)
#define EEPROM_CLOCK_GATING_ID           ((System_CgcModuleType)0x05)

#define QENCDER_CH0_CGC        0
#define QENCDER_CH1_CGC        1

typedef enum gpiocgcmodes
{
	E_GPIO_CGC_RUN_MODE,
	E_GPIO_CGC_SLEEP_MODE,
	E_GPIO_CGC_DEEP_SLEEP_MODE
}Gpio_Cgc_Mode_t;

typedef enum gpiocgcport
{
   E_GPIO_CGC_PORT_A = 0,
   E_GPIO_CGC_PORT_B,
   E_GPIO_CGC_PORT_C,
   E_GPIO_CGC_PORT_D,
   E_GPIO_CGC_PORT_E,
   E_GPIO_CGC_PORT_F,
   E_GPIO_CGC_PORT_G,
   E_GPIO_CGC_PORT_H,
   E_GPIO_CGC_PORT_J,
   E_GPIO_CGC_PORT_K,
   E_GPIO_CGC_PORT_L,
   E_GPIO_CGC_PORT_M,
   E_GPIO_CGC_PORT_N,
   E_GPIO_CGC_PORT_P,
   E_GPIO_CGC_PORT_Q
}System_GpioCgcPortType;

typedef enum
{
	UART0CGC,UART1CGC,
	UART2CGC,UART3CGC,
	UART4CGC,UART5CGC,
	UART6CGC,UART7CGC,
	E_UART_UNSUPPORTED_CHANNEL
}UART_Cgc_t;

typedef enum gpiocgcportstatus
{
	Disable = 0,
	Enable,
}GPIOCGCPortStatus_t;

typedef struct gpiocgc
{
	Gpio_Cgc_Mode_t Mode;
	System_GpioCgcPortType Port;
	GPIOCGCPortStatus_t Status;
}GPIOCGC_t;

typedef enum Interrupttable
{
	E_INTERRUPT_ID_GPIOPortA = 0,
	E_INTERRUPT_ID_GPIOPortB,
	E_INTERRUPT_ID_GPIOPortC,
	E_INTERRUPT_ID_GPIOPortD,
	E_INTERRUPT_ID_GPIOPortE,
	E_INTERRUPT_ID_UART_0,
	E_INTERRUPT_ID_UART1,
	E_INTERRUPT_ID_SSI_0,
	E_INTERRUPT_ID_I2C_0,
	E_INTERRUPT_ID_PWM0Fault,
	E_INTERRUPT_ID_PWM0Generator0,
	E_INTERRUPT_ID_PWM0Generator1,
	E_INTERRUPT_ID_PWM0Generator2,
	E_INTERRUPT_ID_QEI0,
	E_INTERRUPT_ID_ADC0Sequence0,
	E_INTERRUPT_ID_ADC0Sequence1,
	E_INTERRUPT_ID_ADC0Sequence2,
	E_INTERRUPT_ID_ADC0Sequence3,
	E_INTERRUPT_ID_WatchdogTimers0and1,
	E_INTERRUPT_ID_16_32_BitTimer0A,
	E_INTERRUPT_ID_16_32_BitTimer0B,
	E_INTERRUPT_ID_16_32_BitTimer1A,
	E_INTERRUPT_ID_16_32_BitTimer1B,
	E_INTERRUPT_ID_16_32_BitTimer2A,
	E_INTERRUPT_ID_16_32_BitTimer2B,
	E_INTERRUPT_ID_AnalogComparator0,
	E_INTERRUPT_ID_AnalogComparator1,
   E_INTERRUPT_ID_AnalogComparator2,
	E_INTERRUPT_ID_SystemControl,
	E_INTERRUPT_ID_FlashMemoryControlandEEPROMControl,
	E_INTERRUPT_ID_GPIOPortF,
	E_INTERRUPT_ID_GPIOPortG,
   E_INTERRUPT_ID_GPIOPortH,
	E_INTERRUPT_ID_UART2,
	E_INTERRUPT_ID_SSI1,
	E_INTERRUPT_ID_16_32_BitTimer3A,
	E_INTERRUPT_ID_16_32_BitTimer3B,
	E_INTERRUPT_ID_I2C1,
   E_INTERRUPT_ID_CAN0,
   E_INTERRUPT_ID_CAN1,
   E_INTERRUPT_ID_ETHERNET_MAC,
   E_INTERRUPT_ID_HIB,
   E_INTERRUPT_ID_USB_MAC,
	E_INTERRUPT_ID_PWM_GENERATOR3,
	E_INTERRUPT_ID_uDMA0_SW,
   E_INTERRUPT_ID_uDMA0_ERRPR,
	E_INTERRUPT_ID_ADC1Sequence0,
	E_INTERRUPT_ID_ADC1Sequence1,
	E_INTERRUPT_ID_ADC1Sequence2,
	E_INTERRUPT_ID_ADC1Sequence3,
	E_INTERRUPT_ID_EPI0,
	E_INTERRUPT_ID_GPIOPortJ,
	E_INTERRUPT_ID_GPIOPortK,
   E_INTERRUPT_ID_GPIOPortL,
	E_INTERRUPT_ID_SSI2,
	E_INTERRUPT_ID_SSI3,
	E_INTERRUPT_ID_UART3,
	E_INTERRUPT_ID_UART4,
	E_INTERRUPT_ID_UART5,
	E_INTERRUPT_ID_UART6,
	E_INTERRUPT_ID_UART7,
	E_INTERRUPT_ID_I2C2,
	E_INTERRUPT_ID_I2C3,
	E_INTERRUPT_ID_16_32_Bit_Timer4A,
	E_INTERRUPT_ID_16_32_BitTimer4B,
	E_INTERRUPT_ID_16_32_BitTimer5A,
	E_INTERRUPT_ID_16_32_BitTimer5B,
   E_INTERRUPT_ID_FLOATING_POINT_IMPRECISE,
   E_INTERRUPT_ID_RES,
   E_INTERRUPT_ID_I2C4,
   E_INTERRUPT_ID_I2C5,
   E_INTERRUPT_ID_GPIOPortM,
   E_INTERRUPT_ID_GPIOPortN,
   E_INTERRUPT_ID_RES1,
   E_INTERRUPT_ID_TAMPER,
	E_INTERRUPT_ID_32_64_BitTimer0A,
	E_INTERRUPT_ID_32_64_BitTimer0B,
	E_INTERRUPT_ID_32_64_BitTimer1A,
	E_INTERRUPT_ID_32_64_BitTimer1B,
	E_INTERRUPT_ID_32_64_BitTimer2A,
	E_INTERRUPT_ID_32_64_BitTimer2B,
	E_INTERRUPT_ID_32_64_BitTimer3A,
	E_INTERRUPT_ID_32_64_BitTimer3B,
	E_INTERRUPT_ID_32_64_BitTimer4A,
	E_INTERRUPT_ID_32_64_BitTimer4B,
	E_INTERRUPT_ID_32_64_BitTimer5A,
	E_INTERRUPT_ID_32_64_BitTimer5B,
	E_INTERRUPT_ID_SystemException,
	E_INTERRUPT_ID_res7,
	E_INTERRUPT_ID_PWM1Generator0,
	E_INTERRUPT_ID_PWM1Generator1,
	E_INTERRUPT_ID_PWM1Generator2,
	E_INTERRUPT_ID_PWM1Generator3,
	E_INTERRUPT_ID_PWM1Fault,
}Interrupt_Tbl_t;

typedef enum wrstatues
{
	Read,
	Write,
}WRStatues_t;
typedef struct nvic
{
	uint8_t EN;
	uint8_t Disable;
	uint8_t Pending;
	uint8_t UnPending;
	uint8_t ACTIVE;
	uint8_t Priority;
	WRStatues_t WR;
	Interrupt_Tbl_t InterruptNum;
}NIVC_Cfg_t;

typedef enum
{
	E_ADVANCED_PERIPHERAL_BUS,
	E_ADVANCED_HIGH_PERFORMANCE_BUS
}Gpio_Bus_t;

typedef enum
{
	Timer0_16_32,Timer1_16_32,
	Timer2_16_32,Timer3_16_32,
	Timer4_16_32,Timer5_16_32,
	Timer0_32_64,Timer1_32_64,
	Timer2_32_64,Timer3_32_64,
	Timer4_32_64,Timer5_32_64,
}GPTMCGC_t;

typedef uint8_t System_CgcModuleType;
typedef uint8_t System_CgcArgType;

typedef System_CgcArgType QEncoder_Cgc_t;
typedef System_CgcArgType Pwm_Cgc_t;
typedef System_CgcArgType Dma_Cgc_t;
typedef System_CgcArgType EepRom_CgcType;

typedef struct System_CfgSt System_CfgType;

typedef const System_CfgType *System_CfgRefType;

Std_ReturnType Gpio_BusControl(System_GpioCgcPortType Port, Gpio_Bus_t Bus);
Std_ReturnType GPIO_resetPort(System_GpioCgcPortType Port);
Std_ReturnType NIVC_InterruptSetup(Interrupt_Tbl_t InterruptNum);
void NVIC_Set_Interrupt_Priority(Interrupt_Tbl_t iNum, uint8_t priority);

void Perform_System_Reset(void);

Std_ReturnType System_Init(System_CfgRefType cfgPtr);

#endif /* HAL_SYSTEM_SYSTEM_H_ */
