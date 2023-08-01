#include "RTD.h"
#include "dma.h"
#include "adc.h"
#include "config.h"
#include <math.h>




/**********************************************************************************************
*																	PARIVATE MACROS
***********************************************************************************************/
#define RTD_A 3.9083e-3
#define RTD_B -5.775e-7

/**********************************************************************************************
*																	PRIVATE VARIABLES
***********************************************************************************************/
volatile uint16_t dma_raw_adc[RTD_CHANNEL_NUM];

/**********************************************************************************************
*																	STRUCT & ENUMS
***********************************************************************************************/

typedef enum{
	Convertion_Completed = 0,
	Convertion_Ongooing = 1,
}Convertion_State_t;

typedef struct{
	Convertion_State_t Convertion_State;
	uint16_t ADC_Value_Raw[RTD_CHANNEL_NUM];
}ADC_DMA_Handler_t;

ADC_DMA_Handler_t adh;

ADC_channel_t RTD_1_ADC_Channel = {
	.channel.Channel = ADC_CHANNEL_2,
	.channel.Rank = ADC_REGULAR_RANK_1,
	.channel.SamplingTime = ADC_SAMPLINGTIME_COMMON_1,
	.port = GPIOA,
	.pin = GPIO_PIN_2
};

ADC_channel_t RTD_2_ADC_Channel = {
	.channel.Channel = ADC_CHANNEL_1,
	.channel.Rank = ADC_REGULAR_RANK_2,
	.channel.SamplingTime = ADC_SAMPLINGTIME_COMMON_1,
	.port = GPIOA,
	.pin = GPIO_PIN_1
};

ADC_channel_t RTD_3_ADC_Channel = {
	.channel.Channel = ADC_CHANNEL_0,
	.channel.Rank = ADC_REGULAR_RANK_3,
	.channel.SamplingTime = ADC_SAMPLINGTIME_COMMON_1,
	.port = GPIOA,
	.pin = GPIO_PIN_0
};






/**********************************************************************************************
*																	PRIVATE FUNCTION PROTOTYPE
***********************************************************************************************/
static void RTD_Raw_Adc_Mean(RTD_t* rtd,uint16_t* raw_adc);



/*************************INITIATOR FOR RTD MODULE*************************************/
void RTD_Init(RTD_t* rtd){
		/*RTD CONFIGURATION WITH ADC-DMA */
		
		/*STEP 1 Configuration of DMA*/
		dma_config();
		/*STEP 2 Configuration of ADC in DMA Read Mode*/
		adc_config_dma();
		/*STEP 3 Configuration of ADC Channels*/
		adc_config_channel(&RTD_3_ADC_Channel);
		adc_config_channel(&RTD_2_ADC_Channel);
		adc_config_channel(&RTD_1_ADC_Channel);
		/*STEP 4 ADC Calibration*/
		calibrate_adc(&RTD_ADC);
		/*STEP 5 Start ADC Continious Convertion with DMA*/
		HAL_ADC_Start_DMA(&RTD_ADC,(uint32_t*)adh.ADC_Value_Raw,RTD_CHANNEL_NUM);
	
	
		rtd->enable = RTD_ENABLE;
	  rtd->status = RTD_Status_Data_Unvalid;
		rtd->sampling_status = RTD_Sampling_Status_Ongoing;
		rtd->measurment.RTD_Resistance = 0;
		rtd->measurment.RTD_Tempreture = 0;
}

/*************************CALCULATOR FOR RTD TEMPRETURE*************************************/
void RTD_Resistance_Read (RTD_t* rtd,uint16_t raw_measument){
	float tmp;
	tmp = rtd->calibration.Gain_Factor*(raw_measument - rtd->calibration.ADC_Raw_Min_Temp);
	rtd->measurment.RTD_Resistance = tmp + rtd->calibration.Refrence_Resistor	;
}

/*************************GETTER FOR RTD RESISTANCE*************************************/
float RTD_Resistance_Get (RTD_t* rtd){
	return rtd->measurment.RTD_Resistance;
}


/*************************CALCULATROR FOR RTD TEMPRETURE IN MATHEMATICAL MODE*************************************/
void RTD_Tempreture_Read_Mathematicall(RTD_t* rtd,float rtd_resistance){
	
	float Z1, Z2, Z3, Z4, Rt, temp;
	uint8_t skip_flag = 0;
	
	Z1 = -RTD_A;
  Z2 = RTD_A * RTD_A - (4 * RTD_B);
  Z3 = (4 * RTD_B) / rtd->base_resistance;
  Z4 = 2 * RTD_B;

  temp = Z2 + (Z3 * rtd_resistance);
  temp = (sqrt(temp) + Z1) / Z4;

  if (temp >= 0){
    skip_flag = SET;
		rtd->measurment.RTD_Tempreture = temp;
	}

	if(skip_flag){
		
		// ugh.
		rtd_resistance /= rtd->base_resistance;
		rtd_resistance *= 100; // normalize to 100 ohm

		float rpoly = rtd_resistance;

		temp = -242.02;
		temp += 2.2228 * rpoly;
		rpoly *= rtd_resistance; // square
		temp += 2.5859e-3 * rpoly;
		rpoly *= rtd_resistance; // ^3
		temp -= 4.8260e-6 * rpoly;
		rpoly *= rtd_resistance; // ^4
		temp -= 2.8183e-8 * rpoly;
		rpoly *= rtd_resistance; // ^5
		temp += 1.5243e-10 * rpoly;
		rtd->measurment.RTD_Tempreture = temp;
	}
 
}

