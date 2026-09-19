/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "math.h"
#include "LCD_4DIGIT.h"
#include <stdio.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

//variable for TT7-10KC3-11 thermistor
#define R0_NTC 10000.0f
#define T0_NTC 298.15f
#define BETA_NTC 3977.0f
#define R_SERIE 9990.0f
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;
DMA_HandleTypeDef hdma_adc1;

LCD_HandleTypeDef hlcd;

LPTIM_HandleTypeDef hlptim1;
LPTIM_HandleTypeDef hlptim2;

RTC_HandleTypeDef hrtc;

/* USER CODE BEGIN PV */
volatile Flag system_flag = {0};
State system_state = START;
uint16_t temp_value;
uint16_t bat_value;
uint8_t sleep_time = 0;
volatile uint8_t StopMode_Activated = 0;
uint32_t start;

volatile uint16_t adc_dma_buffer[2];

RTC_TimeTypeDef Time = {0};
RTC_DateTypeDef Date = {0};
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_ADC1_Init(void);
static void MX_LCD_Init(void);
static void MX_RTC_Init(void);
static void MX_LPTIM1_Init(void);
static void MX_LPTIM2_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void mini_sprintf(int n, char *s, uint8_t width, char pad)
{

	s[width-1] = '\0';

    for (int8_t i = width - 2; i >= 0; i--) {
        // 2. Fill digits using the absolute value
        if (n > 0 || (i == width - 2 && n == 0)) {
            s[i] = (n % 10) + '0';
            n /= 10;
        }
        // 4. Fill remaining space with padding
        else {
            s[i] = pad;
        }
    }
}

State switch_state(Flag sys_flag)
{
	State state = START;

	if(sys_flag.need_calibration > 400) state = CALIBRATION;
	else if(sys_flag.adc_update)  state = ADC_MEASUREMENT;
	else if(sys_flag.adc_ready)  state = ADC_READ;
	else if(sys_flag.wait_adc) state = WAIT_FOR_ADC;
	else if(sys_flag.bat_update)  state = PRINT_BAT;
	else if(sys_flag.time_update) state = PRINT_TIME;
	else if(sys_flag.temp_update) state = PRINT_TEMP;
	else state = SLEEP_MODE;

	return state;
}

uint8_t Read_ADC(uint16_t *temp_val, uint16_t *bat_val)
{
	float temp_celsius = 0;
	int ADC_temp_value = 0;
	float R_ADC = 0;

	int ADC_bat_value = 0;

	HAL_GPIO_WritePin(BAT_ADC_ENABLE_GPIO_Port, BAT_ADC_ENABLE_Pin, RESET);
	HAL_GPIO_WritePin(TEMP_ADC_ENABLE_GPIO_Port, TEMP_ADC_ENABLE_Pin, RESET);

	ADC_temp_value = adc_dma_buffer[0];
	ADC_bat_value = adc_dma_buffer[1];

	if(ADC_temp_value < 1058) //temp < 0°C
	{
		*temp_val = 0;
	}
	else
	{
		R_ADC = R_SERIE*((4095.0f/(float)ADC_temp_value) - 1.0f);

		float steinhart = logf(R_ADC / R0_NTC) / BETA_NTC;
		steinhart += 1.0f / T0_NTC;
		temp_celsius = (1.0f / steinhart) - 273.15f;
		*temp_val = (uint16_t)(temp_celsius * 10.0f);
	}

	*bat_val = (uint16_t)(660.0*(float)ADC_bat_value/4096.0);


	return 0;
}

uint8_t Print_temp(uint16_t temp_val)
{
	char temp_txt[5] = "   *";

	mini_sprintf(temp_val, temp_txt, sizeof(temp_txt), ' ');

	for (uint8_t i = 0; i < 3; i++) {
		temp_txt[i] = temp_txt[i + 1];
	}

	temp_txt[3] = '*';
	LCD_WriteDigit(temp_txt,0b0010);

	return 0;
}

