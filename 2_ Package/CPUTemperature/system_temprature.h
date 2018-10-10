#ifndef __SYSTEM_TEMPRATURE__H
#define __SYSTEM_TEMPRATURE__H

#include "stm32f4xx_adc.h"


void system_temprature_Init(void);
short getCPUtemperature(void);


#endif  /*system_temprature.h*/
