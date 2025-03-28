/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "sai.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <math.h>
#include "arm_math.h"




////////////////////////////////////////////////////////////////////////////////////////
//
//		Product: Rotary mixer DBA5
//
//
//		Generated with: STM32CubeMX 6.10.0
//		Firmware Package Name and Version STM32Cube FW_H7 V1.11.1
//
//
//		add file arm_math.h
//
//
//
//
//
//
////////////////////////////////////////////////////////////////////////////////////////
//
//
//
//	ver. 0.01
//	- empty project
//	ver. 0.04
//	- added potenciometer adcs handler
//	ver. 0.05
//	- modify pwms
//	ver. 0.07
//	-	improved adcs and dacs inits
//	-	changed irq priority
//	-	changed tim2: htim2.Init.Prescaler = 1919; 	
//	ver. 0.15
//	-	improved dacs inits and 48K
//	- added inertion for leveler
//	-	improved audio handler
//	ver. 0.17
//	-	improved filters
//	ver. 1.01
//	-	added HPF filters
//	-	added check clipping on master and hp bus
//	ver. 1.05
//	-	added TIM5 PWM for orange led ch2
//
//
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////


/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

#include "global_variables.h"


#include "audio.h"
#include "audio_handler.h"

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void PeriphCommonClock_Config(void);
/* USER CODE BEGIN PFP */
void ADC1_SELECT_CH(uint32_t ch);
void ADC2_SELECT_CH(uint32_t ch);
void ADC3_SELECT_CH(uint32_t ch);
void CALC_CUTF_0(void);
void CALC_CUTF_1(void);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */
/* Enable the CPU Cache */

  /* Enable I-Cache---------------------------------------------------------*/
  SCB_EnableICache();

  /* Enable D-Cache---------------------------------------------------------*/
  SCB_EnableDCache();

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