uint8_t Print_time(uint8_t hour, uint8_t minute)
{
	char time_txt[5] = "0000";
	char minute_txt[3] = "00";

	mini_sprintf(minute, minute_txt, sizeof(minute_txt), '0');
	mini_sprintf(hour, time_txt, sizeof(time_txt), '0');

	for (uint8_t i = 0; i < 2; i++) {
		time_txt[i] = time_txt[i + 2];
	}

	time_txt[2] = minute_txt[0];
	time_txt[3] = minute_txt[1];

	LCD_WriteDigit(time_txt,0b1000);

	return 0;
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_ADC1_Init();
  MX_LCD_Init();
  MX_RTC_Init();
  MX_LPTIM1_Init();
  MX_LPTIM2_Init();
  /* USER CODE BEGIN 2 */

  HAL_LCD_Clear(&hlcd);

  Time.Hours = 0;
  Time.Minutes = 0;
  Time.Seconds = 0;
  HAL_RTC_SetTime(&hrtc, &Time, RTC_FORMAT_BIN);

  system_flag.adc_update = 1;
  system_flag.need_calibration = 401;
  system_flag.time_update = 1;
//  HAL_RCCEx_PeriphCLKConfig();

  HAL_LPTIM_Counter_Start_IT(&hlptim1);
  HAL_LPTIM_Counter_Start_IT(&hlptim2);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  switch (system_state)
	  {
	  case START:

		  system_state = switch_state(system_flag);
		  break;

	  case CALIBRATION:

		  HAL_ADCEx_Calibration_Start(&hadc1);
		  system_flag.need_calibration = 0;

		  system_state = switch_state(system_flag);
		  break;

	  case ADC_READ:

		  Read_ADC(&temp_value, &bat_value);

		  system_flag.adc_ready = 0;
		  system_flag.wait_adc = 0;

		  if(bat_value < 340)
		  {
			  system_flag.low_battery = 1;
		  }

		  system_state = switch_state(system_flag);
		  break;

	  case ADC_MEASUREMENT:

		  HAL_GPIO_WritePin(TEMP_ADC_ENABLE_GPIO_Port, TEMP_ADC_ENABLE_Pin, SET);
		  HAL_GPIO_WritePin(BAT_ADC_ENABLE_GPIO_Port, BAT_ADC_ENABLE_Pin, SET);

		  start = HAL_GetTick();

		  while ((HAL_GetTick() - start) < 10);

		  HAL_ADC_Start_DMA(&hadc1, (uint32_t*)adc_dma_buffer, 2);

		  system_flag.adc_update = 0;
		  system_flag.wait_adc = 1;

		  system_state = switch_state(system_flag);
		  break;

	  case WAIT_FOR_ADC:

		  system_flag.wait_adc = 1;

		  system_state = switch_state(system_flag);
		  break;

	  case PRINT_TIME:

		  system_flag.time_update = 0;

		  Print_time(Time.Hours, Time.Minutes);

		  sleep_time = 5;

		  system_state = switch_state(system_flag);
		  break;

	  case PRINT_TEMP:

		  system_flag.temp_update = 0;

		  Print_temp(temp_value);

		  sleep_time = 3;

		  system_state = switch_state(system_flag);
		  break;

	  case PRINT_BAT:

		  system_flag.bat_update = 0;

		  LCD_WriteDigit("batt", 0);

		  if(bat_value > 344)
		  {
			  system_flag.low_battery = 0;
		  }

		  sleep_time = 20;

		  system_state = switch_state(system_flag);
		  break;

	  case SLEEP_MODE:

		  if(sleep_time > 1)
		  {
			  HAL_RTCEx_SetWakeUpTimer_IT(&hrtc, sleep_time-1, RTC_WAKEUPCLOCK_CK_SPRE_16BITS, 0);
			  StopMode_Activated = 1;
			  HAL_SuspendTick();
			  HAL_PWREx_EnterSTOP2Mode(PWR_SLEEPENTRY_WFI);

			  HAL_RTCEx_DeactivateWakeUpTimer(&hrtc);
		  }

		  HAL_RTC_GetTime(&hrtc, &Time, RTC_FORMAT_BIN);
		  HAL_RTC_GetDate(&hrtc, &Date, RTC_FORMAT_BIN);

		  if(system_flag.low_battery) system_flag.bat_update = 1;
		  else if(sleep_time == 5) system_flag.temp_update = 1;
		  else if(sleep_time == 3) system_flag.time_update = 1;

		  system_state = switch_state(system_flag);
		  break;

	  default:
		  system_state = START;
		  break;
	  }
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE2);

  /** Configure LSE Drive Capability
  */
  HAL_PWR_EnableBkUpAccess();
  __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.ScanConvMode = ADC_SCAN_ENABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hadc1.Init.LowPowerAutoWait = DISABLE;
  hadc1.Init.LowPowerAutoPowerOff = DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.NbrOfConversion = 2;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.DMAContinuousRequests = DISABLE;
  hadc1.Init.Overrun = ADC_OVR_DATA_PRESERVED;
  hadc1.Init.SamplingTimeCommon1 = ADC_SAMPLETIME_160CYCLES_5;
  hadc1.Init.SamplingTimeCommon2 = ADC_SAMPLETIME_12CYCLES_5;
  hadc1.Init.OversamplingMode = DISABLE;
  hadc1.Init.TriggerFrequencyMode = ADC_TRIGGER_FREQ_HIGH;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_9;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLINGTIME_COMMON_1;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_3;
  sConfig.Rank = ADC_REGULAR_RANK_2;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief LCD Initialization Function
  * @param None
  * @retval None
  */
