/**
  ******************************************************************************
  * @file    wwdg.c
  * @brief   This file provides code for the configuration
  *          of the WWDG instances.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "wwdg.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* WWDG init function */
void MX_WWDG_Init(void)
{

  /* Peripheral clock enable */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_WWDG);

  /* WWDG interrupt Init */
  NVIC_SetPriority(WWDG_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));
  NVIC_EnableIRQ(WWDG_IRQn);

  LL_WWDG_SetCounter(WWDG, 64);
  LL_WWDG_Enable(WWDG);
  LL_WWDG_SetPrescaler(WWDG, LL_WWDG_PRESCALER_1);
  LL_WWDG_SetWindow(WWDG, 64);

}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