/* Configure the peripherals common clocks */
  PeriphCommonClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_UART5_Init();
  MX_ADC1_Init();
  MX_ADC2_Init();
  MX_ADC3_Init();
  MX_SAI1_Init();
  MX_SAI2_Init();
  MX_SAI3_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
	MX_TIM5_Init();
  /* USER CODE BEGIN 2 */
	HAL_Delay(1450);
	HAL_GPIO_WritePin(VU_BACKLIGHT_GPIO_Port, VU_BACKLIGHT_Pin, GPIO_PIN_SET);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);				//start VU meter pwm
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);				//start VU meter pwm
	HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_2);				//start LED pwm
	HAL_TIM_PWM_Start(&htim5, TIM_CHANNEL_1);			//start LED pwm
	
	HAL_Delay(750);
	for(i=0;i<255;i++)
		{
		TIM3->CCR2 = i;
		TIM3->CCR1 = i;	
		HAL_Delay(6);	
		}
	for(i=255;i>0;i--)
		{
		TIM3->CCR2 = i;
		TIM3->CCR1 = i;
		HAL_Delay(6);	
		}
	HAL_TIM_Base_Start_IT(&htim2);									//start timer	
	HAL_SAI_Transmit_IT(&hsai_BlockA1, MASTER_BUS, 2);	
	HAL_SAI_Transmit_IT(&hsai_BlockA2, HP_BUS, 2);
	HAL_SAI_Receive_IT(&hsai_BlockA3, CH2IN, 2);
	HAL_SAI_Receive_IT(&hsai_BlockB3, CH1IN, 2);




  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
		
		
	//for debug	
	if(prewcrc!=currcrc)
		{
		HAL_Delay(2);
		//sprintf((char*)U_TX_DATA, "%03lu.%03lu.%03lu.%03lu.%03lu.%03lu\n\r", VR_LEVEL[0], VR_LEVEL[1], VR_LEVEL[2], VR_LEVEL[3], VR_LEVEL[4], VR_LEVEL[5]);
		//HAL_UART_Transmit(&huart5, U_TX_DATA, 25, 15);
			
			
		if(previous_VR_LEVEL[VCF0]!=VR_LEVEL[VCF0])		///FILTER CH1
			{
			CALC_CUTF_0();	
			previous_VR_LEVEL[VCF0] = VR_LEVEL[VCF0];	
			if(VR_LEVEL[VCF0]<128)			//LPF
				{
				TIM4->ARR = 3400;	
				TIM4->CCR2 = 1700;
				TIM4->PSC = 5000+421*VR_LEVEL[VCF0];
				}
			else if(VR_LEVEL[VCF0]==128)			//bandpass
				{
				TIM4->CNT = 0;	
				TIM4->ARR = 999;	
				TIM4->CCR2 = 0; 
				TIM4->PSC = 47;	
				}
			else				//hpf
				{
				TIM4->ARR = 3400;	
				TIM4->CCR2 = 1700; 	
				TIM4->PSC = 5000+421*(255-VR_LEVEL[VCF0]);				
				}			
			}

		if(previous_VR_LEVEL[CUES]!=VR_LEVEL[CUES])		///CUES selector
			{
			previous_VR_LEVEL[CUES] = VR_LEVEL[CUES];
			if(VR_LEVEL[CUES]<55)				//CH1
				{
				CUE_NUM	= 0;
				}			
			else if(VR_LEVEL[CUES]<92)	//CH1+M
				{
				CUE_NUM	= 1;	
				}		
			else if(VR_LEVEL[CUES]<130)	//MASTER
				{
				CUE_NUM	= 2;	
				}					
			else if(VR_LEVEL[CUES]<175)	//CH2+M
				{
				CUE_NUM	= 3;	
				}
			else							//CH2
				{
				CUE_NUM	= 4;
				}			
			}		
			
		if(previous_VR_LEVEL[VCF1]!=VR_LEVEL[VCF1])		///FILTER CH2
			{
			CALC_CUTF_1();	
			previous_VR_LEVEL[VCF1] = VR_LEVEL[VCF1];	
			if(VR_LEVEL[VCF1]<128)			//LPF
				{
				TIM5->ARR = 3400;	
				TIM5->CCR1 = 1700;
				TIM5->PSC = 5000+421*VR_LEVEL[VCF1];
				}
			else if(VR_LEVEL[VCF1]==128)			//bandpass
				{
				TIM5->CNT = 0;	
				TIM5->ARR = 999;	
				TIM5->CCR1 = 0; 
				TIM5->PSC = 47;	
				}
			else				//hpf
				{
				TIM5->ARR = 3400;	
				TIM5->CCR1 = 1700; 	
				TIM5->PSC = 5000+421*(255-VR_LEVEL[VCF1]);				
				}		
			}
			
		if(previous_VR_LEVEL[QFAC]!=VR_LEVEL[QFAC])		/// QFAC
			{
			CALC_CUTF_0();		
			CALC_CUTF_1();	
			previous_VR_LEVEL[QFAC] = VR_LEVEL[QFAC];	
			}			
		prewcrc = currcrc;	
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

  /** Supply configuration update enable
  */
  HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);

  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

  __HAL_RCC_SYSCFG_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE0);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 10;
  RCC_OscInitStruct.PLL.PLLN = 384;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_1;
  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_D3PCLK1|RCC_CLOCKTYPE_D1PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief Peripherals Common Clock Configuration
  * @retval None
  */
