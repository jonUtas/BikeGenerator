/*
 * BikeADC.c
 *
 *  Created on: Jul 28, 2025
 *      Author: jrodden
 */


#include "main.h"
#include <cmsis_os2.h>
#include <stdbool.h>
#include "BikeADC.h"

	/*
		PA6		A0		ADC1	IN6		Rectifier volts
		PA4		A1		ADC1	IN4		Rectifier Amps
		PC2		A2		ADC1	IN12	Temperature
		PF10	A3		ADC3	IN8		Potentiometer
		PF8		A4		ADC3	IN6		SuperCap Volts
	 */

extern ADC_HandleTypeDef hadc1;
extern ADC_HandleTypeDef hadc3;

#define CHANNEL_COUNT 5
int ADC_Channel[] = {  6,  4,  12, 8,  6};// from table above
ADC_HandleTypeDef *ADC_Peripheral[] = {  &hadc1, &hadc1, &hadc1, &hadc3, &hadc3};// from table above
int adcIndex = 0;

volatile bool newData = false;

ADC_ChannelConfTypeDef adcConfig = {0};

uint16_t RectifierVoltsRaw = 0;
uint16_t RectifierAmpsRaw = 0;
uint16_t TemperatureRaw = 0;
uint16_t PotentiometerRaw = 0;
uint16_t SuperCap_VoltsRaw  = 0;


int RectifierVoltsMapped = 0;
int RectifierAmpsMapped = 0;
int TemperatureMapped = 0;
int PotentiometerMapped = 0;
int SuperCapVoltsMapped  = 0;


long ADCMap(long x, long in_min, long in_max, long out_min, long out_max)
{
  return (x - in_min) * (out_max - out_min + 1) / (in_max - in_min + 1) + out_min;
}


void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
	  newData = true;
}

void StartADC(ADC_HandleTypeDef *hadcPtr, int32_t channel)
{

	adcConfig.Channel = channel;
	if (HAL_ADC_ConfigChannel(hadcPtr, &adcConfig) != HAL_OK)
	{
		Error_Handler();
	}
	HAL_ADC_Start_IT(hadcPtr);
}


void Init_BikeADC_Task()
{
	adcIndex = 0;
	adcConfig.Channel = ADC_CHANNEL_0;
	adcConfig.Rank = ADC_REGULAR_RANK_1;
	adcConfig.SamplingTime = ADC_SAMPLETIME_15CYCLES;
//	HAL_ADCEx_Calibration_Start(&hadc1);
	StartADC(&hadc1, 6);
}

void Run_BikeADC_Task()
{
	GPIO_PinState backLightState = GPIO_PIN_SET;
	/* Infinite loop */
	for(;;)
	{
//		HAL_GPIO_WritePin(LCD_BL_CTRL_GPIO_Port, LCD_BL_CTRL_Pin, backLightState);
		if(backLightState == GPIO_PIN_SET)
		{
			backLightState = GPIO_PIN_RESET;
		}
		else
		{
			backLightState = GPIO_PIN_SET;
		}
		if(newData)
		{
			newData = false;
			uint32_t rawADC = HAL_ADC_GetValue(ADC_Peripheral[adcIndex]);
			switch(adcIndex)
			{
				case 0: {RectifierVoltsRaw = rawADC;} break;
				case 1: {RectifierAmpsRaw = rawADC;} break;
				case 2: {TemperatureRaw = rawADC;} break;
				case 3: {PotentiometerRaw = rawADC;} break;
				case 4: {SuperCap_VoltsRaw = rawADC;} break;
				default: Error_Handler();

			}
			// todo use float? check map. low pass filter?

//			rectifierVoltsMapped = ADCMap(rectifierVoltsRaw, 1700, 65535, 0, 100);// todo use float? check map. low pass filter?
//			Rectifier_AmpsMapped = ADCMap(RectifierAmpsRaw, 1700, 65535, 0, 100);
//			TemperatureMapped = ADCMap(TemperatureRaw, 1700, 65535, 0, 100);
//			PotentiometerMapped = ADCMap(PotentiometerRaw, 1700, 65535, 0, 100);
//			SuperCap_VoltsMapped  = ADCMap(SuperCap_VoltsRaw, 1700, 65535, 0, 100);
			adcIndex++;
			if(adcIndex >= CHANNEL_COUNT )
			{
				adcIndex = 0;
			}
			StartADC(ADC_Peripheral[adcIndex], adcIndex);

		}
		osDelay(1000);
	}
}
