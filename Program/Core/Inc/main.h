/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#include "stm32u0xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdint.h>
#include <stdbool.h>
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
#define BAT_ADC_Pin GPIO_PIN_3
#define BAT_ADC_GPIO_Port GPIOC
#define TEMP_ADC_Pin GPIO_PIN_5
#define TEMP_ADC_GPIO_Port GPIOA
#define HEURE1_Pin GPIO_PIN_9
#define HEURE1_GPIO_Port GPIOA
#define HEURE1_EXTI_IRQn EXTI4_15_IRQn
#define HEURE2_Pin GPIO_PIN_10
#define HEURE2_GPIO_Port GPIOA
#define HEURE2_EXTI_IRQn EXTI4_15_IRQn
#define MINUT1_Pin GPIO_PIN_11
#define MINUT1_GPIO_Port GPIOA
#define MINUT1_EXTI_IRQn EXTI4_15_IRQn
#define MINUT2_Pin GPIO_PIN_12
#define MINUT2_GPIO_Port GPIOA
#define MINUT2_EXTI_IRQn EXTI4_15_IRQn
#define TEMP_ADC_ENABLE_Pin GPIO_PIN_12
#define TEMP_ADC_ENABLE_GPIO_Port GPIOC
#define BAT_ADC_ENABLE_Pin GPIO_PIN_9
#define BAT_ADC_ENABLE_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */
typedef enum
{
	START,
	CALIBRATION,
	ADC_READ,
	ADC_MEASUREMENT,
	WAIT_FOR_ADC,
	PRINT_TIME,
	PRINT_TEMP,
	PRINT_BAT,
	SLEEP_MODE
}State;

typedef struct
{
	volatile uint16_t need_calibration;
	volatile bool low_battery;
	volatile bool time_update;
	volatile bool temp_update;
	volatile bool bat_update;
	volatile bool adc_update;
	volatile bool adc_ready;
	volatile bool wait_adc;
}Flag;
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
