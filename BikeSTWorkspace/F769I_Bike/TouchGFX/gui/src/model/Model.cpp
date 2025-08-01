#include <gui/model/Model.hpp>
#include <gui/model/ModelListener.hpp>
#include "FreeRTOS.h"
#include "queue.h"
#include "globals.h"
#include <cmsis_os2.h>

extern "C"
{
    extern osMessageQueueId_t adcQueueHandle;
    extern osMessageQueueId_t buttonQueueHandle;
}


unsigned int counter = 0;

Model::Model() : modelListener(0), ADC_value (0), Button_State(false)
{

}

void Model::tick()
{

	static int power;
	if (osMessageQueueGetCount(adcQueueHandle) > 0)
	{
		if (osMessageQueueGet(adcQueueHandle, &power, 0U, 0) == osOK)
		{
			modelListener->setADC(power);
		}
	}

}

