#ifndef __BUTTON_H
#define __BUTTON_H


#include "allheader.h"

#define READ_PB11 !(GPIOB->IDR & GPIO_IDR_IDR11)

#define LED8_on() 	GPIOA->ODR != GPIO_ODR_ODR8
#define LED8_off() 	GPIOA->ODR &= ~GPIO_ODR_ODR8


void Button_Init(void);

#endif