/*************************GETTER FOR RTD TEMPRETURE*************************************/
float RTD_Tempreture_Get(RTD_t* rtd){
	return rtd->measurment.RTD_Tempreture;
}


/*************************SETTER FOR RAW ADC IN POLLING MODE*************************************/
void RTD_Raw_ADC_Set(RTD_t* rtd,uint16_t raw_adc){
	if(rtd->enable == SET){
		rtd->sampling_status = RTD_Sampling_Status_Ongoing;
		rtd->measurment.RTD_ADC_Raw_Value = raw_adc;
		rtd->measurment.RTD_ADC_Sampling_Buffer[rtd->measurment.RTD_ADC_Mean_Interval_Counter] = raw_adc; /*Buffering the adc samples*/
		rtd->measurment.RTD_ADC_Mean_Interval_Counter ++;
		if(rtd->measurment.RTD_ADC_Mean_Interval_Counter == rtd->measurment.RTD_Mean_Number){
			rtd->sampling_status = RTD_Sampling_Status_Finished;
			rtd->measurment.RTD_ADC_Mean_Interval_Counter =RESET;
		}
	}
}
/*************************SETTER FOR RAW ADC IN DMA MODE*************************************/
void RTD_Raw_ADC_Set_DMA(RTD_t* rtd){
	if(rtd->enable == SET){
		rtd->sampling_status = RTD_Sampling_Status_Ongoing;
		if(adh.Convertion_State == Convertion_Completed){
			rtd->measurment.RTD_ADC_Raw_Value = adh.ADC_Value_Raw[rtd->channel-1];
			rtd->measurment.RTD_ADC_Sampling_Buffer[rtd->measurment.RTD_ADC_Mean_Interval_Counter] = rtd->measurment.RTD_ADC_Raw_Value; /*Buffering the adc samples*/
			rtd->measurment.RTD_ADC_Mean_Interval_Counter ++;
			if(rtd->measurment.RTD_ADC_Mean_Interval_Counter == rtd->measurment.RTD_Mean_Number){
				rtd->sampling_status = RTD_Sampling_Status_Finished;
				rtd->measurment.RTD_ADC_Mean_Interval_Counter =RESET;
			}
			adh.Convertion_State = Convertion_Ongooing;
		}
		
	}
}
/*************************GETTER FOR RAW ADC *************************************/
uint16_t RTD_Raw_ADC_Get(RTD_t* rtd){
	return rtd->measurment.RTD_ADC_Raw_Value;
}
/*************************CALCULATOR FOR RAW ADC MEAN*************************************/
static void RTD_Raw_Adc_Mean(RTD_t* rtd,uint16_t* raw_adc){
	uint16_t interval=0;
	uint32_t sum = 0;
	uint16_t buff_raw_value;
	for(interval=0;interval< rtd->measurment.RTD_Mean_Number ; interval++){
		sum += raw_adc[interval];
		if( (raw_adc[interval+1] - raw_adc[interval] ) > 500){
			rtd->status = RTD_Status_Disconnection;
		}
		else{
			if(raw_adc[interval] < 4090 && raw_adc[interval] > 5){
				rtd->status = RTD_Status_Data_valid;
			}
			else{
				rtd->status = RTD_Status_Disconnection;
			}
			
		}
	}
	rtd->measurment.RTD_ADC_Mean_Value = sum / rtd->measurment.RTD_Mean_Number;
}

/*************************GETTER FOR RAW ADC MEAN*************************************/
uint16_t RTD_Raw_ADC_Mean_Get(RTD_t* rtd){
	return rtd->measurment.RTD_ADC_Mean_Value;
}






float RTD_Full_Convertion(RTD_t* rtd){
	uint16_t rtd_raw_adc_mean;
	float rtd_temp;
	
	/*Here is The Function That Reads The Actual ADC we have 2 method For this one :
	1 - Polling
	2 - DMA
	*/
	#if RTD_READING_METHOD == DMA_SETUP 
	RTD_Raw_ADC_Set_DMA(rtd); // Simply Reading ADC With This Function
	#elif RTD_READING_METHOD == POLLING_SETUP
	
	#endif
	
	if(rtd->enable == RESET){
		rtd->status= RTD_Status_Data_Unvalid;
		rtd->sampling_status = RTD_Sampling_Status_Hold;
	}
	if(rtd->sampling_status == RTD_Sampling_Status_Finished && rtd->enable == SET){
		RTD_Raw_Adc_Mean(rtd,rtd->measurment.RTD_ADC_Sampling_Buffer);  /*Meaning The ADC raw value base on the interval that has been set*/
		rtd_raw_adc_mean = RTD_Raw_ADC_Mean_Get(rtd);  						/*Gettig The ADC raw mean value*/
		RTD_Resistance_Read(rtd,rtd_raw_adc_mean);     						/*Calculating The RTD resistance */
		RTD_Tempreture_Read_Mathematicall(rtd,rtd->measurment.RTD_Resistance);           /*Calculating The RTD tempreture */
		rtd_temp = RTD_Tempreture_Get(rtd);                       /*Getting the RTD tempreture */
	}
	else{
		rtd_temp = rtd->measurment.RTD_Tempreture;    //we send the buffer output when we are sampling
	}
	
	
	return rtd_temp;
}



/******************************************************************************************************************
*	@DISCRIPTION DMA Callback Function on Reading ADC Value From various ADC Channels
*******************************************************************************************************************/
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc){
	adh.Convertion_State = Convertion_Completed;	
}












