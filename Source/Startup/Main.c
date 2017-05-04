#include "stm32f7xx.h"

#include "stm32f7xx_ll_bus.h"
#include "stm32f7xx_ll_rcc.h"
#include "stm32f7xx_ll_system.h"
#include "stm32f7xx_ll_utils.h"
#include "stm32f7xx_ll_pwr.h"
#include "stm32f7xx_ll_exti.h"
#include "stm32f7xx_ll_gpio.h"
#include "stm32f7xx_ll_adc.h"
#include "stm32f7xx_ll_cortex.h"
#include "stm32f7xx_ll_crc.h"
#include "stm32f7xx_ll_dac.h"
#include "stm32f7xx_ll_dma.h"
#include "stm32f7xx_ll_dma2d.h"
#include "stm32f7xx_ll_i2c.h"
#include "stm32f7xx_ll_iwdg.h"
#include "stm32f7xx_ll_rtc.h"
#include "stm32f7xx_ll_spi.h"
#include "stm32f7xx_ll_tim.h"
#include "stm32f7xx_ll_usart.h"
#include "stm32f7xx_ll_wwdg.h"

void SystemConfig(void)
{
  /* Set FLASH latency */ 
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_7);

  /* Enable PWR clock */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

  /* Activation OverDrive Mode */
  LL_PWR_EnableOverDriveMode();
  while(LL_PWR_IsActiveFlag_OD() != 1)
  {
  };

  /* Activation OverDrive Switching */
  LL_PWR_EnableOverDriveSwitching();
  while(LL_PWR_IsActiveFlag_ODSW() != 1)
  {
  };
  
  /* Main PLL configuration and activation */
  LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSI, LL_RCC_PLLM_DIV_16, 432, LL_RCC_PLLP_DIV_2);
  LL_RCC_PLL_Enable();
  while(LL_RCC_PLL_IsReady() != 1) 
  {
  };
  
  /* Sysclk activation on the main PLL */
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL) 
  {
  };
  
  /* Set APB1 & APB2 prescaler*/
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_4);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_2);
  
  /* Set systick to 1ms */
  SysTick_Config(216000000 / 1000);
  
  /* Update CMSIS variable (which can be updated also through SystemCoreClockUpdate function) */
  SystemCoreClock = 216000000; 
	
	  /* Disable MPU */
  LL_MPU_Disable();

  /* Configure the MPU attributes as WT for SRAM */
  LL_MPU_ConfigRegion(LL_MPU_REGION_NUMBER0, 0x00, 0x20020000UL, 
         LL_MPU_REGION_SIZE_512KB | LL_MPU_REGION_FULL_ACCESS | LL_MPU_ACCESS_NOT_BUFFERABLE |
         LL_MPU_ACCESS_CACHEABLE | LL_MPU_ACCESS_SHAREABLE | LL_MPU_TEX_LEVEL0 |
         LL_MPU_INSTRUCTION_ACCESS_ENABLE);

  /* Enable MPU (any access not covered by any enabled region will cause a fault) */
  LL_MPU_Enable(LL_MPU_CTRL_PRIVILEGED_DEFAULT);
	
	  /* Enable I-Cache */
  SCB_EnableICache();

  /* Enable D-Cache */
  SCB_EnableDCache();
}

int main(void)
{
    SystemConfig();
    for (;;);
}

