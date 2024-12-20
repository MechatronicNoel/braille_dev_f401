/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#include "stm32f4xx_hal.h"

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
#define LED_Pin GPIO_PIN_13
#define LED_GPIO_Port GPIOC
#define CD74HC595_SH_CP_Pin GPIO_PIN_12
#define CD74HC595_SH_CP_GPIO_Port GPIOB
#define CD74HC595_ST_CP_Pin GPIO_PIN_13
#define CD74HC595_ST_CP_GPIO_Port GPIOB
#define CD74HC595_DS_Pin GPIO_PIN_14
#define CD74HC595_DS_GPIO_Port GPIOB
#define CD74HCT4067_EN_Pin GPIO_PIN_3
#define CD74HCT4067_EN_GPIO_Port GPIOB
#define CD74HCT4067_S0_Pin GPIO_PIN_4
#define CD74HCT4067_S0_GPIO_Port GPIOB
#define CD74HCT4067_S1_Pin GPIO_PIN_5
#define CD74HCT4067_S1_GPIO_Port GPIOB
#define CD74HCT4067_S2_Pin GPIO_PIN_6
#define CD74HCT4067_S2_GPIO_Port GPIOB
#define CD74HCT4067_S3_Pin GPIO_PIN_7
#define CD74HCT4067_S3_GPIO_Port GPIOB
#define CD74HCT4067_S_Pin GPIO_PIN_8
#define CD74HCT4067_S_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
