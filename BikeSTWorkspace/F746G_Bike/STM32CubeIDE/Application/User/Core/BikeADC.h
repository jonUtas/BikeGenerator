/*
 * BikeADC.h
 *
 *  Created on: Jul 28, 2025
 *      Author: jrodden
 */

#ifndef APPLICATION_USER_CORE_BIKEADC_H_
#define APPLICATION_USER_CORE_BIKEADC_H_
#include "stm32f7xx_hal.h"

#define CHANNEL_COUNT 5
extern volatile uint16_t ADC_VAL[CHANNEL_COUNT];
long ADCMap(long x, long in_min, long in_max, long out_min, long out_max);
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc);

#endif /* APPLICATION_USER_CORE_BIKEADC_H_ */
