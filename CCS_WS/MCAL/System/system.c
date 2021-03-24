/***************************************
 * system.c
 *
 *  Created on: Aug 5, 2017
 *      Author: Ahmed Yusri
 ************************************/

#include "System_Cfg.h"
#include "system.h"

typedef Std_ReturnType(*const System_CgcHdlRefType)(const uint32_t);

static Std_ReturnType Gpio_clockGating(const GPIOCGC_t *const Pg);
static Std_ReturnType UART_clockGating(const UART_Cgc_t UART);
static Std_ReturnType GPTM_clockGating(const GPTMCGC_t TIMER);
static Std_ReturnType QEncoder_clockGating(const QEncoder_Cgc_t qei);
static Std_ReturnType Pwm_clockGating(const Pwm_Cgc_t pwm_module);
static Std_ReturnType Dma_clockGating(const Dma_Cgc_t moduleId);
static Std_ReturnType Eeprom_clockGating(const EepRom_CgcType moduleId);

const static System_CgcHdlRefType System_clockGatingHdl_Tbl[] =\
{
   (System_CgcHdlRefType)&UART_clockGating,
   (System_CgcHdlRefType)&GPTM_clockGating,
   (System_CgcHdlRefType)&QEncoder_clockGating,
   (System_CgcHdlRefType)&Pwm_clockGating,
   (System_CgcHdlRefType)&Dma_clockGating,
   (System_CgcHdlRefType)&Eeprom_clockGating
};

Std_ReturnType System_Init(System_CfgRefType cfgPtr)
{
   Std_ReturnType result = E_OK;
   GPIOCGC_t Gpio_clkCfg = {E_GPIO_CGC_RUN_MODE, cfgPtr->gpioPort[0],Enable};
   uint8_t cfgIdx;
   
   for(cfgIdx = 0; cfgIdx < SYSTEM_CFG_CGC_MODULES_NUM; ++cfgIdx)
   {
      result = System_clockGatingHdl_Tbl[cfgPtr->module[cfgIdx].id](cfgPtr->module[cfgIdx].arg);   /*Enable the clock gate for the appropriate module.*/
      
      DATA_SYNC_BARRIER();

      if(E_OK != result)
      {
         break;
      }
   }

   if(E_OK == result)
   {
      for(cfgIdx = 0; cfgIdx < SYSTEM_CFG_USED_PORTS_NUM; ++cfgIdx)
      {
         Gpio_clkCfg.Port = cfgPtr->gpioPort[cfgIdx];    /*get the port id.*/

         result = Gpio_clockGating((const GPIOCGC_t *)&Gpio_clkCfg);   /*Enable the clock gate for the appropriate port.*/

         if(E_OK != result)   /*Check if there is something wrong..*/
         {
            break;
         }
      }
   }

   return result;
}/*System_Init*/

Std_ReturnType Gpio_BusControl(System_GpioCgcPortType Port,Gpio_Bus_t Bus)
{
	if(Bus == E_ADVANCED_HIGH_PERFORMANCE_BUS)
   {
		/* AHB */
		WRITE_REG_32BIT(GPIOHBCTL,READ_REG_32BIT(GPIOHBCTL) | (1 << Port));
	}
	else
	{
		/*APB*/
		WRITE_REG_32BIT(GPIOHBCTL,READ_REG_32BIT(GPIOHBCTL) & (~(1 << Port)));
	}
	return E_OK;
}/*Gpio_BusControl*/

static Std_ReturnType Gpio_clockGating(const GPIOCGC_t *const Pg)
{
	Std_ReturnType ret = E_NOT_OK;

	switch(Pg->Mode)
	{
		case E_GPIO_CGC_RUN_MODE:
		{
			if(Enable == Pg->Status)
         {
				SET_BIT(RCGCGPIO, Pg->Port);
			}
            else
            {
            CLEAR_BIT(RCGCGPIO, Pg->Port);
			}

			DATA_SYNC_BARRIER();

			if(BIT_IS_SET(PRGPIO, Pg->Port))
			{
				ret = E_OK;
			}
			break;
		}
		case E_GPIO_CGC_SLEEP_MODE:
		{
			if(Enable == Pg->Status)
         {
			    WRITE_REG_32BIT(SCGCGPIO, READ_REG_32BIT(SCGCGPIO) | (1 << Pg->Port));
			}
         else
         {
				WRITE_REG_32BIT(SCGCGPIO, READ_REG_32BIT(SCGCGPIO) & (~(1 << Pg->Port)));
			}

			DATA_SYNC_BARRIER();

			if((READ_REG_32BIT(PRGPIO) & ((1 << Pg->Port)) >> Pg->Port) == 1)
         {
				ret = E_OK;
			}
			break;
		}
		case E_GPIO_CGC_DEEP_SLEEP_MODE:
		{
			if(Enable == Pg->Status)
         {
			    WRITE_REG_32BIT(DCGCGPIO, READ_REG_32BIT(DCGCGPIO) | (1 << Pg->Port));
			}
         else
         {
				WRITE_REG_32BIT(DCGCGPIO, READ_REG_32BIT(DCGCGPIO) & (~(1 << Pg->Port)));
			}

			if((READ_REG_32BIT(PRGPIO) & ((1 << Pg->Port)) >> Pg->Port) == 1)
         {
				ret = E_OK;
			}
			break;
		}
		default:
		{
			break;
		}
	}
	return ret;
}/*Gpio_clockGating*/

