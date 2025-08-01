/*
 * globals.h
 *
 *  Created on: Jul 28, 2025
 *      Author: jrodden
 */

#ifndef APPLICATION_USER_CORE_GLOBALS_H_
#define APPLICATION_USER_CORE_GLOBALS_H_

#include <stdbool.h>
#include <stdint.h>
#include <FreeRTOS.h>




#ifdef __cplusplus
extern "C" {
#endif

struct ADCState {   // Structure declaration
	  uint32_t  Power;           // Member (int variable)
	  uint32_t  Volts;           // Member (int variable)
	  uint32_t  Current;           // Member (int variable)
	  uint32_t  Speed;           // Member (int variable)
	  uint32_t  Ticks;           // Member (int variable)
};


extern bool test;
extern int globalCount;


#ifdef __cplusplus
}
#endif

#endif /* APPLICATION_USER_CORE_GLOBALS_H_ */
