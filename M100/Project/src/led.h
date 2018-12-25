#ifndef __LED_H
#define __LED_H

#include "stm32f10x.h"




extern void LED_Configuration(void);
extern void LED_Blink(void);
extern void LED_Toggle(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);

#endif