Std_ReturnType GPIO_resetPort(System_GpioCgcPortType Port)
{
	WRITE_REG_32BIT(SRGPIO, READ_REG_32BIT(SRGPIO) | (1 << Port));
	WRITE_REG_32BIT(SRGPIO, READ_REG_32BIT(SRGPIO) & (~(1 << Port)));
	return E_OK;
}/*GPIO_resetPort*/

Std_ReturnType NIVC_InterruptSetup(Interrupt_Tbl_t InterruptNum)
{
	Std_ReturnType ret = E_OK;

#if defined(DEV_ERROR)
   if(InterruptNum >= 139)
   {
		ret = E_NOT_OK;
	}
   else
#endif
   {
		if(InterruptNum < 32)
      {
		   WRITE_REG_32BIT(ENB0,READ_REG_32BIT(ENB0) | (1 << InterruptNum));
		}
      else if(InterruptNum < 64)
      {
			WRITE_REG_32BIT(ENB0+0X4,READ_REG_32BIT(ENB0+0X4) | (1 << (InterruptNum % 32)));
		}
      else if(InterruptNum < 96)
      {
		   WRITE_REG_32BIT(ENB0+0X8,READ_REG_32BIT(ENB0+0X8) | (1 << (InterruptNum % 32)));
		}
      else if(InterruptNum < 128)
      {
		   WRITE_REG_32BIT(ENB0+0XC,READ_REG_32BIT(ENB0+0XC) | (1 << (InterruptNum % 32)));
		}
      else
      {
		   WRITE_REG_32BIT(ENB0+0X10,READ_REG_32BIT(ENB0+0X10) | (1 << (InterruptNum % 32)));
		}
    }

	return ret;
}/*NIVC_InterruptSetup*/

void NVIC_Set_Interrupt_Priority(Interrupt_Tbl_t iNum, uint8_t priority)
{
   volatile uint32_t *base_register = (volatile uint32_t *)PRI0;
   
   const uint32_t register_offset = (iNum >> 2); /*get the PRI Register number.*/
	
   base_register[register_offset] = (priority << 5) << ((iNum % 4) << 3); /*Update the interrupt priority..*/

}/*NVIC_Set_Interrupt_Priority*/

void Perform_System_Reset(void)
{
   /*TODO: Add the system reset..*/
}/*Perform_System_Reset*/

static Std_ReturnType UART_clockGating(const UART_Cgc_t UART)
{
	Std_ReturnType ret = E_OK;

	if(UART > UART7CGC)
	{
		ret = E_NOT_OK;
	}
	else
	{
		WRITE_REG_32BIT(SYS_BASE + RCGCUART,(1 << UART) | READ_REG_32BIT(SYS_BASE + RCGCUART));
	}
	return ret;
}/*UART_clockGating*/

static Std_ReturnType GPTM_clockGating(const GPTMCGC_t TIMER)
{
	Std_ReturnType ret = E_OK;
   GPTMCGC_t lTimer = TIMER;
	if(TIMER > Timer5_32_64)
	{
		ret = E_NOT_OK;
	}
	else
	{
		if(TIMER > Timer5_16_32)
		{
		   lTimer = (GPTMCGC_t)(((uint8_t)TIMER % 5) - 1);

			WRITE_REG_32BIT(SYS_BASE + RCGCWTIMER,(1 << lTimer) | READ_REG_32BIT(SYS_BASE + RCGCWTIMER));
		}
		else
		{
			WRITE_REG_32BIT(SYS_BASE + RCGCTIMER,(1 << lTimer) | READ_REG_32BIT(SYS_BASE + RCGCTIMER));
		}
	}
   
   return ret;
}/*GPTM_clockGating*/

static Std_ReturnType QEncoder_clockGating(const QEncoder_Cgc_t qei)
{
	Std_ReturnType ret = E_OK;
#ifdef IN_DEV_MODE
	if(qei > QENCDER_CH1_CGC)
	{
		ret = E_NOT_OK;
	}
	else
#endif
	{
		SET_BIT(SYS_BASE + RCGCQEI, qei);
	}

	return ret;
}/*QEncoder_clockGating*/

static Std_ReturnType Pwm_clockGating(const Pwm_Cgc_t pwm_module)
{
	Std_ReturnType result = E_OK;
#if IN_DEV_MODE
	if(pwm_module > 1)
	{
		result = E_NOT_OK;
	}
   else
#endif
   {
      SET_BIT(SYS_BASE + RCGCPWM, pwm_module);
   }
   return result;
}/*Pwm_clockGating*/

static Std_ReturnType Dma_clockGating(const Dma_Cgc_t moduleId)
{
   SET_BIT(SYS_BASE + RCGCDMA, moduleId); /*Enable and provide a clock to the uDMA module in run mode.*/

   return E_OK;
}/*Dma_clockGating*/

static Std_ReturnType Eeprom_clockGating(const EepRom_CgcType moduleId)
{
   SET_BIT(SYS_BASE + RCGCEEPROM, moduleId); /*Enable and provide a clock to the EEPROM module in Run mode*/

   return E_OK;
}/*Eeprom_clockGating*/
