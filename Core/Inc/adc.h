#ifndef __ADC_H__
#define __ADC_H__

#include "stm32g0xx_hal.h"
#include <stdbool.h>


#define RTD_ADC    hadc1



typedef struct{
	GPIO_TypeDef* port;
	uint32_t pin;
	ADC_ChannelConfTypeDef channel;
}ADC_channel_t;

extern ADC_HandleTypeDef hadc1;








bool calibrate_adc (ADC_HandleTypeDef* adc_handle);

void adc_config_polling(void);
void adc_config_dma(void);
void adc_config_channel(ADC_channel_t* channel);

void adc_read_polling (uint16_t * raw_value,uint8_t sequence);
void adc_read_dma(uint16_t* raw_array,uint8_t channel_num);




#endif 

