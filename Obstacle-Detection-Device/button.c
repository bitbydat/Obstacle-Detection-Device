#include "button.h"



void ButtonLED_Init(void)
{

	RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;
	AFIO->EXTICR[0] |= 1<<0;
	EXTI->IMR |= 1<<0;
	EXTI->RTSR &= ~(1<<0);
	EXTI->FTSR |= 1<<0;
	init_GP(PB, 0, IN, I_PP);
	GPIOB->ODR |= (1<<0);
	NVIC_SetPriority(EXTI0_IRQn, 5);
	NVIC_EnableIRQ(EXTI0_IRQn);

	
	
	
	
//	init_GP(PB, 1, IN, I_PP);
//	GPIOB->ODR |= GPIO_ODR_ODR1;
//	
//	init_GP(PB, 8, IN, I_PP);
//	GPIOB->ODR |= GPIO_ODR_ODR8;
//	
//	init_GP(PB, 9, IN, I_PP);
//	GPIOB->ODR |= GPIO_ODR_ODR9;
//	
//	init_GP(PA, 8, OUT50, O_GP_PP);
	
	
}



uint8_t ultrasonic_enable(void)
{
	uint8_t flag = 1;
	static uint8_t last_state_8 = 0;
  uint8_t current_state = (GPIOB->IDR & GPIO_IDR_IDR8) ? 0 : 1;

  if (current_state && !last_state_8)
  {
      flag = 0;
  }
	else flag = 1;
  last_state_8 = current_state;
	return flag;
}

uint8_t active_mode(void)
{
	uint8_t flag = 1;
	static uint8_t last_state_9 = 0;
  uint8_t current_state = (GPIOB->IDR & GPIO_IDR_IDR9) ? 0 : 1;

  if (current_state && !last_state_9)
  {
      flag = 0;
  }
	else flag = 1;
  last_state_9 = current_state;
	return flag;
}


void Buzzer_On(uint16_t millis)
{
	GPIOA->ODR ^= GPIO_ODR_ODR11;
	vTaskDelay(pdMS_TO_TICKS(millis));
}

void Buzzer_Off(void)
{
	GPIOA->ODR &= ~GPIO_ODR_ODR11;
}


