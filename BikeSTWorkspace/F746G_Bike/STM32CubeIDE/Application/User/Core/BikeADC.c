/*
 * BikeADC.c
 *
 *  Created on: Jul 28, 2025
 *      Author: jrodden
 */


#include "BikeADC.h"

volatile uint16_t ADC_VAL[CHANNEL_COUNT];
int count = 0;
int value = 0;

long ADCMap(long x, long in_min, long in_max, long out_min, long out_max)
{
  return (x - in_min) * (out_max - out_min + 1) / (in_max - in_min + 1) + out_min;
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
	  value = ADCMap(ADC_VAL[0], 1700, 65535, 0, 100);
}
