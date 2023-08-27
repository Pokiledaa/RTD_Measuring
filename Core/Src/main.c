#include "main.h"
#include "config.h"
#include "RTD.h"


/**********************************************************
*								RTD OBJECTS INITIALLIZATION
***********************************************************/
RTD_t RTD_3 = {
	.measurment.RTD_Mean_Number = 50,
	.calibration.Refrence_Resistor = 68,
	.calibration.Gain_Factor = 0.018006263,
	.calibration.ADC_Raw_Min_Temp = 109,
	.channel = 3, // Remember to Set The Channel Base On The ADC Channel That you Choosed 
	.base_resistance = 100,
};

RTD_t RTD_2 = {
	.measurment.RTD_Mean_Number = 50,
	.calibration.Refrence_Resistor = 68,
	.calibration.Gain_Factor = 0.018006263,
	.calibration.ADC_Raw_Min_Temp = 109,
	.channel = 2,
	.base_resistance = 100,
};

RTD_t RTD_1 = {
	.measurment.RTD_Mean_Number = 50,
	.calibration.Refrence_Resistor = 68,
	.calibration.Gain_Factor = 0.018006263,
	.calibration.ADC_Raw_Min_Temp = 109,
	.channel = 1,
	.base_resistance = 100,
};




#define LED_PORT   GPIOA
#define LED_PIN    GPIO_PIN_15



float rtd_3_tempreture = 0;


void led_config(void);
void led_on(void);
void led_off(void);


void SystemClock_Config(void);




int main(void)
{
	
	
  HAL_Init();
  SystemClock_Config();
	led_config();
	RTD_Init(&RTD_3);   // Initializing The Library
	RTD_Enable(&RTD_3); // Enableing The RTD Module
  while (1)
  {
		led_on();
		rtd_3_tempreture = RTD_Full_Convertion(&RTD_3);    
		led_off();
  }
}

void led_on(void){
	HAL_GPIO_WritePin(LED_PORT,LED_PIN,GPIO_PIN_SET);
}

void led_off(void){
	HAL_GPIO_WritePin(LED_PORT,LED_PIN,GPIO_PIN_RESET);
}

void led_config(void){
	
	/*Configuring LED on GPIOA , GPIO_PIN_15 */
	
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_GPIOF_CLK_ENABLE();
	GPIO_InitTypeDef led;
	
	led.Mode = GPIO_MODE_OUTPUT_PP;
	led.Pin = LED_PIN;
	led.Speed = GPIO_SPEED_FREQ_HIGH;
	led.Pull = GPIO_NOPULL;
	
	HAL_GPIO_Init(LED_PORT,&led);
	
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
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSIDiv = RCC_HSI_DIV1;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV1;
  RCC_OscInitStruct.PLL.PLLN = 8;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}


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