void PeriphCommonClock_Config(void)
{
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /** Initializes the peripherals clock
  */
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_ADC|RCC_PERIPHCLK_SAI1
                              |RCC_PERIPHCLK_SAI2|RCC_PERIPHCLK_SAI3;
  PeriphClkInitStruct.PLL2.PLL2M = 10;
  PeriphClkInitStruct.PLL2.PLL2N = 144;
  PeriphClkInitStruct.PLL2.PLL2P = 6;
  PeriphClkInitStruct.PLL2.PLL2Q = 2;
  PeriphClkInitStruct.PLL2.PLL2R = 2;
  PeriphClkInitStruct.PLL2.PLL2RGE = RCC_PLL2VCIRANGE_1;
  PeriphClkInitStruct.PLL2.PLL2VCOSEL = RCC_PLL2VCOWIDE;
  PeriphClkInitStruct.PLL2.PLL2FRACN = 0;
  PeriphClkInitStruct.PLL3.PLL3M = 25;
  PeriphClkInitStruct.PLL3.PLL3N = 295;
  PeriphClkInitStruct.PLL3.PLL3P = 2;
  PeriphClkInitStruct.PLL3.PLL3Q = 2;
  PeriphClkInitStruct.PLL3.PLL3R = 2;
  PeriphClkInitStruct.PLL3.PLL3RGE = RCC_PLL3VCIRANGE_0;
  PeriphClkInitStruct.PLL3.PLL3VCOSEL = RCC_PLL3VCOWIDE;
  PeriphClkInitStruct.PLL3.PLL3FRACN = 0;
  PeriphClkInitStruct.Sai1ClockSelection = RCC_SAI1CLKSOURCE_PLL3;
  PeriphClkInitStruct.Sai23ClockSelection = RCC_SAI23CLKSOURCE_PLL3;
  PeriphClkInitStruct.AdcClockSelection = RCC_ADCCLKSOURCE_PLL2;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */


#include "timer_handler.h"

///////////////////////////////////////////////
//
//	CALC_CUTF_0	
//	
void CALC_CUTF_0(void)
	{
	if(VR_LEVEL[VCF0]<128)
		{
		fcutoff_0 = VR_VCFx116[VR_LEVEL[VCF0]];	
		resonanse_0 = VR_QFAC[VR_LEVEL[QFAC]]*VR_VCF1minusKV[VR_LEVEL[VCF0]];		
		}
	else if(VR_LEVEL[VCF0]>128)
		{
		fcutoff_0h = VR_VCFx116[VR_LEVEL[VCF0]-129];	
		resonanse_0h = VR_QFAC[VR_LEVEL[QFAC]]*VR_VCF1minusKV[VR_LEVEL[VCF0]-129];		
		}		
	};

///////////////////////////////////////////////
//
//	CALC_CUTF_1	
//	
void CALC_CUTF_1(void)
	{
	if(VR_LEVEL[VCF1]<128)
		{
		fcutoff_1 = VR_VCFx116[VR_LEVEL[VCF1]];
		resonanse_1 = VR_QFAC[VR_LEVEL[QFAC]]*VR_VCF1minusKV[VR_LEVEL[VCF1]];
		}	
	else if(VR_LEVEL[VCF1]>128)
		{
		fcutoff_1h = VR_VCFx116[VR_LEVEL[VCF1]-129];	
		resonanse_1h = VR_QFAC[VR_LEVEL[QFAC]]*VR_VCF1minusKV[VR_LEVEL[VCF1]-129];		
		}		
	};	

	
//////////////////////////////////////////////////////////////////////////////
//
//		select channel for ADC1		
//			
void ADC1_SELECT_CH(uint32_t ch)
	{
	ADC_ChannelConfTypeDef sConfig;
	sConfig.Channel = ch;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
  sConfig.SingleDiff = ADC_SINGLE_ENDED;
  sConfig.OffsetNumber = ADC_OFFSET_NONE;
  sConfig.Offset = 0;
  sConfig.OffsetSignedSaturation = DISABLE;	
	HAL_ADC_ConfigChannel(&hadc1, &sConfig);	
	}		

//////////////////////////////////////////////////////////////////////////////
//
//		select channel for ADC2		
//			
void ADC2_SELECT_CH(uint32_t ch)
	{
	ADC_ChannelConfTypeDef sConfig;
	sConfig.Channel = ch;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
  sConfig.SingleDiff = ADC_SINGLE_ENDED;
  sConfig.OffsetNumber = ADC_OFFSET_NONE;
  sConfig.Offset = 0;
  sConfig.OffsetSignedSaturation = DISABLE;	
	HAL_ADC_ConfigChannel(&hadc2, &sConfig);	
	}			
	
//////////////////////////////////////////////////////////////////////////////
//
//		select channel for ADC3		
//			
void ADC3_SELECT_CH(uint32_t ch)
	{
	ADC_ChannelConfTypeDef sConfig;
	sConfig.Channel = ch;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
  sConfig.SingleDiff = ADC_SINGLE_ENDED;
  sConfig.OffsetNumber = ADC_OFFSET_NONE;
  sConfig.Offset = 0;
  sConfig.OffsetSignedSaturation = DISABLE;	
	HAL_ADC_ConfigChannel(&hadc3, &sConfig);	
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

#ifdef  USE_FULL_ASSERT
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