static void MX_LCD_Init(void)
{

  /* USER CODE BEGIN LCD_Init 0 */

  /* USER CODE END LCD_Init 0 */

  /* USER CODE BEGIN LCD_Init 1 */

  /* USER CODE END LCD_Init 1 */
  hlcd.Instance = LCD;
  hlcd.Init.Prescaler = LCD_PRESCALER_1;
  hlcd.Init.Divider = LCD_DIVIDER_16;
  hlcd.Init.Duty = LCD_DUTY_STATIC;
  hlcd.Init.Bias = LCD_BIAS_1_2;
  hlcd.Init.VoltageSource = LCD_VOLTAGESOURCE_INTERNAL;
  hlcd.Init.Contrast = LCD_CONTRASTLEVEL_3;
  hlcd.Init.DeadTime = LCD_DEADTIME_0;
  hlcd.Init.PulseOnDuration = LCD_PULSEONDURATION_0;
  hlcd.Init.BlinkMode = LCD_BLINKMODE_OFF;
  hlcd.Init.BlinkFrequency = LCD_BLINKFREQUENCY_DIV8;
  hlcd.Init.MuxSegment = LCD_MUXSEGMENT_DISABLE;
  if (HAL_LCD_Init(&hlcd) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN LCD_Init 2 */

  /* USER CODE END LCD_Init 2 */

}

/**
  * @brief LPTIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_LPTIM1_Init(void)
{

  /* USER CODE BEGIN LPTIM1_Init 0 */

  /* USER CODE END LPTIM1_Init 0 */

  /* USER CODE BEGIN LPTIM1_Init 1 */

  /* USER CODE END LPTIM1_Init 1 */
  hlptim1.Instance = LPTIM1;
  hlptim1.Init.Clock.Source = LPTIM_CLOCKSOURCE_APBCLOCK_LPOSC;
  hlptim1.Init.Clock.Prescaler = LPTIM_PRESCALER_DIV128;
  hlptim1.Init.Trigger.Source = LPTIM_TRIGSOURCE_SOFTWARE;
  hlptim1.Init.Period = 15300;
  hlptim1.Init.UpdateMode = LPTIM_UPDATE_ENDOFPERIOD;
  hlptim1.Init.CounterSource = LPTIM_COUNTERSOURCE_INTERNAL;
  hlptim1.Init.Input1Source = LPTIM_INPUT1SOURCE_GPIO;
  hlptim1.Init.Input2Source = LPTIM_INPUT2SOURCE_GPIO;
  hlptim1.Init.RepetitionCounter = 0;
  if (HAL_LPTIM_Init(&hlptim1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN LPTIM1_Init 2 */

  /* USER CODE END LPTIM1_Init 2 */

}

/**
  * @brief LPTIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_LPTIM2_Init(void)
{

  /* USER CODE BEGIN LPTIM2_Init 0 */

  /* USER CODE END LPTIM2_Init 0 */

  /* USER CODE BEGIN LPTIM2_Init 1 */

  /* USER CODE END LPTIM2_Init 1 */
  hlptim2.Instance = LPTIM2;
  hlptim2.Init.Clock.Source = LPTIM_CLOCKSOURCE_APBCLOCK_LPOSC;
  hlptim2.Init.Clock.Prescaler = LPTIM_PRESCALER_DIV128;
  hlptim2.Init.Trigger.Source = LPTIM_TRIGSOURCE_SOFTWARE;
  hlptim2.Init.Period = 65535;
  hlptim2.Init.UpdateMode = LPTIM_UPDATE_ENDOFPERIOD;
  hlptim2.Init.CounterSource = LPTIM_COUNTERSOURCE_INTERNAL;
  hlptim2.Init.Input1Source = LPTIM_INPUT1SOURCE_GPIO;
  hlptim2.Init.RepetitionCounter = 215;
  if (HAL_LPTIM_Init(&hlptim2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN LPTIM2_Init 2 */

  /* USER CODE END LPTIM2_Init 2 */

}

/**
  * @brief RTC Initialization Function
  * @param None
  * @retval None
  */
static void MX_RTC_Init(void)
{

  /* USER CODE BEGIN RTC_Init 0 */

  /* USER CODE END RTC_Init 0 */

  /* USER CODE BEGIN RTC_Init 1 */

  /* USER CODE END RTC_Init 1 */

  /** Initialize RTC Only
  */
  hrtc.Instance = RTC;
  hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
  hrtc.Init.AsynchPrediv = 127;
  hrtc.Init.SynchPrediv = 255;
  hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
  hrtc.Init.OutPutRemap = RTC_OUTPUT_REMAP_NONE;
  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  hrtc.Init.OutPutPullUp = RTC_OUTPUT_PULLUP_NONE;
  hrtc.Init.BinMode = RTC_BINARY_NONE;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }

  /** Enable the WakeUp
  */
  if (HAL_RTCEx_SetWakeUpTimer_IT(&hrtc, 0, RTC_WAKEUPCLOCK_CK_SPRE_16BITS, 0) != HAL_OK)
  {
    Error_Handler();
  }

  /** Enable Calibration
  */
  if (HAL_RTCEx_SetCalibrationOutPut(&hrtc, RTC_CALIBOUTPUT_1HZ) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RTC_Init 2 */

  /* USER CODE END RTC_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Channel1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(TEMP_ADC_ENABLE_GPIO_Port, TEMP_ADC_ENABLE_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(BAT_ADC_ENABLE_GPIO_Port, BAT_ADC_ENABLE_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : HEURE1_Pin HEURE2_Pin MINUT1_Pin MINUT2_Pin */
  GPIO_InitStruct.Pin = HEURE1_Pin|HEURE2_Pin|MINUT1_Pin|MINUT2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : TEMP_ADC_ENABLE_Pin */
  GPIO_InitStruct.Pin = TEMP_ADC_ENABLE_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(TEMP_ADC_ENABLE_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : BAT_ADC_ENABLE_Pin */
  GPIO_InitStruct.Pin = BAT_ADC_ENABLE_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(BAT_ADC_ENABLE_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI4_15_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI4_15_IRQn);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
/**
  * @brief  EXTI line detection callback.
  * @param  GPIO_Pin Specifies the port pin connected to corresponding EXTI line.
  * @retval None
  */
void HAL_GPIO_EXTI_Falling_Callback(uint16_t GPIO_Pin)
{

  if(StopMode_Activated == 1)
  {
	  SystemClock_Config();
	  HAL_ResumeTick();
	  StopMode_Activated = 0;
  }

  HAL_RTC_GetTime(&hrtc, &Time, RTC_FORMAT_BIN);
  HAL_RTC_GetDate(&hrtc, &Date, RTC_FORMAT_BIN);

  if(GPIO_Pin == MINUT2_Pin)
  {
	  Time.Minutes = (Time.Minutes + 1) % 60;
  }
  else if(GPIO_Pin == MINUT1_Pin)
  {
	  Time.Minutes = (Time.Minutes + 10) % 60;
  }
  else if(GPIO_Pin == HEURE2_Pin)
  {
	  Time.Hours = (Time.Hours + 1) % 24;
  }
  else if(GPIO_Pin == HEURE1_Pin)
  {
	  Time.Hours = (Time.Hours + 10) % 24;
  }
  else
  {
	  __NOP();
  }

  HAL_RTC_SetTime(&hrtc, &Time, RTC_FORMAT_BIN);
  system_flag.time_update = 1;
  sleep_time = 3;
}

void HAL_RTCEx_WakeUpTimerEventCallback(RTC_HandleTypeDef *hrtc)
{
	if(StopMode_Activated == 1)
	{
	  SystemClock_Config();
	  HAL_ResumeTick();
	  StopMode_Activated = 0;
	}

}

void HAL_LPTIM_AutoReloadMatchCallback(LPTIM_HandleTypeDef *hlptim)
{
	if(StopMode_Activated == 1)
	{
	  SystemClock_Config();
	  HAL_ResumeTick();
	  StopMode_Activated = 0;
	}

	if (hlptim->Instance == LPTIM2) system_flag.need_calibration++;
	else if (hlptim->Instance == LPTIM1) system_flag.adc_update = 1;
	else __NOP();
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
    if (hadc->Instance == ADC1)
    {
        system_flag.adc_ready = 1;
    }
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
