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


#define CHANNEL_COUNT 5

volatile uint16_t ADC_VAL[CHANNEL_COUNT];
extern ADC_HandleTypeDef hadc3;

bool newData = false;
uint16_t rectifierVoltsRaw = 0;
uint16_t Rectifier_AmpsRaw = 0;
uint16_t TemperatureRaw = 0;
uint16_t PotentiometerRaw = 0;
uint16_t SuperCap_VoltsRaw  = 0;

int rectifierVoltsMapped = 0;
int Rectifier_AmpsMapped = 0;
int TemperatureMapped = 0;
int PotentiometerMapped = 0;
int SuperCap_VoltsMapped  = 0;


long ADCMap(long x, long in_min, long in_max, long out_min, long out_max)
{
  return (x - in_min) * (out_max - out_min + 1) / (in_max - in_min + 1) + out_min;
}


void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
	/*
		PA0		A0		ADC3	IN0		Rectifier volts
		PF7		A4		ADC3	IN5		SuperCap Volts
		PF8		A3		ADC3	IN6		Potentiometer
		PF9		A2		ADC3	IN7		Temperature
		PF10	A1		ADC3	IN8		Rectifier Amps

	 */
	  rectifierVoltsRaw = ADC_VAL[0];
//	  Rectifier_AmpsRaw = ADC_VAL[1];
//	  TemperatureRaw = ADC_VAL[2];
//	  PotentiometerRaw = ADC_VAL[3];
//	  SuperCap_VoltsRaw  = ADC_VAL[4];
	  newData = true;
	if(hadc->Instance==ADC3)
	{
		HAL_ADC_Start_DMA(&hadc3, (uint32_t *)ADC_VAL, CHANNEL_COUNT);
	}
}

void Init_BikeADC_Task()
{
	HAL_ADC_Start_DMA(&hadc3, (uint32_t *)ADC_VAL, CHANNEL_COUNT);
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
//			rectifierVoltsMapped = ADCMap(rectifierVoltsRaw, 1700, 65535, 0, 100);// todo use float? check map. low pass filter?
//			Rectifier_AmpsMapped = ADCMap(Rectifier_AmpsRaw, 1700, 65535, 0, 100);
//			TemperatureMapped = ADCMap(TemperatureRaw, 1700, 65535, 0, 100);
//			PotentiometerMapped = ADCMap(PotentiometerRaw, 1700, 65535, 0, 100);
//			SuperCap_VoltsMapped  = ADCMap(SuperCap_VoltsRaw, 1700, 65535, 0, 100);
		}
		osDelay(1000);
	}
}
