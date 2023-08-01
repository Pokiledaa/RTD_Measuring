#include "adc.h"
#include "config.h"






ADC_HandleTypeDef hadc1;
DMA_HandleTypeDef hdma_adc1;



void HAL_ADC_MspInit(ADC_HandleTypeDef* adcHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};
  if(adcHandle->Instance==ADC1)
  {
  /* USER CODE BEGIN ADC1_MspInit 0 */

  /* USER CODE END ADC1_MspInit 0 */

  /** Initializes the peripherals clocks
  */
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
    PeriphClkInit.AdcClockSelection = RCC_ADCCLKSOURCE_SYSCLK;
    HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit);
    

    /* ADC1 clock enable */
    __HAL_RCC_ADC_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**ADC1 GPIO Configuration
    PA0     ------> ADC1_IN0
    PA1     ------> ADC1_IN1
    PA2     ------> ADC1_IN2
    */
    GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
		#if RTD_READING_METHOD == DMA_SETUP
    /* ADC1 DMA Init */
    /* ADC1 Init */
    hdma_adc1.Instance = DMA1_Channel1;
    hdma_adc1.Init.Request = DMA_REQUEST_ADC1;
    hdma_adc1.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_adc1.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_adc1.Init.MemInc = DMA_MINC_ENABLE;
    hdma_adc1.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
    hdma_adc1.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
    hdma_adc1.Init.Mode = DMA_CIRCULAR;
    hdma_adc1.Init.Priority = DMA_PRIORITY_LOW;
    HAL_DMA_Init(&hdma_adc1);

    __HAL_LINKDMA(adcHandle,DMA_Handle,hdma_adc1);
		#endif

  /* USER CODE BEGIN ADC1_MspInit 1 */

  /* USER CODE END ADC1_MspInit 1 */
  }
}

void HAL_ADC_MspDeInit(ADC_HandleTypeDef* adcHandle)
{

  if(adcHandle->Instance==ADC1)
  {
  /* USER CODE BEGIN ADC1_MspDeInit 0 */

  /* USER CODE END ADC1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_ADC_CLK_DISABLE();

    /**ADC1 GPIO Configuration
    PA0     ------> ADC1_IN0
    PA1     ------> ADC1_IN1
    PA2     ------> ADC1_IN2
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2);

    /* ADC1 DMA DeInit */
    HAL_DMA_DeInit(adcHandle->DMA_Handle);
  /* USER CODE BEGIN ADC1_MspDeInit 1 */

  /* USER CODE END ADC1_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */


void adc_config_dma(void){
	 /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
  */
  RTD_ADC.Instance = ADC1;
  RTD_ADC.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV4;
  RTD_ADC.Init.Resolution = ADC_RESOLUTION_12B;
  RTD_ADC.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  RTD_ADC.Init.ScanConvMode = ADC_SCAN_ENABLE;
  RTD_ADC.Init.EOCSelection = ADC_EOC_SEQ_CONV;
  RTD_ADC.Init.LowPowerAutoWait = DISABLE;
  RTD_ADC.Init.LowPowerAutoPowerOff = DISABLE;
  RTD_ADC.Init.ContinuousConvMode = ENABLE;
  RTD_ADC.Init.NbrOfConversion = 3;
  RTD_ADC.Init.DiscontinuousConvMode = DISABLE;
  RTD_ADC.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  RTD_ADC.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  RTD_ADC.Init.DMAContinuousRequests = ENABLE;
  RTD_ADC.Init.Overrun = ADC_OVR_DATA_PRESERVED;
  RTD_ADC.Init.SamplingTimeCommon1 = ADC_SAMPLETIME_160CYCLES_5;
  RTD_ADC.Init.SamplingTimeCommon2 = ADC_SAMPLETIME_7CYCLES_5;
  RTD_ADC.Init.OversamplingMode = DISABLE;
  RTD_ADC.Init.TriggerFrequencyMode = ADC_TRIGGER_FREQ_HIGH;
  HAL_ADC_Init(&RTD_ADC);

}

void adc_config_polling(void){
	
  RTD_ADC.Instance = ADC1;
  RTD_ADC.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV4;
  RTD_ADC.Init.Resolution = ADC_RESOLUTION_12B;
  RTD_ADC.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  RTD_ADC.Init.ScanConvMode = ADC_SCAN_ENABLE;
  RTD_ADC.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  RTD_ADC.Init.LowPowerAutoWait = DISABLE;
  RTD_ADC.Init.LowPowerAutoPowerOff = DISABLE;
  RTD_ADC.Init.ContinuousConvMode = DISABLE;
  RTD_ADC.Init.NbrOfConversion = 3;
  RTD_ADC.Init.DiscontinuousConvMode = DISABLE;
  RTD_ADC.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  RTD_ADC.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  RTD_ADC.Init.DMAContinuousRequests = DISABLE;
  RTD_ADC.Init.Overrun = ADC_OVR_DATA_PRESERVED;
  RTD_ADC.Init.SamplingTimeCommon1 = ADC_SAMPLETIME_160CYCLES_5;
  RTD_ADC.Init.SamplingTimeCommon2 = ADC_SAMPLETIME_7CYCLES_5;
  RTD_ADC.Init.OversamplingMode = DISABLE;
  RTD_ADC.Init.TriggerFrequencyMode = ADC_TRIGGER_FREQ_HIGH;
  HAL_ADC_Init(&RTD_ADC);
}

void adc_config_channel(ADC_channel_t* channel){
	/** Configure Regular Channel
  */
  HAL_ADC_ConfigChannel(&hadc1,&channel->channel);
}

/*******************************************************************
*  Reading 3 Channel with sampling_cycle : 39.5 takes : 15.7 us
*  Reading 3 Channel with sampling_cycle : 160.5 takes : 38.4us
********************************************************************/

void adc_read_polling (uint16_t * raw_value,uint8_t sequence){
	uint8_t channel = 0;
	for(channel = 0 ; channel < sequence ; channel++){
			HAL_ADC_Start(&RTD_ADC);
			HAL_ADC_PollForConversion(&RTD_ADC,1000);
			raw_value[channel] = HAL_ADC_GetValue(&RTD_ADC);
		}
	if(sequence == 2 ){
		HAL_ADC_Start(&RTD_ADC);
		HAL_ADC_PollForConversion(&RTD_ADC,1000);
	}
	if(sequence == 1 ){
		HAL_ADC_Start(&RTD_ADC);
		HAL_ADC_PollForConversion(&RTD_ADC,1000);
	}
}

bool calibrate_adc (ADC_HandleTypeDef* adc_handle){
	HAL_StatusTypeDef err;
	err = HAL_ADCEx_Calibration_Start(adc_handle);
	return err;
}






/* USER CODE END 1 */
