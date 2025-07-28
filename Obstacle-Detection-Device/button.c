#include "button.h"



void Button_Init(void)
{
	//Enable clock for AFIO
	RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;
	
	/*Init PB0 as EXTI for the emergency button to send SMS*/ 
	AFIO->EXTICR[0] |= 1<<0;										//Select source input for EXTI0 as PB
	EXTI->IMR |= 1<<0;													//Config ISR request from Line 0 is not masked
	EXTI->RTSR &= ~(1<<0);											
	EXTI->FTSR |= 1<<0;													//Select falling trigger for EXTI0 
	init_GP(PB, 0, IN, I_PP);										
	GPIOB->ODR |= (1<<0);												//Init PB0 as Input Pull-up	
	EXTI->PR |= EXTI_PR_PR0;										//Clear ISR flag before enabling ISR
	NVIC_SetPriority(EXTI0_IRQn, 5);						//Set priority (numberic) for EXTI0_IRQn >= configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY
																							//if necessary to use the .fromISR() functions
	NVIC_EnableIRQ(EXTI0_IRQn);									//Enable EXTI0_IRQn
	
	/*Init PA11 as EXTI for the stop button of the ultrasonic sensor measurement.*/ 
	AFIO->EXTICR[2] &= ~(0xF<<12);
	EXTI->IMR |= 1<<11;
	EXTI->RTSR &= ~(1<<11);
	EXTI->FTSR |= 1<<11;
	init_GP(PA, 11, IN, I_PP);
	GPIOA->ODR |= (1<<11);
	NVIC_EnableIRQ(EXTI15_10_IRQn);
	
	/*Init PA12 as EXTI for the stop button of the ultrasonic sensor measurement.*/ 
	AFIO->EXTICR[3] &= ~0xF;
	EXTI->IMR |= 1<<12;
	EXTI->RTSR &= ~(1<<12);
	EXTI->FTSR |= 1<<12;
	init_GP(PA, 12, IN, I_PP);
	GPIOA->ODR |= (1<<12);
	EXTI->PR |= EXTI_PR_PR12;
	NVIC_EnableIRQ(EXTI15_10_IRQn);
	
//	/*Init PB9 as EXTI for the stop button of the ultrasonic sensor measurement.*/ 
//	AFIO->EXTICR[2] |= 1<<4;
//	EXTI->IMR |= 1<<9;
//	EXTI->RTSR &= ~(1<<9);
//	EXTI->FTSR |= 1<<9;
//	init_GP(PB, 9, IN, I_PP);
//	GPIOB->ODR |= (1<<8);
//	NVIC_EnableIRQ(EXTI9_5_IRQn);
	
}





