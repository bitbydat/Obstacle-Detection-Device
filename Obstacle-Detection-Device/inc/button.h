#ifndef __BUTTON_H
#define __BUTTON_H


#include "allheader.h"

#define READ_PB11 !(GPIOB->IDR & GPIO_IDR_IDR11)

#define LED8_on() 	GPIOA->ODR != GPIO_ODR_ODR8
#define LED8_off() 	GPIOA->ODR &= ~GPIO_ODR_ODR8


void ButtonLED_Init(void);
void Buzzer_On(uint16_t millis);
void Buzzer_Off(void);

uint8_t ultrasonic_enable(void);
uint8_t active_mode(void);

#endif



