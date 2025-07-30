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
		PA0		A0		ADC3	IN0		Rectifier volts
		PF7		A4		ADC3	IN5		SuperCap Volts
		PF8		A3		ADC3	IN6		Potentiometer
		PF9		A2		ADC3	IN7		Temperature
		PF10	A1		ADC3	IN8		Rectifier Amps

	 */
#define CHANNEL_COUNT 5
enum ADC_CHANNEL {  RectifierVolts = 0,  RectifierAmps = 8,  Temperature = 7, Potentiometer = 6,  SuperCapVolts = 5};

extern ADC_HandleTypeDef hadc3;

volatile bool newData = false;
enum ADC_CHANNEL adcIndex = RectifierVolts;

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
void StartADC(int32_t channel)
{
//	HAL_ADCEx_Calibration_Start(&hadc1);
	adcConfig.Channel = channel;
	if (HAL_ADC_ConfigChannel(&hadc3, &adcConfig) != HAL_OK)
	{
		Error_Handler();
	}
	HAL_ADC_Start_IT(&hadc3);
}


void Init_BikeADC_Task()
{
	adcConfig.Channel = ADC_CHANNEL_0;
	adcConfig.Rank = ADC_REGULAR_RANK_1;
	adcConfig.SamplingTime = ADC_SAMPLETIME_15CYCLES;
	StartADC(0);
}

void Run_BikeADC_Task()
{
	GPIO_PinState backLightState = GPIO_PIN_SET;
	/* Infinite loop */
	for(;;)
	{
		HAL_GPIO_WritePin(LCD_BL_CTRL_GPIO_Port, LCD_BL_CTRL_Pin, backLightState);
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
			uint32_t rawADC = HAL_ADC_GetValue(&hadc3);
			switch(adcIndex)
			{
				case RectifierVolts: { RectifierVoltsRaw = rawADC; adcIndex = RectifierAmps;} break;
				case RectifierAmps: { RectifierAmpsRaw = rawADC; adcIndex = Temperature;} break;
				case Temperature: { TemperatureRaw = rawADC; adcIndex = Potentiometer;} break;
				case Potentiometer: { PotentiometerRaw = rawADC; adcIndex = SuperCapVolts;} break;
				case SuperCapVolts: { SuperCap_VoltsRaw = rawADC; adcIndex = RectifierVolts;} break;
			}

//			rectifierVoltsMapped = ADCMap(rectifierVoltsRaw, 1700, 65535, 0, 100);// todo use float? check map. low pass filter?
//			Rectifier_AmpsMapped = ADCMap(RectifierAmpsRaw, 1700, 65535, 0, 100);
//			TemperatureMapped = ADCMap(TemperatureRaw, 1700, 65535, 0, 100);
//			PotentiometerMapped = ADCMap(PotentiometerRaw, 1700, 65535, 0, 100);
//			SuperCap_VoltsMapped  = ADCMap(SuperCap_VoltsRaw, 1700, 65535, 0, 100);
			StartADC(adcIndex);
		}
		osDelay(1000);
	}
}
