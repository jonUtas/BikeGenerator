/*
 * BikeADC.h
 *
 *  Created on: Jul 28, 2025
 *      Author: jrodden
 */

#ifndef APPLICATION_USER_CORE_BIKEADC_H_
#define APPLICATION_USER_CORE_BIKEADC_H_
#include "stm32f7xx_hal.h"

void Init_BikeADC_Task();
void Run_BikeADC_Task();
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc);

#endif /* APPLICATION_USER_CORE_BIKEADC_H_ */
