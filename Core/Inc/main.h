/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

#include "stm32f1xx_ll_i2c.h"
#include "stm32f1xx_ll_bus.h"
#include "stm32f1xx_ll_cortex.h"
#include "stm32f1xx_ll_rcc.h"
#include "stm32f1xx_ll_system.h"
#include "stm32f1xx_ll_utils.h"
#include "stm32f1xx_ll_pwr.h"
#include "stm32f1xx_ll_gpio.h"
#include "stm32f1xx_ll_dma.h"

#include "stm32f1xx_ll_exti.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define RRGB_R_Pin GPIO_PIN_2
#define RRGB_R_GPIO_Port GPIOE
#define RRGB_G_Pin GPIO_PIN_3
#define RRGB_G_GPIO_Port GPIOE
#define RRGB_B_Pin GPIO_PIN_4
#define RRGB_B_GPIO_Port GPIOE
#define PS2_CS_Pin GPIO_PIN_4
#define PS2_CS_GPIO_Port GPIOA
#define PS2_CLK_Pin GPIO_PIN_5
#define PS2_CLK_GPIO_Port GPIOA
#define PS2_DI_Pin GPIO_PIN_6
#define PS2_DI_GPIO_Port GPIOA
#define PS2_DO_Pin GPIO_PIN_7
#define PS2_DO_GPIO_Port GPIOA
#define S1_Pin GPIO_PIN_0
#define S1_GPIO_Port GPIOB
#define TRIG_Pin GPIO_PIN_11
#define TRIG_GPIO_Port GPIOF
#define ECHO_Pin GPIO_PIN_12
#define ECHO_GPIO_Port GPIOF
#define LRGB_R_Pin GPIO_PIN_1
#define LRGB_R_GPIO_Port GPIOG
#define LRGB_G_Pin GPIO_PIN_7
#define LRGB_G_GPIO_Port GPIOE
#define LRGB_B_Pin GPIO_PIN_2
#define LRGB_B_GPIO_Port GPIOG
#define KEY1_Pin GPIO_PIN_3
#define KEY1_GPIO_Port GPIOG
#define KEY2_Pin GPIO_PIN_4
#define KEY2_GPIO_Port GPIOG
#define KEY3_Pin GPIO_PIN_5
#define KEY3_GPIO_Port GPIOG
#define Buzzer_Pin GPIO_PIN_12
#define Buzzer_GPIO_Port GPIOG
#define LED1_Pin GPIO_PIN_14
#define LED1_GPIO_Port GPIOG
#define LED2_Pin GPIO_PIN_15
#define LED2_GPIO_Port GPIOG
#define OLED_SDA_Pin GPIO_PIN_6
#define OLED_SDA_GPIO_Port GPIOB
#define OLED_SDL_Pin GPIO_PIN_7
#define OLED_SDL_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
