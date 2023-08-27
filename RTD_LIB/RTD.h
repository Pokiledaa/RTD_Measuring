#ifndef _RTD_H_
#define _RTD_H_
#include "stm32g0xx_hal.h"



#define POLLING_SETUP     0
#define DMA_SETUP         1
#define INTERRUP_SETUP    2

#define RTD_CHANNEL_NUM  3
#define RTD_MEANING_INTERVAL  50

#define RTD_READING_METHOD    DMA_SETUP



#define RTD_ADC            hadc1
#define RTD_CHANNEL_NUM    3

typedef struct {
	uint16_t RTD_ADC_Sampling_Buffer[1024];
	uint16_t RTD_ADC_Mean_Interval_Counter;
	uint16_t RTD_ADC_Raw_Value;
	uint16_t RTD_ADC_Mean_Value;
	uint16_t RTD_Mean_Number;
	float RTD_Resistance;
	float RTD_Tempreture;
}RTD_Measurming_t;



typedef enum {
	RTD_Status_Data_valid = 0,
	RTD_Status_Data_Unvalid = 1,
	RTD_Status_Disconnection = 4,
}RTD_Status_t;


typedef enum{
	Calibration_Done = 1u,
	Calibration_Ongoing = 2u,
	Calibration_Error = 3u,
}RTD_Calibration_Status_t;


typedef enum {
	RTD_Sampling_Status_Ongoing = 0,
	RTD_Sampling_Status_Finished = 1,
	RTD_Sampling_Status_Hold = 2,
}RTD_Sampling_status_t;

typedef enum {
	RTD_DISABLE = 0,
	RTD_ENABLE = 1,
}RTD_Enable_t;


typedef struct{
	uint16_t ADC_Raw_Max_Temp;
	uint16_t ADC_Raw_Min_Temp;
	float ADC_Max_Temp;
	float ADC_Min_Temp;
	float Gain_Factor;
	float Refrence_Resistor;
	
}RTD_Calibration_t;


typedef struct{
	RTD_Measurming_t measurment;
	RTD_Calibration_t calibration;
	RTD_Status_t status;
	RTD_Sampling_status_t sampling_status;
	RTD_Enable_t enable;
	uint8_t channel;
	float base_resistance;
}RTD_t;

typedef uint8_t RTD_Calibrate_status_Type;




void RTD_Enable(RTD_t* rtd);

/****************************************************************************************************
*
*	@DISCRIPTION : This function initialize the RTD Module base on their RTD OBJECT 
* 							 configuration and setup of the driver for reading the RTD Measurands
*
*****************************************************************************************************/
void RTD_Init();



/****************************************************************************************************
*
*	@DISCRIPTION : The purpose of This Function is to Calculate the RTD Resistance base
* 							 on this equation : RTD_RES = REF_RES + GAIN_FACTOR * ( ADC_RAW - ADC_RAW_MIN_TEMP)
* @param rtd : This is the rtd handler object which has all the rtd unit related parameter
* @param raw_measument : This input is the raw adc value that we calculate the rtd resistance base 
*                        on this value
* @CAUTION : This parameter will Fill the RTD object for getting the rtd resistance 
*            just call RTD_Resistance_get() after this fuction;
*
*****************************************************************************************************/
void RTD_Resistance_Read (RTD_t* rtd,uint16_t raw_measument);



/****************************************************************************************************
*
*	@DISCRIPTION : This function returns the rtd resistance 
* @param rtd : This is the rtd handler object which has all the rtd unit related parameter
*
*****************************************************************************************************/
float RTD_Resistance_Get (RTD_t* rtd);



/****************************************************************************************************
*
*	@DISCRIPTION : The purpose of This Function is to Calculate the RTD Tempreture
								 base on the mathematical calculation of poly nominal equation 
* @param rtd : This is the rtd handler object which has all the rtd unit related parameter
* @param rtd_resistance : This input is the rtd resistance value that we calculate the rtd tempreture
*													base on this value
* @CAUTION : This parameter will Fill the RTD object for getting the rtd resistance 
*            just call RTD_Resistance_get() after this fuction;
*
*****************************************************************************************************/
void RTD_Tempreture_Read_Mathematicall(RTD_t* rtd,float rtd_resistance);



/****************************************************************************************************
*
*	@DISCRIPTION : This function returns the rtd tempreture in cantigerad
* @param rtd : This is the rtd handler object which has all the rtd unit related parameter
*
*****************************************************************************************************/
float RTD_Tempreture_Get(RTD_t* rtd);



/****************************************************************************************************
*
*	@DISCRIPTION : This function Set the ADC Value For RTD Module you can use if in any senario
*                but designed to work with pooling method ,just pass the adc value to it and 
*								 after calling the the calculation methods you will have your tempreture calculated
* @param rtd : This is the rtd handler object which has all the rtd unit related parameter
*	@param raw_adc : The raw adc value for calculation of tempreture base on this
*
*****************************************************************************************************/
void RTD_Raw_ADC_Set(RTD_t* rtd,uint16_t raw_adc);



/****************************************************************************************************
*
*	@DISCRIPTION : This function Reads The ADC Value from DMA and if you desire to use it out of 
*                RTD_Full_Convertion() Function please call it in loop in first function
* @param rtd : This is the rtd handler object which has all the rtd unit related parameter
*
*****************************************************************************************************/
void RTD_Raw_ADC_Set_DMA(RTD_t* rtd);



/****************************************************************************************************
*
*	@DISCRIPTION : This function returns raw value of rtd sample 
* @param rtd : This is the rtd handler object which has all the rtd unit related parameter
*
*****************************************************************************************************/
uint16_t RTD_Raw_ADC_Get(RTD_t* rtd);



/****************************************************************************************************
*
*	@DISCRIPTION : This function returns the mean value of rtd samples in the defined interval
* @param rtd : This is the rtd handler object which has all the rtd unit related parameter
*
*****************************************************************************************************/
uint16_t RTD_Raw_ADC_Mean_Get(RTD_t* rtd);



/****************************************************************************************************
*
*	@DISCRIPTION : This function handles all the process of :
*								 1 - meaning the raw adc value 
*                2 - calculating the resistance
*                3 - calculating the tempreture
*
* @param rtd : This is the rtd handler object which has all the rtd unit related parameter
* @param Return : Returns the RTD calculated tempreture
* @CAUTION :  if you are using this function with pulling method remember to 
*             to call the method RTD_Raw_ADC_Set before this function and link the adc to
*             RTD module
*****************************************************************************************************/
float RTD_Full_Convertion(RTD_t* rtd);


void RTD_Calibrate_Set_ADC_Raw_Max_Temp (RTD_t* rtd , uint16_t adc_raw);
void RTD_Calibrate_Set_ADC_Raw_Min_Temp (RTD_t* rtd, uint16_t adc_raw);
void RTD_Calibrate_Set_ADC_Max_Temp (RTD_t* rtd , float temp);
void RTD_Calibrate_Set_ADC_Min_Temp (RTD_t* rtd, float temp);
void RTD_Calibrate_Set_Gain_Factor (RTD_t* rtd, float gf);
void RTD_Calibrate_Set_Refrence_Resistor (RTD_t* rtd ,float ref_resistor);
RTD_Calibrate_status_Type RTD_Calibration_Calculate_Gain_Factor (RTD_t* rtd);










#endif
